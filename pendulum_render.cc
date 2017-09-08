#include "pendulum.hh"

#include "shader.hh"

#include <glm/glm.hpp>

static glm::float32 square[] = {
    -1,  1, 0,
    -1, -1, 0,
     1, -1, 0,
    -1,  1, 0,
     1,  1, 0,
     1, -1, 0,
};

static GLuint vertexbuffer;
static GLuint shader_prog_id;

void Pendulum::render_init() {
    glGenBuffers(1, &vertexbuffer);    
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    shader_prog_id = LoadShaders(
        "square_drawer_vert.glsl", "square_drawer_frag.glsl");
}

void Pendulum::render(GLFWwindow *window, const State &s) {
    (void)window;
    (void)s;
    glUseProgram(shader_prog_id);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);
}
