#ifndef FPGA_SIMULATOR_PORT_H
#define FPGA_SIMULATOR_PORT_H

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
    Port() = default;
    Port(Bit&& state);

    Bit GetState();
    void SetLow();
    void SetHigh();
    void SetState(Bit state);
};

#endif
