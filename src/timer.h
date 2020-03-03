#pragma once

#include <chrono>

class Timer
{
private:
    using milliseconds_t = std::chrono::duration<float, std::ratio<1, 1000>>;

    std::chrono::time_point<std::chrono::high_resolution_clock> beg;
    std::chrono::time_point<std::chrono::high_resolution_clock> sto;
    bool stopped{};
public:
    Timer() : beg(std::chrono::high_resolution_clock::now())
    {
    }

    void reset()
    {
        stopped = false;
        beg = std::chrono::high_resolution_clock::now();
    }

    void stop()
    {
        stopped = true;
        sto = std::chrono::high_resolution_clock::now();
    }

    float elapsed() const
    {
        auto duration = stopped ?
                        sto - beg :
                        std::chrono::high_resolution_clock::now() - beg;
        return std::chrono::duration_cast<milliseconds_t>(duration).count();
    }
};
