
namespace ilqr {
template <class T> typename NaiveIlqr<T>::Trajectory
NaiveIlqr<T>::solve(const typename T::State &x0, const std::vector<typename T::Control> &u) const {
    return Trajectory{};
}
}  // namespace ilqr
