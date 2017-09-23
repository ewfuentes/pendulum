#include <math.h>
#include <stdio.h>

#include <iostream>

#include <Box2D/Box2D.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "pendulum.hh"
#include "shader.hh"

GLFWwindow *window;

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

int main( void )
{
    window = create_window(1024, 768);
    if (!window) {
        return -1;
    }

    glfwSwapInterval(2);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
    
    Pendulum pen(.05, 3, 1, 9.81);
    State s{.9 * M_PI, 0};

	do{
		// Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
        double u = .1*s(1);
        if (std::abs(u) > .1) {
            u = std::copysign(.5, u);
        }
        std::cout << u << std::endl;
        s = pen.simulate(s, u);
        pen.render(window, s);
        
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

