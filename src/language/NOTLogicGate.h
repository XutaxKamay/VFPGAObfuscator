#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NOTLogicGate : public LogicGate
    {
      public:
        constexpr NOTLogicGate(
          EncodedIndex inputPort,
          const std::vector<EncodedIndex>& outputPorts);
    };

};

constexpr VFPGAObfuscatorLanguage::NOTLogicGate::NOTLogicGate(
  EncodedIndex inputPort,
  const std::vector<EncodedIndex>& outputPorts)
{
    FillStandardTruthTable({ inputPort },
                           outputPorts,
                           nullptr,
                           [](Bit& finalState, Bit firstBit)
                           {
                               finalState = ~firstBit;
                           });
}

#endif
