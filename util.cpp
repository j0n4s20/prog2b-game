#include "util.h"
#include <chrono>
#include <thread>

void msleep(unsigned int milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}
