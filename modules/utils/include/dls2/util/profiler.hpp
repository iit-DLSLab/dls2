#pragma once

#include <chrono>
#include <unordered_map>
#include <string>
#include <cstddef>
#include <sstream>

#include "dls2/util/numerical_moving_window.hpp"
#include "dls2/util/time/duration_utils.hpp"

namespace dls::profiler
{
using Clock = std::chrono::steady_clock;

static constexpr size_t DEFAULT_WINDOW_SIZE = 100;

struct DurationStats {
    double      last_ms  = 0.0;
    double      mean_ms  = 0.0;
    double      min_ms   = 0.0;
    double      max_ms   = 0.0;
    std::size_t count    = 0;

    friend std::ostream& operator<<(std::ostream& os, const DurationStats& duration_stats)
    {
        os << "last_ms: " << duration_stats.last_ms << ", ";
        os << "mean_ms: " << duration_stats.mean_ms << ", ";
        os << "min_ms: " << duration_stats.min_ms << ", ";
        os << "max_ms: " << duration_stats.max_ms << ", ";
        os << "count: " << duration_stats.count << ";\n";

        return os;
    }
};

inline std::unordered_map<std::string, Clock::time_point>            g_starts;
inline std::unordered_map<std::string, NumericalMovingWindow<double>> g_windows;
inline std::unordered_map<std::string, double>                        g_last_values;

inline void tic(const std::string& id)
{
    g_starts[id] = Clock::now();
}

inline void toc(const std::string& id)
{
    auto now = Clock::now();

    auto it = g_starts.find(id);

    if (it == g_starts.end()) {
        std::stringstream err_msg;
        err_msg << "Unmatched tic for id " << id << "\n";
        throw std::runtime_error(err_msg.str());
    }

    const double ms = toMs<double>(now - it->second);
    g_starts.erase(it);

    auto win_it = g_windows.find(id);
    if (win_it == g_windows.end()) {
        win_it = g_windows.emplace(id, NumericalMovingWindow<double>(DEFAULT_WINDOW_SIZE)).first;
    }

    win_it->second.push(ms);
    g_last_values[id] = ms;
}

inline DurationStats getStats(const std::string& id)
{
    DurationStats out;

    auto w_it = g_windows.find(id);
    if (w_it == g_windows.end()) {
        return out;
    }

    const auto& win = w_it->second;
    const std::size_t n = win.size();
    if (n == 0) {
        return out;
    }

    out.count   = n;
    out.mean_ms = win.mean();
    out.min_ms  = win.min();
    out.max_ms  = win.max();

    auto last_it = g_last_values.find(id);
    if (last_it != g_last_values.end()) {
        out.last_ms = last_it->second;
    }

    return out;
}

} // namespace dls::profiler

#define dls_PROF_TIC(id)     ::dls::profiler::tic(id)
#define dls_PROF_TOC(id)     ::dls::profiler::toc(id)
#define dls_PROF_TIC_FUNC()  ::dls::profiler::tic(__func__)
#define dls_PROF_TOC_FUNC()  ::dls::profiler::toc(__func__)

#define dls_PROF_DURATION(id)  ::dls::profiler::getStats(id).last_ms
#define dls_PROF_STATS(id)  ::dls::profiler::getStats(id)


