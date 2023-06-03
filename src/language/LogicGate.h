#ifndef VFPGA_OBFUSCATOR_LANGUAGE_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_LOGIC_GATE_H

#include "Error.h"
#include "simulator/LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class LogicGate :
     public VFPGAObfuscatorSimulator::LogicGate::Serializer
    {
      public:
        constexpr LogicGate() = default;

        constexpr LogicGate(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts);

        template <typename FO_T, typename O_T>
        static constexpr LogicGate CreateStandardTruthTable(
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            inputPorts,
          const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>&
            outputPorts,
          const FO_T& firstOperation,
          const O_T& operation);
    };
};

constexpr VFPGAObfuscatorLanguage::LogicGate::LogicGate(
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
  const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts)
 : Serializer { inputPorts, outputPorts, {}, {} }
{
}

template <typename FO_T, typename O_T>
constexpr VFPGAObfuscatorLanguage::LogicGate VFPGAObfuscatorLanguage::
  LogicGate::CreateStandardTruthTable(
    const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& inputPorts,
    const std::vector<VFPGAObfuscatorLibrary::EncodedIndex>& outputPorts,
    ///////////////////////////////////////////////////////
    /// There might be sometimes only one input port,   ///
    /// if there is, we should just let the user decide ///
    /// (if it's a NOT logic gate for example)          ///
    ///////////////////////////////////////////////////////
    const FO_T& firstOperation,
    const O_T& operation)
{
    LogicGate logicGate { inputPorts, outputPorts };

    std::vector<std::vector<VFPGAObfuscatorLibrary::Bit>> permutations {
        logicGate.input_ports.size() + 1,
        std::vector<VFPGAObfuscatorLibrary::Bit> {
                                                  logicGate.input_ports.size(),
                                                  VFPGAObfuscatorLibrary::Bit {}}
    };

    //////////////////////////////////////////////////////
    /// Make a triangle for all possible permutations. ///
    /// Example:                                       ///
    ///                                                ///
    /// A B C D E F => input ports                     ///
    /// 0 0 0 0 0 0                                    ///
    /// 1 0 0 0 0 0                                    ///
    /// 1 1 0 0 0 0                                    ///
    /// 1 1 1 0 0 0                                    ///
    /// 1 1 1 1 0 0                                    ///
    /// 1 1 1 1 1 0                                    ///
    /// 1 1 1 1 1 1                                    ///
    //////////////////////////////////////////////////////
    for (std::size_t i = 0; i < permutations.size(); i++)
    {
        for (std::size_t j = 0; j < logicGate.input_ports.size(); j++)
        {
            if (i > j)
            {
                permutations[i][j] = 1_vfpga_obf_lib_bit;
            }
            else
            {
                permutations[i][j] = 0_vfpga_obf_lib_bit;
            }
        }
    }

    ///////////////////////////////////////////////////
    /// Search for all states possible              ///
    /// by permuting the values inside the triangle ///
    /// per line.                                   ///
    /// Example for the one given above:            ///
    ///                                             ///
    /// 0 0 0 0 0 0                                 ///
    /// 1 0 0 0 0 0                                 ///
    /// 0 1 0 0 0 0                                 ///
    /// 0 0 1 0 0 0                                 ///
    /// 0 0 0 1 0 0                                 ///
    /// 0 0 0 0 1 0                                 ///
    /// 0 0 0 0 0 1                                 ///
    /// 1 1 0 0 0 0                                 ///
    /// 1 0 1 0 0 0                                 ///
    /// 1 0 0 1 0 0                                 ///
    /// 1 0 0 0 1 0                                 ///
    /// 1 0 0 0 0 1                                 ///
    /// 0 1 1 0 0 0                                 ///
    /// 0 1 0 1 0 0                                 ///
    /// 0 1 0 0 1 0                                 ///
    /// 0 1 0 0 0 1                                 ///
    /// 0 0 1 1 0 0                                 ///
    /// 0 0 1 0 1 0                                 ///
    /// 0 0 1 0 0 1                                 ///
    /// 0 0 0 1 1 0                                 ///
    /// 0 0 0 1 0 1                                 ///
    /// 0 0 0 0 1 1                                 ///
    /// 1 1 1 0 0 0                                 ///
    /// 1 1 0 1 0 0                                 ///
    /// 1 1 0 0 1 0                                 ///
    /// 1 1 0 0 0 1                                 ///
    /// 1 0 1 1 0 0                                 ///
    /// 1 0 1 0 1 0                                 ///
    /// 1 0 1 0 0 1                                 ///
    /// 1 0 0 1 1 0                                 ///
    /// 1 0 0 1 0 1                                 ///
    /// 1 0 0 0 1 1                                 ///
    /// 0 1 1 1 0 0                                 ///
    /// 0 1 1 0 1 0                                 ///
    /// 0 1 1 0 0 1                                 ///
    /// 0 1 0 1 1 0                                 ///
    /// 0 1 0 1 0 1                                 ///
    /// 0 1 0 0 1 1                                 ///
    /// 0 0 1 1 1 0                                 ///
    /// 0 0 1 1 0 1                                 ///
    /// 0 0 1 0 1 1                                 ///
    /// 0 0 0 1 1 1                                 ///
    /// 1 1 1 1 0 0                                 ///
    /// 1 1 1 0 1 0                                 ///
    /// 1 1 1 0 0 1                                 ///
    /// 1 1 0 1 1 0                                 ///
    /// 1 1 0 1 0 1                                 ///
    /// 1 1 0 0 1 1                                 ///
    /// 1 0 1 1 1 0                                 ///
    /// 1 0 1 1 0 1                                 ///
    /// 1 0 1 0 1 1                                 ///
    /// 1 0 0 1 1 1                                 ///
    /// 0 1 1 1 1 0                                 ///
    /// 0 1 1 1 0 1                                 ///
    /// 0 1 1 0 1 1                                 ///
    /// 0 1 0 1 1 1                                 ///
    /// 0 0 1 1 1 1                                 ///
    /// 1 1 1 1 1 0                                 ///
    /// 1 1 1 1 0 1                                 ///
    /// 1 1 1 0 1 1                                 ///
    /// 1 1 0 1 1 1                                 ///
    /// 1 0 1 1 1 1                                 ///
    /// 0 1 1 1 1 1                                 ///
    /// 1 1 1 1 1 1                                 ///
    ///////////////////////////////////////////////////
    std::ranges::for_each(
      permutations,
      [&](std::vector<VFPGAObfuscatorLibrary::Bit>& permutation)
      {
          do
          {
              std::vector<ElementType> elements;

              std::ranges::transform(
                permutation,
                std::back_inserter(elements),
                [&](VFPGAObfuscatorLibrary::Bit bit) noexcept
                {
                    return bit;
                });

              logicGate.input_truth_table.push_back(elements);
          }
          while (std::ranges::next_permutation(permutation,
                                               std::ranges::greater())
                   .found);
      });

    ////////////////////////////////////////////////////
    /// For each lines,                              ///
    /// check the ones that verifies the truth table ///
    /// and apply the operation()                    ///
    /// and push back the final state to the         ///
    /// output ports.                                ///
    /// We must also check if there's atleast 1      ///
    /// input state, with zero states                ///
    /// it might an error made by the user           ///
    ////////////////////////////////////////////////////
    std::ranges::for_each(
      logicGate.input_truth_table,
      [&](const std::vector<ElementType>& elements)
      {
          ////////////////////////////////////////////////
          /// Check if there's at least one input port ///
          ////////////////////////////////////////////////
          if (elements.size() >= 1)
          {
              VFPGAObfuscatorLibrary::Bit finalState;
              firstOperation(finalState, std::get<1>(elements[0]));

              for (std::size_t i = 1; i < elements.size(); i++)
              {
                  operation(finalState, std::get<1>(elements[i]));
              }

              logicGate.output_truth_table.push_back(
                std::vector<ElementType>(logicGate.output_ports.size(),
                                         finalState));
          }
      });

    return logicGate;
}

#endif
