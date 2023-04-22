#ifndef LOGIC_GATE_H
#define LOGIC_GATE_H

#include "StandardHeaders.h"

template <std::size_t N>
using Bits = std::bitset<N>;

template <std::size_t INPUT_NUMBER_OF_BITS,
          std::size_t OUTPUT_NUMBER_OF_BITS>
class LogicGate
{
  public:
    using InputBits  = Bits<INPUT_NUMBER_OF_BITS>;
    using OutputBits = Bits<OUTPUT_NUMBER_OF_BITS>;

    virtual ~LogicGate()                         = 0;
    virtual void compute()                       = 0;
    virtual void setInputBits(OutputBits&& bits) = 0;
    virtual InputBits outputBits()               = 0;

    InputBits _input_bits;
    OutputBits _output_bits;
};

#endif
