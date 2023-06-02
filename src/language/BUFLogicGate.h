#ifndef VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class BUFLogicGate : public LogicGate
    {
        struct FirstOperation
        {
            constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                               VFPGAObfuscatorLibrary::Bit firstBit) const;
        };

        struct Operation
        {
            constexpr void run(VFPGAObfuscatorLibrary::Bit&,
                               VFPGAObfuscatorLibrary::Bit) const;
        };

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
 : LogicGate { CreateStandardTruthTable({ inputPort },
                                        outputPorts,
                                        FirstOperation {},
                                        Operation {}) }
{
}

constexpr void VFPGAObfuscatorLanguage::BUFLogicGate::FirstOperation::run(
  VFPGAObfuscatorLibrary::Bit& finalState,
  VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState = firstBit;
}

constexpr void VFPGAObfuscatorLanguage::BUFLogicGate::Operation::run(
  VFPGAObfuscatorLibrary::Bit&,
  VFPGAObfuscatorLibrary::Bit) const
{
}

#endif
