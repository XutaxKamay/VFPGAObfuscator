#ifndef VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class BUFLogicGate : public LogicGate
    {
      public:
        constexpr BUFLogicGate(
          VFPGAObfuscatorLibrary::EncodedIndex inputPort,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };

};

constexpr VFPGAObfuscatorLanguage::BUFLogicGate::BUFLogicGate(
  VFPGAObfuscatorLibrary::EncodedIndex inputPort,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
{
    FillStandardTruthTable({ inputPort },
                           outputPorts,
                           nullptr,
                           [](VFPGAObfuscatorLibrary::Bit& finalState,
                              VFPGAObfuscatorLibrary::Bit firstBit)
                           {
                               finalState = firstBit;
                           });
}

#endif
