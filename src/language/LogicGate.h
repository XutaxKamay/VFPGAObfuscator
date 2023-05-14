#ifndef VFPGA_OBFUSCATOR_LANGUAGE_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_LOGIC_GATE_H

#include "simulator/LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    using Bit = VFPGAObfuscatorSimulator::Bit;

    class LogicGate :
     public VFPGAObfuscatorSimulator::LogicGate::Serializer
    {
      protected:
        constexpr void FillStandardTruthTable(
          const std::vector<EncodedIndex>& inputPorts,
          const std::vector<EncodedIndex>& outputPorts,
          const std::function<void(Bit& finalState, Bit nextBit)>&
            operation,
          const std::function<void(Bit& finalState, Bit firstBit)>&
            firstOperation);
    };
};

constexpr void VFPGAObfuscatorLanguage::LogicGate::FillStandardTruthTable(
  const std::vector<EncodedIndex>& inputPorts,
  const std::vector<EncodedIndex>& outputPorts,
  const std::function<void(Bit& finalState, Bit nextBit)>& operation,
  ///////////////////////////////////////////////////////
  /// There might be sometimes only one input port,   ///
  /// if there is, we should just let the user decide ///
  /// (if it's a NOT logic gate for example)          ///
  ///////////////////////////////////////////////////////
  const std::function<void(Bit& finalState, Bit firstBit)>& firstOperation)
{
    input_ports  = inputPorts;
    output_ports = outputPorts;

    std::vector<std::vector<Bit>> permutations(
      input_ports.size() + 1,
      std::vector<Bit>(input_ports.size()));

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
        for (std::size_t j = 0; j < input_ports.size(); j++)
        {
            if (i > j)
            {
                permutations[i][j] = 1;
            }
            else
            {
                permutations[i][j] = 0;
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
      [&](std::vector<Bit>& permutation)
      {
          do
          {
              std::vector<ElementType> elements;
              std::ranges::transform(permutation,
                                     std::back_inserter(elements),
                                     [](const Bit bit)
                                     {
                                         return bit;
                                     });

              input_truth_table.push_back(elements);
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
      input_truth_table,
      [&](const std::vector<ElementType>& elements)
      {
          ////////////////////////////////////////////////
          /// Check if there's at least one input port ///
          ////////////////////////////////////////////////
          if (elements.size() >= 1)
          {
              Bit finalState;
              firstOperation(finalState, std::get<1>(elements[0]));

              for (std::size_t i = 1; i < elements.size(); i++)
              {
                  operation(finalState, std::get<1>(elements[i]));
              }

              output_truth_table.push_back(
                std::vector<ElementType>(outputPorts.size(), finalState));
          }
          else
          {
              ///////////////////////////////////////////////
              /// This will be an error during simulation ///
              ///////////////////////////////////////////////
              output_truth_table.push_back({});
          }
      });
}

#endif
