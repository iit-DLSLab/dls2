#include "dls2/math/frenet_path.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<double> xs = {0.0, 1.0, 2.0, 2.5, 3.0};
    std::vector<double> ys = {0.0, 0.0, 0.2, 0.8, 1.2};
    dls::math::FrenetPath path;
    path.buildFromXY(xs, ys);

    double xq = 1.2;
    double yq = 0.0;
    double psi_q = 0.2;
    std::size_t last_hint = 1;
    auto res = path.projectPoint(xq, yq, psi_q, last_hint, /*window=*/6, /*full_scan_allowed=*/true);

    std::cout << "s=" << res.s << " ey=" << res.ey << " epsi=" << res.epsi
              << " idx=" << res.idx << " t=" << res.t_on_seg << " dist2=" << res.dist2 << std::endl;
    return 0;
}