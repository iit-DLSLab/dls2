#include "dls2/math/frenet_path.hpp"

using namespace dls::math;

void FrenetPath::buildFromXY(const std::vector<double>& x_coords, const std::vector<double>& y_coords) 
{
	assert(x_coords.size() == y_coords.size());
	size_t N = x_coords.size();
	assert(N >= 2);

	samples_.clear();
	samples_.reserve(N);
	for (size_t i = 0; i < N; ++i) {
		FrenetSample s; s.x = x_coords[i]; s.y = y_coords[i];
		s.s = 0.0; s.psi = 0.0; s.nx = 0.0; s.ny = 0.0;
		samples_.push_back(s);
	}

	// compute arc-length s
	samples_[0].s = 0.0;
	for (size_t i = 1; i < N; ++i) {
		double dx = samples_[i].x - samples_[i-1].x;
		double dy = samples_[i].y - samples_[i-1].y;
		double seglen = std::hypot(dx, dy);
		samples_[i].s = samples_[i-1].s + seglen;
		seg_lengths_.push_back(seglen);
	}
	// seg_lengths_.size() == N-1

	// compute psi using central diff, endpoints by forward/backward
	for (size_t i = 0; i < N; ++i) {
		if (i == 0) {
			double dx = samples_[1].x - samples_[0].x;
			double dy = samples_[1].y - samples_[0].y;
			samples_[0].psi = std::atan2(dy, dx);
		} else if (i == N-1) {
			double dx = samples_[N-1].x - samples_[N-2].x;
			double dy = samples_[N-1].y - samples_[N-2].y;
			samples_[N-1].psi = std::atan2(dy, dx);
		} else {
			double dx = samples_[i+1].x - samples_[i-1].x;
			double dy = samples_[i+1].y - samples_[i-1].y;
			samples_[i].psi = std::atan2(dy, dx);
		}
	}
	// unwrap psi to be continuous
	unwrapPsi();

	// compute normals (left-hand normal) at each sample
	for (size_t i = 0; i < N; ++i) {
		samples_[i].nx = -std::sin(samples_[i].psi);
		samples_[i].ny =  std::cos(samples_[i].psi);
	}
	// clear any previous data we might want
}

size_t FrenetPath::size() const { return samples_.size(); }

ProjectResult FrenetPath::projectPoint(double xq, double yq, double psi_q,
						size_t last_index_hint,
						size_t max_index_window,
						bool full_scan_allowed) const
{
	const size_t N = samples_.size();
	assert(N >= 2);

	auto project_on_segment = [&](size_t i_seg, double &t, double &cx, double &cy, double &seg_len) 
	{
		// segment from samples_[i_seg] -> samples_[i_seg+1]
		const auto &A = samples_[i_seg];
		const auto &B = samples_[i_seg+1];
		double vx = B.x - A.x;
		double vy = B.y - A.y;
		seg_len = std::hypot(vx, vy);
		if (seg_len <= 1e-12) 
		{ 
			t = 0.0; 
			cx = A.x; 
			cy = A.y; 
			return; 
		}
		double inv_denom = 1.0 / (vx*vx + vy*vy);
		double wx = xq - A.x;
		double wy = yq - A.y;
		t = (wx*vx + wy*vy) * inv_denom;
		t = clamp01(t);
		cx = A.x + t * vx;
		cy = A.y + t * vy;
	};

	// Determine candidate segment index range
	size_t start_seg = 0;
	size_t end_seg = N - 2;
	if (max_index_window > 0 && last_index_hint < N-1) {
		// center window at last_index_hint, clamp to bounds
		size_t low = (last_index_hint >= max_index_window ? last_index_hint - max_index_window : 0u);
		size_t high = std::min(last_index_hint + max_index_window, N - 2);
		start_seg = low;
		end_seg = high;
	}

	// Search function (returns best result in given range)
	auto search_range = [&](size_t s_idx, size_t e_idx) -> ProjectResult 
	{
		ProjectResult best;
		best.dist2 = std::numeric_limits<double>::infinity();
		best.idx = s_idx;
		best.s = 0.0;
		best.ey = 0.0;
		best.epsi = 0.0;
		best.t_on_seg = 0.0;

		for (size_t i = s_idx; i <= e_idx; ++i) {
			double t, cx, cy, seglen;
			project_on_segment(i, t, cx, cy, seglen);
			double dx = xq - cx;
			double dy = yq - cy;
			double d2 = dx*dx + dy*dy;
			if (d2 < best.dist2) {
				best.dist2 = d2;
				best.idx = i;
				best.t_on_seg = t;
				// compute s at projection
				double s_at_proj = samples_[i].s + t * seg_lengths_[i];
				best.s = s_at_proj;
				// segment tangent psi (interpolate between sample headings)
				// simple approach: use samples_[i].psi (or compute from seg vector)
				double seg_vx = samples_[i+1].x - samples_[i].x;
				double seg_vy = samples_[i+1].y - samples_[i].y;
				double seg_psi = std::atan2(seg_vy, seg_vx);
				// normalize relative to precomputed psi (avoid unwrap issues)
				// compute normal (left-hand)
				double nx = -std::sin(seg_psi);
				double ny =  std::cos(seg_psi);
				// signed lateral error = dot((query - closest), normal)
				best.ey = dx * nx + dy * ny; // positive = left of the tangent
				// heading error: robot_psi - path_psi (we follow convention robot minus path)
				best.epsi = normalizeAngle(psi_q - seg_psi);
			}
		}
		return best;
	};

	// First try local window
	ProjectResult res = search_range(start_seg, end_seg);

	// If local search was not allowed to cover whole path and result seems bad,
	// optionally fall back to full scan (for first call or big jumps).
	if (full_scan_allowed && max_index_window > 0) {
		// heuristics: if projection distance is huge vs local dx (~maybe path far) or we started with hint=0,
		// do a full scan to be safe (this covers first-call case).
		const double LARGE_DIST2_THRESH = 1e6; // arbitrary, but safe
		if (res.dist2 > LARGE_DIST2_THRESH || last_index_hint == 0) {
			ProjectResult res_all = search_range(0, N-2);
			if (res_all.dist2 < res.dist2) res = res_all;
		}
	} else if (max_index_window == 0) {
		// user requested full scan
		res = search_range(0, N-2);
	}

	return res;
}

bool FrenetPath::getXYAtS(double s_query, double &x_out, double &y_out) const 
{
	if (samples_.empty()) 
	{
		return false;
	}
	const size_t N = samples_.size();
	if (s_query <= samples_.front().s) 
	{ 
		x_out = samples_.front().x;
		y_out = samples_.front().y;
		return true;
	}
	if (s_query >= samples_.back().s)   
	{ 
		x_out = samples_.back().x;
		y_out = samples_.back().y;
		return true;
	}
	// binary search for segment
	size_t lo = 0;
	size_t hi = N-1;
	while (hi - lo > 1) {
		size_t mid = (lo + hi) >> 1;
		if (samples_[mid].s <= s_query){
			lo = mid;
		}else {
			hi = mid;
		}
	}
	double seg_s = s_query - samples_[lo].s;
	double seglen = seg_lengths_[lo];
	double t = (seglen > 1e-12 ? seg_s / seglen : 0.0);
	t = clamp01(t);
	x_out = samples_[lo].x + t * (samples_[lo+1].x - samples_[lo].x);
	y_out = samples_[lo].y + t * (samples_[lo+1].y - samples_[lo].y);
	return true;
}

void FrenetPath::unwrapPsi() 
{
	if (samples_.empty()) {
		return;
	}

	double prev = samples_[0].psi;
	for (size_t i = 1; i < samples_.size(); ++i) {
		double cur = samples_[i].psi;
		double delta = cur - prev;

		// bring delta to [-pi,pi)
		delta = normalizeAngle(delta);
		samples_[i].psi = samples_[i-1].psi + delta;
		prev = samples_[i].psi;
	}
}
