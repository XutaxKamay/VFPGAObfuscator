#ifndef VFPGA_TIMER_H
#define VFPGA_TIMER_H

#include "StandardHeaders.h"

namespace VFPGAObfuscatorSimulator
{
    class Timer
    {
      public:
        std::uint_fast16_t Nanos() const;
        std::uint_fast16_t Micros() const;
        std::uint_fast16_t Millis() const;
        std::uint_fast16_t Seconds() const;
        std::int_fast64_t Difference() const;

      public:
        void Start();
        void End();

      private:
        std::chrono::high_resolution_clock::time_point _start;
        std::chrono::high_resolution_clock::time_point _end;
        std::int_fast64_t _difference;
    };
}

#endif
