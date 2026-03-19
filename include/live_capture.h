#ifndef LIVE_CAPTURE_H
#define LIVE_CAPTURE_H

#include <string>
#include <functional>
#include "types.h"

namespace DPI {

using PacketCallback = std::function<void(PacketJob&)>;

class LiveCapture {
public:
    LiveCapture(const std::string& interface_name);

    bool start(PacketCallback callback);
    void stop();

private:
    std::string interface_;
    bool running_ = false;
};

}

#endif