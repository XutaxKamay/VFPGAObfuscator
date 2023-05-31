#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NOT_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NOTLogicGate : public LogicGate
    {
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
{
    struct
    {
        constexpr void run(VFPGAObfuscatorLibrary::Bit&,
                           VFPGAObfuscatorLibrary::Bit) const
        {
        }
    } operation;

    struct
    {
        constexpr void run(VFPGAObfuscatorLibrary::Bit& finalState,
                           VFPGAObfuscatorLibrary::Bit nextBit) const
        {
            finalState.Set(~nextBit);
        }
    } firstOperation;

    FillStandardTruthTable({ inputPort },
                           outputPorts,
                           firstOperation,
                           operation);
}

#endif
