#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NOR_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NOR_LOGIC_GATE_H

#include "ORLogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NORLogicGate : public ORLogicGate
    {
      public:
        constexpr NORLogicGate(const std::vector<Port>& inputPorts,
                               const std::vector<Port>& outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::NORLogicGate::NORLogicGate(
  const std::vector<Port>& inputPorts,
  const std::vector<Port>& outputPorts)
 : ORLogicGate { inputPorts, outputPorts }
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
