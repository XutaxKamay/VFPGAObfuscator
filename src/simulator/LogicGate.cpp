#include "LogicGate.h"
#include "Deserializer.h"
#include "Error.h"
#include "Serializer.h"

using namespace FPGASimulator;

void LogicGate::Deserialized::Deserialize(
  const std::vector<std::byte>& serialized,
  const std::vector<Port*>& allPorts)
{
    Deserializer deserializer { serialized };
}

void LogicGate::Deserialized::RunLogicFunction()
{
    //////////////////////////////////////////////////////////////////
    /// Get the bit in the truth table,                            ///
    /// there is two types:                                        ///
    /// - A port                                                   ///
    /// - A bit                                                    ///
    /// If it's a port,                                            ///
    /// then we get its state otherwise we just get the bit value  ///
    //////////////////////////////////////////////////////////////////
    static const auto GetBitState =
      [&](const std::variant<Port*, Bit>& element)
    {
        Bit state;

        switch (element.index())
        {
            case 0:
            {
                ///////////////////
                /// It's a port ///
                ///////////////////
                state = std::get<0>(element)->state;
                break;
            }

            case 1:
            {
                //////////////////
                /// It's a bit ///
                //////////////////
                state = std::get<1>(element);
                break;
            }

            default:
            {
                ///////////////////////////
                /// Should never happen ///
                ///////////////////////////
                Error::ExitWithMsg(
                  Error::Msg::UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE);
            }
        }

        return state;
    };

    /////////////////////////////////////////////////////////////
    /// Verifies for each line if the truth table is verified ///
    /////////////////////////////////////////////////////////////
    for (std::size_t lineIndex = 0; lineIndex < input_truth_table.size();
         lineIndex++)
    {
        const auto& elementsOnLine = input_truth_table[lineIndex];
        bool verifiesTruthTable    = false;

        //////////////////////////////////////////////////////////////////
        /// We are selecting the element/value in the truth table and  ///
        /// check if the input is the same as it is on the truth table ///
        //////////////////////////////////////////////////////////////////
        for (std::size_t columnIndex = 0;
             columnIndex < elementsOnLine.size();
             columnIndex++)
        {
            const auto& element = elementsOnLine[columnIndex];
            const auto state    = GetBitState(element);

            if (input_ports[columnIndex]->state == state)
            {
                verifiesTruthTable = true;
                continue;
            }

            verifiesTruthTable = false;
            break;
        }

        ///////////////////////////////////////////////////////////////
        /// We found one match, apply all the outputs and exit loop ///
        ///////////////////////////////////////////////////////////////
        if (verifiesTruthTable)
        {
            for (std::size_t columnIndex = 0;
                 columnIndex < output_ports.size();
                 columnIndex++)
            {
                output_ports[columnIndex]->state = GetBitState(
                  output_truth_table[lineIndex][columnIndex]);
            }

            break;
        }
    }
}

std::vector<std::byte> LogicGate::Serializer::Serialize()
{
    ::Serializer serializer;

    static const auto AddPort = [&](const EncodedIndex& portIndex)
    {
        serializer.AddVar(portIndex);
    };

    static const auto AddElementType =
      [&](const std::variant<EncodedIndex, Bit>& element)
    {
        serializer.AddVar(static_cast<EncodedIndex>(element.index()));

        switch (element.index())
        {
            case 0:
            {
                serializer.AddVar(std::get<0>(element));
                break;
            }

            case 1:
            {
                serializer.AddVar(static_cast<std::uint8_t>(
                  std::get<1>(element).to_ullong()));
                break;
            }

            default:
            {
                Error::ExitWithMsg(
                  Error::Msg::UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE);
            }
        }
    };

    static const auto AddPorts =
      [&](const std::vector<EncodedIndex>& vector)
    {
        serializer.AddVar<EncodedIndex>(vector.size());
        std::ranges::for_each(vector, AddPort);
    };

    static const auto AddElements =
      [&](const std::vector<ElementType>& elements)
    {
        serializer.AddVar<EncodedIndex>(elements.size());
        std::ranges::for_each(elements, AddElementType);
    };

    static const auto AddTruthTable = [&](const TruthTable& truthTable)
    {
        serializer.AddVar<EncodedIndex>(truthTable.size());
        std::ranges::for_each(truthTable, AddElements);
    };

    AddPorts(input_ports);
    AddPorts(output_ports);

    AddTruthTable(input_truth_table);
    AddTruthTable(output_truth_table);

    return serializer.data;
}

void LogicGate::Simulate()
{
    deserialized.RunLogicFunction();
}

