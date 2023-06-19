#ifndef VFPGA_OBFUSCATOR_LANGUAGE_ADD_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_ADD_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{

    class ADDLogicGate : public LogicGate
    {
      public:
        template <std::size_t INTEGER_BIT_SIZE_T>
        using Integer = std::array<Port, INTEGER_BIT_SIZE_T>;

        template <std::size_t INTEGER_BIT_SIZE_T>
        requires (INTEGER_BIT_SIZE_T >= 1)
        constexpr ADDLogicGate(
          const Integer<INTEGER_BIT_SIZE_T>& inputIntegers,
          const std::vector<Integer<INTEGER_BIT_SIZE_T>>& outputInteger,
          const Port& signedFlagIn,
          const Port& carryFlagOut,
          const Port& overflowFlagOut,
          const Port& zeroFlagOut);
    };
};

template <std::size_t INTEGER_BIT_SIZE_T>
requires (INTEGER_BIT_SIZE_T >= 1)
constexpr VFPGAObfuscatorLanguage::ADDLogicGate::ADDLogicGate(
  const Integer<INTEGER_BIT_SIZE_T>& inputIntegers,
  const std::vector<Integer<INTEGER_BIT_SIZE_T>>& outputInteger,
  const Port& signedFlagIn,
  const Port& carryFlagOut,
  const Port& overflowFlagOut,
  const Port& zeroFlagOut)
{
    input_ports.push_back(signedFlagIn);

    std::ranges::for_each(
      inputIntegers,
      [&](const Integer<INTEGER_BIT_SIZE_T>& inputInteger) noexcept
      {
          input_ports.insert(input_ports.end(),
                             inputInteger.begin(),
                             inputInteger.end());
      });

    output_ports = { outputInteger.begin(), outputInteger.end() };
    output_ports.push_back(carryFlagOut);
    output_ports.push_back(overflowFlagOut);
    output_ports.push_back(zeroFlagOut);

    ///////////////////////////
    /// Integer + sign flag ///
    ///////////////////////////
    input_truth_table = CreateStandardInputTruthTable(
      INTEGER_BIT_SIZE_T * inputIntegers.size());

    VFPGAObfuscatorLibrary::Bit carryBit {}, overflowBit {}, zeroBit {};

    for (std::size_t lineIndex = 0; lineIndex < input_truth_table.size();
         lineIndex++)
    {
        std::vector<ElementType> integerResult;


        integerResult.push_back(carryBit);
        integerResult.push_back(overflowBit);
        integerResult.push_back(zeroBit);

        /////////////////////////////////////////////
        /// Finally push the whole integer result ///
        /////////////////////////////////////////////
        output_truth_table.push_back(integerResult);
    }
}

#endif
