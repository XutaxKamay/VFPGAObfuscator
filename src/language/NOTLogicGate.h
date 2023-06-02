#ifndef VFPGA_OBFUSCATNOT_LANGUAGE_NOT_LOGIC_GATE_H
#define VFPGA_OBFUSCATNOT_LANGUAGE_NOT_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NOTLogicGate : public LogicGate
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
        constexpr NOTLogicGate(
          VFPGAObfuscatorLibrary::EncodedIndex inputPort,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::NOTLogicGate::NOTLogicGate(
  VFPGAObfuscatorLibrary::EncodedIndex inputPort,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
 : LogicGate { CreateStandardTruthTable({ inputPort },
                                        { outputPorts },
                                        FirstOperation {},
                                        Operation {}) }
{
}

constexpr void VFPGAObfuscatorLanguage::NOTLogicGate::FirstOperation::run(
  VFPGAObfuscatorLibrary::Bit& finalState,
  VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState.Set(~firstBit);
}

constexpr void VFPGAObfuscatorLanguage::NOTLogicGate::Operation::run(
  VFPGAObfuscatorLibrary::Bit&,
  VFPGAObfuscatorLibrary::Bit) const
{
}

#endif
