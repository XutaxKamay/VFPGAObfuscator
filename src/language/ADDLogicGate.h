#ifndef VFPGA_OBFUSCATOR_LANGUAGE_ADD_LOGIC_GATE_H
#define VFPGA_OBFUSCATOR_LANGUAGE_ADD_LOGIC_GATE_H

#include "LogicGate.h"

namespace VFPGAObfuscatorLanguage
{
    class ADDLogicGate : public LogicGate
    {
      public:
        template <std::size_t INTEGER_BIT_SIZE_T>
        requires (INTEGER_BIT_SIZE_T >= 1)
        constexpr ADDLogicGate(
          const std::array<std::array<Port, INTEGER_BIT_SIZE_T>, 2>&
            inputIntegers,
          const std::array<Port, INTEGER_BIT_SIZE_T>& outputInteger,
          const Port& carryFlagBitPort,
          const Port& overflowFlagBitPort,
          const Port& signedFlagBitPort,
          const Port& zeroFlagBitPort);
    };
};

template <std::size_t INTEGER_BIT_SIZE_T>
requires (INTEGER_BIT_SIZE_T >= 1)
constexpr VFPGAObfuscatorLanguage::ADDLogicGate::ADDLogicGate(
  const std::array<std::array<Port, INTEGER_BIT_SIZE_T>, 2>& inputIntegers,
  const std::array<Port, INTEGER_BIT_SIZE_T>& outputInteger,
  const Port& carryFlagBitPort,
  const Port& overflowFlagBitPort,
  const Port& signedFlagBitPort,
  const Port& zeroFlagBitPort)
{
    std::ranges::for_each(inputIntegers,
                          [&](const std::array<Port, INTEGER_BIT_SIZE_T>&
                                inputInteger) noexcept
                          {
                              input_ports.insert(input_ports.end(),
                                                 inputInteger.begin(),
                                                 inputInteger.end());
                          });

    output_ports = { outputInteger.begin(), outputInteger.end() };
    output_ports.push_back(carryFlagBitPort);

    input_truth_table = CreateStandardInputTruthTable(
      INTEGER_BIT_SIZE_T * inputIntegers.size());

    VFPGAObfuscatorLibrary::Bit lastCarryBit {};

    for (std::size_t lineIndex = 0; lineIndex < input_truth_table.size();
         lineIndex++)
    {
        std::vector<ElementType> integerResult;

        for (std::size_t columnIndex = 0;
             columnIndex < INTEGER_BIT_SIZE_T;
             columnIndex++)
        {
            const auto firstIntegerBit = std::get<
              VFPGAObfuscatorLibrary::Bit>(
              input_truth_table[lineIndex][columnIndex]);

            const auto secondIntegerBit = std::get<
              VFPGAObfuscatorLibrary::Bit>(
              input_truth_table[lineIndex]
                               [INTEGER_BIT_SIZE_T + columnIndex]);

            const auto sumBit = firstIntegerBit ^ secondIntegerBit
                                ^ lastCarryBit;

            integerResult.push_back(sumBit);

            lastCarryBit = firstIntegerBit & secondIntegerBit;

            /////////////////////////////////////
            /// i1 + i2 + carry               ///
            ///                               ///
            /// bitIndex 1: 1 + 1 + x = 0 (1) ///
            /// bitIndex 2: 1 + 1 + 1 = 1 (1) ///
            /// bitIndex 3: 0 + 0 + 1 = 1 (0) ///
            /// ...                           ///
            /////////////////////////////////////
        }

        //////////////////////////////////////////////////
        /// As last element, we will put the carry bit ///
        //////////////////////////////////////////////////
        integerResult.push_back(lastCarryBit);

        /////////////////////////////////////////////
        /// Finally push the whole integer result ///
        /////////////////////////////////////////////
        output_truth_table.push_back(integerResult);
    }
}

#endif
