#ifndef FPGA_SIMULATOR_PORT_H
#define FPGA_SIMULATOR_PORT_H

#include "StandardHeaders.h"

namespace FPGASimulator
{
    using Bit = std::bitset<1>;

    class Port
    {
      public:
        static inline constexpr Bit LOW  = 0;
        static inline constexpr Bit HIGH = 1;

      public:
        Bit state;

      public:
        void SetLow();
        void SetHigh();
    };
}

#endif
