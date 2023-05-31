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
    struct
    {
        constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                           VFPGAObfuscatorLibrary::Bit nextBit) const
        {
            finalState ^= nextBit;
        }
    } operation;

    struct
    {
        constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                           VFPGAObfuscatorLibrary::Bit nextBit) const
        {
            finalState = nextBit;
        }
    } firstOperation;

    FillStandardTruthTable({ inputPort },
                           outputPorts,
                           firstOperation,
                           operation);
}

#endif
