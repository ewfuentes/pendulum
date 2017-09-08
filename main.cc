#include <math.h>
#include <stdio.h>

#include <iostream>

#include <Box2D/Box2D.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "pendulum.hh"

void glfw_error_func(int error_code, const char *description) {
    std::cout << "Error code: " << error_code << " " << description << std::endl;
}

GLFWwindow *create_window(int width, int height) {
    GLFWwindow *win = nullptr;
    (void)width;
    (void)height;

    glfwSetErrorCallback(&glfw_error_func);
    if (!glfwInit()) {
        return nullptr;
    }

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE,GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win = glfwCreateWindow(width, height, "Pendulum", nullptr, nullptr); 
    if (!win) {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(win);

    glewExperimental = true;
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return nullptr;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(win, GLFW_STICKY_KEYS, GL_TRUE);
    return win;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; 
    GLFWwindow *window = create_window(1024, 768);
    if (!window) {
        return 1;
    }

    glfwSwapInterval(1);
    Pendulum pen;
    while (!glfwWindowShouldClose(window)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        pen.render(window, {{.9 * M_PI, 0}});
        

        glfwSwapBuffers(window);  

        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

//int main(int argc, char **argv) {
//    (void)argc;
//    (void)argv;
//    std::cout << "Hello, World!" << std::endl;
//
//    std::cout << "test string" << std::endl;
//    
//    Pendulum pen = Pendulum(.01, 1, 1, 9.81);
//    std::vector<double> u(5000, .5);
//    std::vector<State> states = pen.simulate({{M_PI - M_PI/10, 0}}, u);
//
//    FILE *in;
//
//    if (!(in = popen("gnuplot", "w"))) {
//        std::cout << "Couldn't open gnuplot" << std::endl;
//    }
//    fprintf(in, "set terminal x11\r\n");
//    fprintf(in, "reset\r\n");
//    fprintf(in, "plot '-' using 1:2 with lines\r\n");
//    double t = 0;
//    for (auto &s : states) {
//        fprintf(in, "%f %f %f\r\n", t, s[0], s[1]);
//        std::cout << t << " " << s[0] << " " << s[1] << std::endl;
//        t += .01;
//    }
//    fprintf(in, "e\r\n");
//    fflush(in);
//    getchar();
//
//    pclose(in);
//}
