#ifndef DLS2_UTILS_HPP
#define DLS2_UTILS_HPP

#include <chrono>
#include <functional>
#include <thread>

namespace dls
{
	namespace utils{
        
        bool wait(std::function<bool()> func, int timeout_ms, int period_ms, bool& stop_wait);

    }// namespace utils

} // namespace dls

#endif /* end of include guard: DLS2_UTILS_HPP */
