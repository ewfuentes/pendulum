#pragma once
#include <vector>
#include <array>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <Eigen/Core>

using State = Eigen::Vector2d;

enum StateId {
    POSITION = 0,
    VELOCITY,
    NUM_STATES
};

class Pendulum {
friend class PendulumPlanner;
 public:
    Pendulum();
    Pendulum(const double dt,
             const double mass,
             const double length,
             const double g);     
    State simulate(const State x_0, const double u);
    std::vector<State> simulate(const State &x_0, 
                                const std::vector<double> &u); 
    void render(GLFWwindow *window, const State &s);

    Eigen::Matrix2d compute_state_jacobian(const State &x);
    std::vector<Eigen::Matrix2d> compute_state_jacobian(const std::vector<State> &x);
    Eigen::Vector2d compute_contol_jacobian(const double u);
    std::vector<Eigen::Vector2d> compute_contol_jacobian(const std::vector<double> &u);
 private:
    void render_init();

    const double dt_;
    const double mass_;
    const double length_;
    const double gravity_;
};
