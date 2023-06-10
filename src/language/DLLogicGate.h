#ifndef VFPGA_OBFUSCATOR_LANGUAGE_DL_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_DL_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class DLLogicGate : public LogicGate
    {
      public:
        //////////////////////////////////////
        /// First input port: Enable set   ///
        /// Second input port: Data to set ///
        //////////////////////////////////////
        constexpr DLLogicGate(const std::array<Port, 2>& inputPorts,
                              const std::vector<Port>& outputPorts);
    };
};

constexpr VFPGAObfuscatorLanguage::DLLogicGate::DLLogicGate(
  const std::array<Port, 2>& inputPorts,
  const std::vector<Port>& outputPorts)
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
