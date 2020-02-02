#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>

#include "trace.hpp"
#include "restarter.hpp"

namespace {

    void foo (int state) {
        trace ("foo");
        std::cout << "foo state=" << state << '\n';
        throw std::runtime_error ("bad things happened");
    }

} // end anonymous namespace

int main () {
    try {
        restarter r;
        std::thread thread ([&r] () { r.run (foo, 5); });

        std::this_thread::sleep_for (std::chrono::seconds{2});
        r.stop ();

        trace ("join");
        thread.join ();
    } catch (std::exception const & ex) {
        std::cerr << "Error: " << ex.what () << std::endl;
    } catch (...) {
        std::cerr << "An unknown error occurred" << std::endl;
    }
}
