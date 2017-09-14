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

    glfwSwapInterval(1);
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
    
    Pendulum pen;

//	static const GLfloat g_vertex_buffer_data[] = { 
//		-1.0f, -1.0f, 0.0f,
//		 1.0f, -1.0f, 0.0f,
//		 0.0f,  1.0f, 0.0f,
//	};
//
//    GLuint vertexbuffer;
//    glGenBuffers(1, &vertexbuffer);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//	GLuint programID = LoadShaders("square_drawer_vert.glsl", "square_drawer_frag.glsl" );

	do{

		// Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
        pen.render(window, State{{0.9*M_PI, 0}});
//		glUseProgram(programID);
//		glEnableVertexAttribArray(0);
//		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//		glVertexAttribPointer(
//			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
//			3,                  // size
//			GL_FLOAT,           // type
//			GL_FALSE,           // normalized?
//			0,                  // stride
//			(void*)0            // array buffer offset
//		);

		// Draw the triangle !
//		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

//		glDisableVertexAttribArray(0);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO
//	glDeleteBuffers(1, &vertexbuffer);
//	glDeleteVertexArrays(1, &VertexArrayID);
//	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
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
