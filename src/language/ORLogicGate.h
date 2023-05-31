#ifndef VFPGA_OBFUSCATOR_LANGUAGE_OR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_OR_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class ORLogicGate : public LogicGate
    {
      public:
        constexpr ORLogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::ORLogicGate::ORLogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
{
    struct
    {
        constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                           VFPGAObfuscatorLibrary::Bit nextBit) const
        {
            finalState |= nextBit;
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

    FillStandardTruthTable(inputPorts,
                           outputPorts,
                           firstOperation,
                           operation);
}

#endif
