CXXFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -ggdb -isystem /usr/include/eigen3
LDFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -lBox2D -lGLEW -lglfw
GTEST_LIBS = gtest/libgtest.a gtest/libgtest_main.a

OS = $(shell uname -s)
ifeq ($(OS), Darwin)
	CXX = /usr/local/opt/llvm/bin/clang++
 	LDFLAGS += -framework OpenGL
else
	CXX = clang++-5.0
	CXXFLAGS += -stdlib=libstdc++
	LDFLAGS += -lGL
endif

all: main compile_commands.json

# CPPSRC = main.cc pendulum.cc pendulum_render.cc shader.cc
CPPSRC = ilqr.cc numerical_diff.cc
OBJS = $(patsubst %.cc,%.o,$(CPPSRC))
JSON = $(addsuffix .json,$(CPPSRC))

%.o %.cc.json : %.cc
	$(CXX) -c $< -o $@ -MJ $<.json $(CXXFLAGS)

main : $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

compile_commands.json: $(JSON) 
	./merge_compile.py $^

clean:
	rm -f main
	rm -f ilqr_test
	rm -f numerical_diff_test
	rm -f *.json
	rm -f *.o

ilqr_test: ilqr_test.cc gtest/*.a
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(CXXFLAGS) -lpthread

numerical_diff_test: numerical_diff_test.cc gtest/*.a
	$(CXX) $^ $(GTEST_LIBS) -o $@ $(CXXFLAGS) -lpthread

.PHONY: compile_commands all clean
