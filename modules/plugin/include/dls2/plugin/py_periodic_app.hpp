#ifndef PY_PERIODIC_APP_HPP
#define PY_PERIODIC_APP_HPP

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

#include <pybind11/pybind11.h>

#include "dls2/application/periodic_app.hpp"

namespace py = pybind11;

namespace dls
{
class PyPeriodicApp : public dls::PeriodicApp
{
public:
    explicit PyPeriodicApp(const std::string& id);
    ~PyPeriodicApp() override;

    void start();
    void activate();
    void deactivate();
    void shutdown();
    void serve();

    bool isStarted() const;
    std::string stateName();

    bool checkActivation() override;
    void run(const std::chrono::system_clock::time_point& time) final;

protected:
    virtual bool pythonCheckActivation();
    virtual void pythonRun();

    std::thread app_thread_;
    std::atomic<bool> started_{false};
    std::atomic<bool> shutdown_requested_{false};
};
} // namespace dls

#endif // PY_PERIODIC_APP_HPP
