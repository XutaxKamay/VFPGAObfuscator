#ifndef FPGA_TIMER_H
#define FPGA_TIMER_H

#include "StandardHeaders.h"

class Timer
{
  public:
    std::uint16_t Nanos() const;
    std::uint16_t Micros() const;
    std::uint16_t Millis() const;
    std::uint64_t Seconds() const;
    std::uint64_t Difference() const;

  public:
    void Start();
    void End();

  private:
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _end;
    std::uint64_t _difference;
};

#endif