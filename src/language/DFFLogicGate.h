#ifndef VFPGA_OBFUSCATOR_LANGUAGE_DFF_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_DFF_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class DFFLogicGate : public LogicGate
    {
      public:
        //////////////////////////////////////
        /// First input port: Enable set   ///
        /// Second input port: Data to set ///
        //////////////////////////////////////
        constexpr DFFLogicGate(
          const std::array<VFPGAObfuscatorLibrary::EncodedIndex, 2>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::DFFLogicGate::DFFLogicGate(
  const std::array<VFPGAObfuscatorLibrary::EncodedIndex, 2>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
{
    using namespace VFPGAObfuscatorLibrary;

    input_ports  = { inputPorts.begin(), inputPorts.end() };
    output_ports = outputPorts;

    input_truth_table = {
        {1_vfpga_obf_lib_bit, 0_vfpga_obf_lib_bit},
        {1_vfpga_obf_lib_bit, 1_vfpga_obf_lib_bit}
    };

    std::ranges::for_each(outputPorts,
                          [&](const EncodedIndex&)
                          {
                              std::vector<ElementType> elements = {
                                  0_vfpga_obf_lib_bit,
                                  1_vfpga_obf_lib_bit
                              };

                              output_truth_table.push_back(elements);
                          });
}

#endif
