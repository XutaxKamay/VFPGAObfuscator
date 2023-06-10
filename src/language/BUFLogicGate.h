#ifndef VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_BUF_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class BUFLogicGate : public LogicGate
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
        constexpr BUFLogicGate(const Port& inputPort,
                               const std::vector<Port>& outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::BUFLogicGate::BUFLogicGate(
  const Port& inputPort,
  const std::vector<Port>& outputPorts)
 : LogicGate {
       CreateStandardTruthTable<FirstOperation, Operation>({ inputPort },
                                                           outputPorts)
   }
{
}

constexpr void VFPGAObfuscatorLanguage::BUFLogicGate::FirstOperation::
  operator()(VFPGAObfuscatorLibrary::Bit& finalState,
             VFPGAObfuscatorLibrary::Bit firstBit) const
{
    finalState = firstBit;
}

constexpr void VFPGAObfuscatorLanguage::BUFLogicGate::Operation::operator()(
  VFPGAObfuscatorLibrary::Bit&,
  VFPGAObfuscatorLibrary::Bit) const
{
}

#endif
