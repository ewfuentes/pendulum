#include "pendulum.hh"

#include "shader.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

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
    // Projection Matrix
    // Use an orthographic projection with a window width of 5x5m
    glm::mat4 projection = glm::ortho<glm::float32>(-2,2,-1.5,1.5, 0.25, 1);
    // View Matrix
    // Set the camera to 0, 0, -.5
    glm::mat4 view = glm::lookAt(glm::vec3(0, 0, 0.5),  // Camera is at 0, 0, -.5
                                 glm::vec3(0.0, 0.0, 0.0),  // look at the origin
                                 glm::vec3(0.0, 1.0, 0.0));  // The positive y axis is up
    // Draw stick
    // Model Transform matrix
    glm::float32 theta = s(0);
    // glm::float32 theta = 0;
    glm::mat4 model_scale = glm::scale<glm::float32>(glm::mat4(1.0f), glm::vec3( 0.05, length_ / 2.0f, 1));
    glm::mat4 model_rotate = glm::rotate<glm::float32>(glm::mat4(1.0f), theta, glm::vec3(0, 0, 1.0));
    glm::vec3 trans = glm::vec3(-glm::sin(theta), glm::cos(theta), 0) * 0.5f * static_cast<glm::float32>(length_);
    glm::mat4 model_translate = glm::translate<glm::float32>(glm::mat4(1.0f), trans); 
    glm::mat4 stick_model = model_translate * model_rotate * model_scale;
    glm::mat4 stick_MVP = projection * view * stick_model;

    // Pass it to the shader
    GLuint MatrixID = glGetUniformLocation(shader_prog_id, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &stick_MVP[0][0]);

    // Draw the stick
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

    // Draw the mass
    // Model Transform matrix
    model_scale = glm::scale<glm::float32>(glm::mat4(1.0f), glm::vec3( 0.125, 0.125, 1));
    model_rotate = glm::rotate<glm::float32>(glm::mat4(1.0f), theta, glm::vec3(0, 0, 1.0));
    trans = glm::vec3(-glm::sin(theta), glm::cos(theta), 0) * static_cast<glm::float32>(length_);
    model_translate = glm::translate<glm::float32>(glm::mat4(1.0f), trans); 
    glm::mat4 mass_model = model_translate * model_rotate * model_scale;
    glm::mat4 mass_MVP = projection * view * mass_model;

    // Pass it to the shader
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mass_MVP[0][0]);

    // Draw the stick
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
