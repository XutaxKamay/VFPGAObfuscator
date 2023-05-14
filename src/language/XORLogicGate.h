#ifndef VFPGA_OBFUSCATOR_LANGUAGE_XOR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_XOR_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class XORLogicGate : public LogicGate
    {
      public:
        constexpr XORLogicGate(
          const std::vector<EncodedIndex>& inputPorts,
          const std::vector<EncodedIndex>& outputPorts)
        {
            FillStandardTruthTable(
              inputPorts,
              outputPorts,
              [](Bit& finalState, Bit nextBit)
              {
                  finalState ^= nextBit;
              },
              [](Bit& finalState, Bit firstBit)
              {
                  finalState = firstBit;
              });
        }
    };

};

#endif
