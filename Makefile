CXX = clang++
CXXFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -c -ggdb
LDFLAGS = -std=c++14 -Werror -Wall -Wextra -Wpedantic -lBox2D -lGLEW -lglfw -lGL

all: main compile_commands.json

CPPSRC = main.cc pendulum.cc
OBJS = main.o pendulum.o

%.o : %.cc
	$(CXX) $< -o $@ -MJ $<.json $(CXXFLAGS)

main : $(OBJS)
	$(CXX) $^ -o $@ $(LDFLAGS)

compile_commands.json: main.cc.json pendulum.cc.json
	./merge_compile.py $^

clean:
	rm -f main
	rm -f *.json
	rm -f *.o

.PHONY: compile_commands all clean
