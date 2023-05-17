#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NOTLogicGate : public LogicGate
    {
      public:
        constexpr NOTLogicGate(
          VFPGAObfuscatorLibrary::EncodedIndex inputPort,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };

};

constexpr VFPGAObfuscatorLanguage::NOTLogicGate::NOTLogicGate(
  VFPGAObfuscatorLibrary::EncodedIndex inputPort,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
{
    FillStandardTruthTable({ inputPort },
                           outputPorts,
                           nullptr,
                           [](VFPGAObfuscatorLibrary::Bit& finalState,
                              VFPGAObfuscatorLibrary::Bit firstBit)
                           {
                               finalState = ~firstBit;
                           });
}

#endif
