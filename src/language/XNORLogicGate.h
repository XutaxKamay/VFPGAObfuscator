#ifndef VFPGA_OBFUSCATOR_LANGUAGE_XNOR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_XNOR_LOGIC_GATE_H

#include "XORLogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class XNORLogicGate : public XORLogicGate
    {
      public:
        constexpr XNORLogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::XNORLogicGate::XNORLogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
 : XORLogicGate { inputPorts, outputPorts }
{
    ///////////////////////////////
    /// Reverse all output bits ///
    ///////////////////////////////
    std::ranges::for_each(
      output_truth_table,
      [&](std::vector<ElementType>& elements)
      {
          std::ranges::for_each(
            elements,
            [&](ElementType& state)
            {
                std::get<VFPGAObfuscatorLibrary::Bit>(state) = ~std::get<
                  VFPGAObfuscatorLibrary::Bit>(state);
            });
      });
}

#endif
