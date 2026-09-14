#pragma once

#include <atomic>
#include <cerrno>
#include <csignal>
#include <fcntl.h>
#include <functional>
#include <poll.h>
#include <stdexcept>
#include <system_error>
#include <thread>
#include <unistd.h>

namespace dls::utils
{
// One instance per executable. Notifications received before start() remain queued.
// The handler only writes to a nonblocking pipe; callbacks run on a normal thread.
class ShutdownSignal
{
public:
    ShutdownSignal()
    {
        if (::pipe2(pipe_, O_NONBLOCK | O_CLOEXEC) != 0)
            throw std::system_error(errno, std::generic_category(), "shutdown pipe");
        int expected = -1;
        if (!write_fd_.compare_exchange_strong(expected, pipe_[1])) {
            ::close(pipe_[0]);
            ::close(pipe_[1]);
            throw std::logic_error("ShutdownSignal already installed");
        }
        struct sigaction action{};
        action.sa_handler = &handle;
        ::sigemptyset(&action.sa_mask);
        action.sa_flags = SA_RESTART;
        if (::sigaction(SIGINT, &action, &old_int_) != 0) {
            const int error = errno;
            release();
            throw std::system_error(error, std::generic_category(), "SIGINT handler");
        }
        if (::sigaction(SIGTERM, &action, &old_term_) != 0) {
            const int error = errno;
            ::sigaction(SIGINT, &old_int_, nullptr);
            release();
            throw std::system_error(error, std::generic_category(), "SIGTERM handler");
        }
    }

    ShutdownSignal(const ShutdownSignal&) = delete;
    ShutdownSignal& operator=(const ShutdownSignal&) = delete;

    ~ShutdownSignal()
    {
        stopping_.store(true);
        if (worker_.joinable()) worker_.join();
        write_fd_.store(-1);
        ::sigaction(SIGINT, &old_int_, nullptr);
        ::sigaction(SIGTERM, &old_term_, nullptr);
        release();
    }

    // Return false while initialization cannot yet accept the request.
    void start(std::function<bool()> request_shutdown)
    {
        if (worker_.joinable()) throw std::logic_error("ShutdownSignal already started");
        worker_ = std::thread([this, callback = std::move(request_shutdown)] {
            pollfd input{pipe_[0], POLLIN, 0};
            bool pending = false;
            while (!stopping_.load()) {
                if (::poll(&input, 1, 50) > 0) {
                    char signals[64];
                    if (::read(pipe_[0], signals, sizeof(signals)) > 0) pending = true;
                }
                if (pending && !stopping_.load() && callback()) return;
            }
        });
    }

private:
    static_assert(std::atomic<int>::is_always_lock_free);
    static_assert(std::atomic<unsigned>::is_always_lock_free);
    inline static std::atomic<int> write_fd_{-1};
    inline static std::atomic<unsigned> handlers_{0};

    static void handle(int) noexcept
    {
        const int saved_errno = errno;
        handlers_.fetch_add(1);
        const int fd = write_fd_.load();
        if (fd >= 0) {
            const char byte = 1;
            ssize_t result;
            do { result = ::write(fd, &byte, 1); } while (result < 0 && errno == EINTR);
            // EAGAIN means an earlier notification is already pending.
        }
        handlers_.fetch_sub(1);
        errno = saved_errno;
    }

    void release() noexcept
    {
        write_fd_.store(-1);
        while (handlers_.load() != 0) std::this_thread::yield();
        ::close(pipe_[0]);
        ::close(pipe_[1]);
    }

    int pipe_[2];
    struct sigaction old_int_{};
    struct sigaction old_term_{};
    std::atomic_bool stopping_{false};
    std::thread worker_;
};
}
