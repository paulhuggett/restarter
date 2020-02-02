#ifndef RESTARTER_HPP
#define RESTARTER_HPP

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <stdexcept>

#include "trace.hpp"

class restarter {
public:
    template <typename Function, typename... Args>
    void run (Function func, Args &&... args);

    void stop ();

private:
    std::condition_variable cv_;
    std::mutex mut_;
    static constexpr auto max_retries_ = 5U;
    std::atomic<bool> done_{false};
};

template <typename Function, typename... Args>
void restarter::run (Function func, Args &&... args) {
    trace ("restarter");

    std::unique_lock<decltype (mut_)> lock{mut_};
    for (auto count = 0U; count < max_retries_ && !done_; ++count) {
        try {
            trace ("thread try ", count);
            func (std::forward<Args> (args)...);
            done_ = true;
            trace ("completed");
            break;
        } catch (std::exception const & ex) {
            trace ("exception:", ex.what ());
        } catch (...) {
            trace ("unknown exception");
        }

        if (!done_) {
            trace ("delay ", count, "s before retry");

            auto const wake_time =
                std::chrono::system_clock::now () + std::chrono::seconds (count);
            cv_.wait_until (lock, wake_time, [&] () {
                return done_ || std::chrono::system_clock::now () >= wake_time;
            });
        }
    }
    trace ("done");
}

#endif // RESTARTER_HPP
