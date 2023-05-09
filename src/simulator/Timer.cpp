#include "Timer.h"

using namespace FPGASimulator;

std::uint16_t Timer::Nanos() const
{
    return static_cast<std::uint16_t>(_difference % 1000);
}

std::uint16_t Timer::Micros() const
{
    return static_cast<std::uint16_t>((_difference / 1000) % 1000);
}

std::uint16_t Timer::Millis() const
{
    return static_cast<std::uint16_t>((_difference / 1000000) % 1000);
}

std::uint16_t Timer::Seconds() const
{
    return static_cast<std::uint16_t>((_difference / 1000000000));
}

std::int64_t Timer::Difference() const
{
    return _difference;
}

auto Timer::Start() -> void
{
    _start      = std::chrono::high_resolution_clock::now();
    _difference = 0;
}

void Timer::End()
{
    _end        = std::chrono::high_resolution_clock::now();
    _difference = (_end - _start).count();
}
