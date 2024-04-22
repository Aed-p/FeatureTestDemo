#pragma once
#include <chrono>
#include <iostream>
#include <memory>

namespace TestDemo
{
/// @brief  not thread safe
class WatchHelper
{
public:
    WatchHelper() = default;
    ~WatchHelper()
    {
        stop();
    }

    void startWatch()
    {
        if (!is_start)
        {
            start = std::make_unique<std::chrono::steady_clock::time_point>(std::chrono::steady_clock::now());
            is_start = true;
        }
    }

    void watch()
    {
        auto now = std::chrono::steady_clock::now();
        std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(now - *start);
        std::cout << "Time elasp: " << float(duration.count()) / 1000 << " milliseconds." << std::endl;
    }

    void stop()
    {
        if (!is_start || !start)
            return;
        
        watch();

        start = nullptr;
        is_start = false;
    }

    using TPPtr = std::unique_ptr<std::chrono::steady_clock::time_point>;

private:
    TPPtr start;
    bool is_start = false;
};

}