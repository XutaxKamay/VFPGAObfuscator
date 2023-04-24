#ifndef FPGA_OBFUSCATION_PORT_H
#define FPGA_OBFUSCATION_PORT_H

#include "StandardHeaders.h"

using Bit = std::bitset<1>;

class Port
{
  public:
    static inline constexpr Bit LOW  = 0;
    static inline constexpr Bit HIGH = 1;

  protected:
    Bit _state;

  public:
    Port(Bit&& state);

    void setHigh();
    void setLow();

    Bit& state();
};

#endif
