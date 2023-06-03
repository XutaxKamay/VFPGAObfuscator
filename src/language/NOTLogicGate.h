#ifndef VFPGA_OBFUSCATNOT_LANGUAGE_NOT_LOGIC_GATE_H
#define VFPGA_OBFUSCATNOT_LANGUAGE_NOT_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NOTLogicGate : public LogicGate
    {
        struct FirstOperation
        {
            constexpr void operator()(
              VFPGAObfuscatorLibrary::Bit& finalState,
              VFPGAObfuscatorLibrary::Bit firstBit) const;
        };

        struct Operation
        {
            constexpr void operator()(VFPGAObfuscatorLibrary::Bit&,
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
 : LogicGate {
       CreateStandardTruthTable<FirstOperation, Operation>({ inputPort },
                                                           outputPorts)
   }
{
}

constexpr void VFPGAObfuscatorLanguage::NOTLogicGate::FirstOperation::
  operator()(VFPGAObfuscatorLibrary::Bit& finalState,
             VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState.Set(~firstBit);
}

constexpr void VFPGAObfuscatorLanguage::NOTLogicGate::Operation::operator()(
  VFPGAObfuscatorLibrary::Bit&,
  VFPGAObfuscatorLibrary::Bit) const
{
}

#endif
