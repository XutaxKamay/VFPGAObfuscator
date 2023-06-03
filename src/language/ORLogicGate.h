#ifndef VFPGA_OBFUSCATOR_LANGUAGE_OR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_OR_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class ORLogicGate : public LogicGate
    {
        struct FirstOperation
        {
            constexpr void operator()(
              VFPGAObfuscatorLibrary::Bit& finalState,
              VFPGAObfuscatorLibrary::Bit firstBit) const;
        };

        struct Operation
        {
            constexpr void operator()(
              VFPGAObfuscatorLibrary::Bit& finalState,
              VFPGAObfuscatorLibrary::Bit nextBit) const;
        };

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
 : LogicGate {
       CreateStandardTruthTable<FirstOperation, Operation>(inputPorts,
                                                           outputPorts)
   }
{
}

constexpr void VFPGAObfuscatorLanguage::ORLogicGate::FirstOperation::
  operator()(VFPGAObfuscatorLibrary::Bit& finalState,
             VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState = firstBit;
}

constexpr void VFPGAObfuscatorLanguage::ORLogicGate::Operation::operator()(
  VFPGAObfuscatorLibrary::Bit& finalState,
  VFPGAObfuscatorLibrary::Bit nextBit) const
{
    finalState |= nextBit;
}

#endif
