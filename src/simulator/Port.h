#ifndef VFPGA_OBFUSCATOR_SIMULATOR_PORT_H
#define VFPGA_OBFUSCATOR_SIMULATOR_PORT_H

#include "Bit.h"

namespace VFPGAObfuscatorSimulator
{

    class Port
    {
      public:
        static inline constinit auto LOW  = 0_vfpga_obf_lib_bit;
        static inline constinit auto HIGH = 1_vfpga_obf_lib_bit;

      public:
        VFPGAObfuscatorLibrary::Bit state;

      public:
        constexpr void SetLow();
        constexpr void SetHigh();
    };
}

constexpr void VFPGAObfuscatorSimulator::Port::SetHigh()
{
    state = HIGH;
}

constexpr void VFPGAObfuscatorSimulator::Port::SetLow()
{
    state = LOW;
}

#endif
