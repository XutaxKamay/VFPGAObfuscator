#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NAND_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NAND_LOGIC_GATE_H

#include "ANDLogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NANDLogicGate : public ANDLogicGate
    {
      public:
        constexpr NANDLogicGate(const std::vector<Port>& inputPorts,
                                const std::vector<Port>& outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::NANDLogicGate::NANDLogicGate(
  const std::vector<Port>& inputPorts,
  const std::vector<Port>& outputPorts)
 : ANDLogicGate { inputPorts, outputPorts }
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
