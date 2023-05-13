#ifndef FPGA_TIMER_H
#define FPGA_TIMER_H

#include "StandardHeaders.h"

namespace FPGAObfuscatorSimulator
{
    class Timer
    {
      public:
        std::uint16_t Nanos() const;
        std::uint16_t Micros() const;
        std::uint16_t Millis() const;
        std::uint16_t Seconds() const;
        std::int64_t Difference() const;

      public:
        void Start();
        void End();

      private:
        std::chrono::high_resolution_clock::time_point _start;
        std::chrono::high_resolution_clock::time_point _end;
        std::int64_t _difference;
    };
}

#endif
