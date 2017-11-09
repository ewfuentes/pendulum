CXXFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -c -ggdb -isystem /usr/include/eigen3
LDFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -lBox2D -lGLEW -lglfw
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
	$(CXX) $< -o $@ -MJ $<.json $(CXXFLAGS)

main : $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

compile_commands.json: $(JSON) 
	./merge_compile.py $^

clean:
	rm -f main
	rm -f *.json
	rm -f *.o

ilqr_test: ilqr_test.cc ilqr.cc ilqr.hh numerical_diff.cc numerical_diff.hh gtest/*.a
	$(CXX) ilqr_test.cc ilqr.cc numerical_diff.cc gtest/libgtest.a gtest/libgtest_main.a -o $@ -isystem /usr/include/eigen3 -std=c++14 -lpthread

.PHONY: compile_commands all clean
