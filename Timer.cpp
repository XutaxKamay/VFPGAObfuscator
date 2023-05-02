#include "Timer.h"

std::uint16_t Timer::Nanos() const
{
    return _difference % 1000;
}

std::uint16_t Timer::Micros() const
{
    return (_difference / 1000) % 1000;
}

std::uint16_t Timer::Millis() const
{
    return (_difference / 1000000) % 1000;
}

std::uint64_t Timer::Seconds() const
{
    return (_difference / 1000000000);
}

std::uint64_t Timer::Difference() const
{
    return _difference;
}

auto Timer::Start() -> void
{
    _start = std::chrono::high_resolution_clock::now();
}

void Timer::End()
{
    _end = std::chrono::high_resolution_clock::now();

    _difference = std::chrono::nanoseconds(_end - _start).count();
}
