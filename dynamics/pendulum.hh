#include "dynamics/dynamical_system.hh"

namespace dynamics {
namespace pendulum {
constexpr int STATE_DIM = 2;
constexpr int CONTROL_DIM = 1;
class Pendulum : public DynamicalSystem<STATE_DIM, CONTROL_DIM> {
 public:
    enum States {
        POSITION = 0,
        VELOCITY = 1,
    };

    enum Controls {
        TORQUE = 0,
    };

    struct Config {
        double mass = -1;
        double length = -1;
        double gravity = -1;
        double dt = -1;
    };

    Pendulum(const Config &config);
    State simulate(const State &x_0, const Control &u);

 private:
    const Config config_;
};
}  // namespace pendulum
}  // namespace dynamics
