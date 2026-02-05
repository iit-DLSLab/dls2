
#pragma once

#include <vector>
#include <cstddef>
#include <cmath>
#include <limits>
#include <cassert>
#include <algorithm>

namespace dls
{
namespace math
{

	struct FrenetSample {
		double x, y;    // sample point in world frame
		double s;       // arc-length along path (at point)
		double psi;     // tangent heading (radians)
		double nx, ny;  // left-hand normal unit vector
	};

	struct ProjectResult {
		double s;           // arc-length at projection
		double ey;          // signed lateral error (positive = left)
		double epsi;        // heading error: (robot_psi - path_psi) normalized to [-pi,pi)
		std::size_t idx;    // index of path point before projection (segment 0..N-2)
		double t_on_seg;    // param along segment in [0,1]
		double dist2;       // squared distance from point to projection
	};

	inline double clamp01(double v) { return (v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v)); }
	inline double normalizeAngle(double a) {
		// normalize to [-pi, pi)
		const double TWO_PI = 2.0 * M_PI;
		a = std::fmod(a + M_PI, TWO_PI);
		if (a < 0) {
			a += TWO_PI;
		}
		return a - M_PI;
	}

	class FrenetPath {
	public:
		FrenetPath() = default;

		/**
		 * @brief Building from discrete samples (x,y). Requires at least 2 points.
		 * 
		 * @param x_coords 
		 * @param y_coords 
		 */
		void buildFromXY(const std::vector<double>& x_coords, const std::vector<double>& y_coords);

		size_t size() const;

		/**
		 * @brief Project query point (xq,yq,psi_q) onto the discrete path.
		 * 
		 * @param xq 
		 * @param yq 
		 * @param psi_q 
		 * @param last_index_hint a previously returned idx (or 0) to speed search and avoid jumps.
		 * @param max_index_window how many segments to search around the hint (if 0 => full scan).
		 * @param full_scan_allowed if true and hint-window search fails to find a good projection, it will fallback to full scan
		 * @return ProjectResult 
		 */
		ProjectResult projectPoint(double xq, double yq, double psi_q,
								size_t last_index_hint = 0,
								size_t max_index_window = 8,
								bool full_scan_allowed = true) const;

		/**
		 * @brief  Utility returning nearest (x,y) on path for a given s value by locating segment and linear interp.
		 * Not highly optimized but useful.
		 * 
		 * @param s_query 
		 * @param x_out 
		 * @param y_out 
		 * @return true 
		 * @return false 
		 */
		bool getXYAtS(double s_query, double &x_out, double &y_out) const;

	private:
		std::vector<FrenetSample> samples_;
		std::vector<double> seg_lengths_; // size N-1

		void unwrapPsi();
	};

} // namespace math
} // namespace dls
