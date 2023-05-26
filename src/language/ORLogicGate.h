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
    FillStandardTruthTable(
      inputPorts,
      outputPorts,
      [](VFPGAObfuscatorLibrary::Bit& finalState,
         VFPGAObfuscatorLibrary::Bit nextBit)
      {
          finalState |= nextBit;
      },
      [](VFPGAObfuscatorLibrary::Bit& finalState,
         VFPGAObfuscatorLibrary::Bit firstBit)
      {
          finalState = firstBit;
      });
}

#endif