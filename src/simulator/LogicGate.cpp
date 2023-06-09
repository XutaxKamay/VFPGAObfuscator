#include "LogicGate.h"
#include "Deserializer.h"
#include "VFPGA.h"

using namespace VFPGAObfuscatorLibrary;
using namespace VFPGAObfuscatorSimulator;

LogicGate LogicGate::Deserializer::Deserialize(
  const std::vector<std::byte>& serialized,
  VFPGA* const vfpga) const
{
    LogicGate logicGate;
    ::Deserializer deserializer { serialized };

    const auto ReadPorts = [&](std::vector<Port*>& ports)
    {
        const auto numberOfPorts = deserializer
                                     .ReadAndCheckStatus<EncodedIndex>();

        for (EncodedIndex index = 0; index < numberOfPorts; index++)
        {
            const auto portIndex = deserializer.ReadAndCheckStatus<
              Serializer::Port>();

            if (portIndex >= vfpga->NumberOfPorts())
            {
                Error::ExitWithMsg(
                  Error::Msg::LOGIC_GATE_DESERIALIZER_READ_FAILED);
            }

            ports.push_back(vfpga->GetPort(portIndex));
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
                  Serializer::Port>();

                if (portIndex >= vfpga->NumberOfPorts())
                {
                    Error::ExitWithMsg(
                      Error::Msg::LOGIC_GATE_DESERIALIZER_READ_FAILED);
                }

                element = vfpga->GetPort(portIndex);
                break;
            }

            case 1:
            {
                //////////////////
                /// It's a bit ///
                //////////////////
                element = Bit {
                    deserializer.ReadAndCheckStatus<decltype(Bit::value)>()
                };
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
                state = std::get<Port*>(element)->state;
                break;
            }

            case 1:
            {
                //////////////////
                /// It's a bit ///
                //////////////////
                state = std::get<Bit>(element);
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

            if (input_ports[columnIndex]->state != state)
            {
                ////////////////////////////////////////////////////
                /// So here is maybe a good example where goto   ///
                /// can be useful, instead of wasting a variable ///
                /// to check if the truth table is verified      ///
                /// and apply output ports states                ///
                /// we can tell to the cpu to skip the line      ///
                /// and let flow the program to the code below   ///
                /// if all input ports states are verified       ///
                /// If not it will set the output ports states   ///
                /// and break the outer loop                     ///
                ////////////////////////////////////////////////////
                goto SKIP_LINE;
            }
        }

        ///////////////////////////////////////////////////////////////
        /// We found one match, apply all the outputs and exit loop ///
        ///////////////////////////////////////////////////////////////
        for (std::size_t columnIndex = 0;
             columnIndex < output_ports.size();
             columnIndex++)
        {
            output_ports[columnIndex]->state = GetBitState(
              output_truth_table[lineIndex][columnIndex]);
        }

        break;

    SKIP_LINE:;
    }
}
