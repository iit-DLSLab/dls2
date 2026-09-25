#include <boost/asio.hpp>

#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

class DeviceProbe : public std::enable_shared_from_this<DeviceProbe>
{
public:
    using tcp = boost::asio::ip::tcp;
    using Clock = std::chrono::steady_clock;

    DeviceProbe(
        boost::asio::io_context& io,
        std::string name,
        std::string ip,
        uint16_t port,
        std::chrono::milliseconds timeout = std::chrono::milliseconds(500))
        : io_(io),
          socket_(io),
          timeout_timer_(io),
          endpoint_(
              boost::asio::ip::make_address(ip),
              port),
          timeout_(timeout),
          name_(name)
    {}

    DeviceProbe(const DeviceProbe&) = delete;
    DeviceProbe& operator=(const DeviceProbe&) = delete;

    // Returns false if a previous check is still pending.
    bool check()
    {
        if (in_progress_)
            return false;

        in_progress_ = true;

        const std::uint64_t probe_id = ++probe_id_;

        boost::system::error_code ignored;
        socket_.close(ignored);

        timeout_timer_.expires_after(timeout_);

        auto weak_self = weak_from_this();

        timeout_timer_.async_wait(
            [weak_self, probe_id](const boost::system::error_code& ec)
            {
                auto self = weak_self.lock();
                if (!self)
                    return;

                // Timer was cancelled because connect completed.
                if (ec == boost::asio::error::operation_aborted)
                    return;

                // Ignore callback belonging to an obsolete probe.
                if (probe_id != self->probe_id_)
                    return;

                // Connect may already have completed while timer callback
                // was waiting in the event queue.
                if (!self->in_progress_)
                    return;

                self->alive_ = false;
                self->last_error_ =
                    boost::asio::error::timed_out;

                ++self->consecutive_failures_;

                self->in_progress_ = false;

                // Abort async_connect().
                boost::system::error_code ignored;
                self->socket_.cancel(ignored);
                self->socket_.close(ignored);
            });

        socket_.async_connect(
            endpoint_,
            [weak_self, probe_id](const boost::system::error_code& ec)
            {
                auto self = weak_self.lock();
                if (!self)
                    return;

                if (probe_id != self->probe_id_)
                    return;

                /*
                 * If the timeout handler already fired, it owns completion
                 * of this probe. async_connect() will normally arrive here
                 * with operation_aborted.
                 */
                if (!self->in_progress_)
                    return;

                self->in_progress_ = false;

                boost::system::error_code ignored;
                self->timeout_timer_.cancel(ignored);

                if (!ec)
                {
                    self->alive_ = true;
                    self->consecutive_failures_ = 0;
                    self->last_success_ = Clock::now();
                    self->last_error_.clear();
                }
                else
                {
                    self->alive_ = false;
                    ++self->consecutive_failures_;
                    self->last_error_ = ec;
                }

                self->socket_.close(ignored);
            });

        return true;
    }

    void cancel()
    {
        /*
         * Invalidates callbacks belonging to the current probe.
         */
        ++probe_id_;
        in_progress_ = false;

        boost::system::error_code ignored;

        timeout_timer_.cancel(ignored);
        socket_.cancel(ignored);
        socket_.close(ignored);
    }

    [[nodiscard]]
    bool alive() const noexcept
    {
        return alive_;
    }

    [[nodiscard]]
    bool checkInProgress() const noexcept
    {
        return in_progress_;
    }

    [[nodiscard]]
    unsigned consecutiveFailures() const noexcept
    {
        return consecutive_failures_;
    }

    [[nodiscard]]
    boost::system::error_code lastError() const
    {
        return last_error_;
    }

    [[nodiscard]]
    Clock::time_point lastSuccess() const noexcept
    {
        return last_success_;
    }

    [[nodiscard]]
    std::string name() const noexcept
    {
        return name_;
    }

private:
    boost::asio::io_context& io_;

    tcp::socket socket_;
    boost::asio::steady_timer timeout_timer_;
    tcp::endpoint endpoint_;

    std::chrono::milliseconds timeout_;

    bool in_progress_{false};
    bool alive_{false};

    unsigned consecutive_failures_{0};

    std::uint64_t probe_id_{0};

    std::string name_;
    Clock::time_point last_success_{};
    boost::system::error_code last_error_;
};