#ifndef VFPGA_OBFUSCATOR_SIMULATOR_PORT_H
#define VFPGA_OBFUSCATOR_SIMULATOR_PORT_H

#include "Bit.h"
#include "SharedSerializedTypes.h"

namespace VFPGAObfuscatorSimulator
{
    using namespace VFPGAObfuscatorLibrary;

    class Port
    {
      public:
        static inline constinit Bit LOW  = 0_vfpga_bit;
        static inline constinit Bit HIGH = 1_vfpga_bit;

      public:
        Bit state;

      public:
        void SetLow();
        void SetHigh();
    };
}

#endif
