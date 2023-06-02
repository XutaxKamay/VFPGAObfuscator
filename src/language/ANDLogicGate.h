#ifndef VFPGA_OBFUSCATOR_LANGUAGE_AND_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_AND_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class ANDLogicGate : public LogicGate
    {
        struct FirstOperation
        {
            constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                               VFPGAObfuscatorLibrary::Bit firstBit) const;
        };

        struct Operation
        {
            constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                               VFPGAObfuscatorLibrary::Bit nextBit) const;
        };

      public:
        constexpr ANDLogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::ANDLogicGate::ANDLogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
 : LogicGate { CreateStandardTruthTable(inputPorts,
                                        outputPorts,
                                        FirstOperation {},
                                        Operation {}) }
{
}

constexpr void VFPGAObfuscatorLanguage::ANDLogicGate::FirstOperation::run(
  VFPGAObfuscatorLibrary::Bit& finalState,
  VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState = firstBit;
}

constexpr void VFPGAObfuscatorLanguage::ANDLogicGate::Operation::run(
  VFPGAObfuscatorLibrary::Bit& finalState,
  VFPGAObfuscatorLibrary::Bit nextBit) const
{
    finalState &= nextBit;
}

#endif
