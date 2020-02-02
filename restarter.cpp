#include "restarter.hpp"

void restarter::stop () {
    trace ("stopping threads");
    done_ = true;
    cv_.notify_one ();
}
