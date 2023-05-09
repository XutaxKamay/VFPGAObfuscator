#include "LogicGate.h"
#include "Deserializer.h"
#include "Error.h"
#include "FPGA.h"
#include "Serializer.h"

using namespace FPGASimulator;

LogicGate LogicGate::Deserializer::Deserialize(
  const std::vector<std::byte>& serialized,
  FPGA* const fpga) const
{
    LogicGate logicGate;
    ::Deserializer deserializer { serialized };

    const auto ReadPorts = [&](std::vector<Port*>& ports)
    {
        const auto numberOfPorts = deserializer
                                     .ReadAndCheckStatus<EncodedIndex>();

        for (EncodedIndex index = 0; index < numberOfPorts; index++)
        {
            const auto portIndex = deserializer
                                     .ReadAndCheckStatus<EncodedIndex>();

            if (portIndex >= fpga->NumberOfPorts())
            {
                Error::ExitWithMsg(
                  Error::Msg::LOGIC_GATE_DESERIALIZER_READ_FAILED);
            }

            ports.push_back(fpga->GetPort(portIndex));
        }
    };

    const auto ReadElement = [&]()
    {
        const auto elementIndex = deserializer
                                    .ReadAndCheckStatus<EncodedIndex>();

        ElementType element;

        switch (elementIndex)
        {
            case 0:
            {
                ///////////////////
                /// It's a port ///
                ///////////////////
                const auto portIndex = deserializer.ReadAndCheckStatus<
                  EncodedIndex>();

                if (portIndex >= fpga->NumberOfPorts())
                {
                    Error::ExitWithMsg(
                      Error::Msg::LOGIC_GATE_DESERIALIZER_READ_FAILED);
                }

                element = fpga->GetPort(portIndex);
                break;
            }

            case 1:
            {
                //////////////////
                /// It's a bit ///
                //////////////////
                element = deserializer.ReadAndCheckStatus<std::uint8_t>();
                break;
            }

            default:
            {
                ///////////////////////////
                /// Should never happen ///
                ///////////////////////////
                Error::ExitWithMsg(
                  Error::Msg::LOGIC_GATE_DESERIALIZER_READ_FAILED);
            }
        }

        return element;
    };

    const auto ReadElements = [&]()
    {
        std::vector<ElementType> elements;

        const auto numberOfElements = deserializer.ReadAndCheckStatus<
          EncodedIndex>();

        for (EncodedIndex index = 0; index < numberOfElements; index++)
        {
            elements.push_back(ReadElement());
        }

        return elements;
    };

    const auto ReadTruthTable = [&](TruthTable& truthTable)
    {
        const auto elementsVectorCount = deserializer.ReadAndCheckStatus<
          EncodedIndex>();

        for (EncodedIndex index = 0; index < elementsVectorCount; index++)
        {
            truthTable.push_back(ReadElements());
        }
    };

    ReadPorts(logicGate.input_ports);
    ReadPorts(logicGate.output_ports);

    ReadTruthTable(logicGate.input_truth_table);
    ReadTruthTable(logicGate.output_truth_table);

    return logicGate;
}

std::vector<std::byte> LogicGate::Serializer::Serialize() const
{
    ::Serializer serializer;

    const auto AddPort = [&](const EncodedIndex& portIndex)
    {
        serializer.AddVar(portIndex);
    };

    const auto AddElementType = [&](const ElementType& element)
    {
        serializer.AddVar<EncodedIndex>(element.index());

        switch (element.index())
        {
            case 0:
            {
                ///////////////////
                /// It's a port ///
                ///////////////////
                serializer.AddVar(std::get<0>(element));
                break;
            }

            case 1:
            {
                //////////////////
                /// It's a bit ///
                //////////////////
                serializer.AddVar(static_cast<std::uint8_t>(
                  std::get<1>(element).to_ullong()));
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
    };

    const auto AddPorts = [&](const std::vector<EncodedIndex>& ports)
    {
        serializer.AddVar<EncodedIndex>(ports.size());
        std::ranges::for_each(ports, AddPort);
    };

    const auto AddElements = [&](const std::vector<ElementType>& elements)
    {
        serializer.AddVar<EncodedIndex>(elements.size());
        std::ranges::for_each(elements, AddElementType);
    };

    const auto AddTruthTable = [&](const TruthTable& truthTable)
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
    //////////////////////////////////////////////////////////////////
    /// Get the bit in the truth table,                            ///
    /// there is two types:                                        ///
    /// - A port                                                   ///
    /// - A bit                                                    ///
    /// If it's a port,                                            ///
    /// then we get its state otherwise we just get the bit value  ///
    //////////////////////////////////////////////////////////////////
    const auto GetBitState = [&](const ElementType& element)
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
