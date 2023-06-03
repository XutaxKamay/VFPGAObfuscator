#ifndef VFPGA_OBFUSCATOR_LANGUAGE_NAND_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_NAND_LOGIC_GATE_H

#include "ANDLogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class NANDLogicGate : public ANDLogicGate
    {
      public:
        constexpr NANDLogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::NANDLogicGate::NANDLogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
 : ANDLogicGate { inputPorts, outputPorts }
{
    ///////////////////////////////
    /// Reverse all output bits ///
    ///////////////////////////////
    std::ranges::for_each(output_truth_table,
                          [&](std::vector<ElementType>& elements)
                          {
                              std::ranges::for_each(
                                elements,
                                [&](ElementType& state)
                                {
                                    std::get<1>(state) = ~std::get<1>(
                                      state);
                                });
                          });
}

#endif
