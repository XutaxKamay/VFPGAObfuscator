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
        constexpr NOTLogicGate(const Port& inputPort,
                               const std::vector<Port>& outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::NOTLogicGate::NOTLogicGate(
  const Port& inputPort,
  const std::vector<Port>& outputPorts)
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
    finalState = ~firstBit;
}

constexpr void VFPGAObfuscatorLanguage::NOTLogicGate::Operation::operator()(
  VFPGAObfuscatorLibrary::Bit&,
  VFPGAObfuscatorLibrary::Bit) const
{
}

#endif
