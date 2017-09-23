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

CPPSRC = main.cc pendulum.cc pendulum_render.cc shader.cc
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

.PHONY: compile_commands all clean
