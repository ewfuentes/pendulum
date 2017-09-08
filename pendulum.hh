#pragma once
#include <vector>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using State = std::array<double,2>;

class Pendulum {
 public:
    Pendulum();
    Pendulum(const double dt,
             const double mass,
             const double length,
             const double g);     
    State simulate(const State x_0, const double u);
    std::vector<State> simulate(const State x_0, 
                                const std::vector<double> &u); 
    void render(GLFWwindow *window, const State &s);
 private:
    void render_init();

    const double dt;
    const double mass;
    const double length;
    const double gravity;
};
