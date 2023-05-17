#ifndef VFPGA_OBFUSCATOR_LANGUAGE_XOR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_XOR_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class XORLogicGate : public LogicGate
    {
      public:
        constexpr XORLogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::XORLogicGate::XORLogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
{
    FillStandardTruthTable(
      inputPorts,
      outputPorts,
      [](VFPGAObfuscatorLibrary::Bit& finalState,
         VFPGAObfuscatorLibrary::Bit nextBit)
      {
          finalState ^= nextBit;
      },
      [](VFPGAObfuscatorLibrary::Bit& finalState,
         VFPGAObfuscatorLibrary::Bit firstBit)
      {
          finalState = firstBit;
      });
}

#endif
