#include "LogicGate.h"

void LogicGate::Decoded::RunLogicFunction(
  const std::vector<Port*>& inputPorts,
  const std::vector<Port*>& outputPorts)
{
    //////////////////////////////////////////////////////////////////
    /// Get the bit in the truth table,                            ///
    /// there is two types:                                        ///
    /// - A port                                                   ///
    /// - A bit                                                    ///
    /// If it's a port,                                            ///
    /// then we get its state otherwise we just get the bit value  ///
    //////////////////////////////////////////////////////////////////
    const auto GetBitState = [&](const std::variant<Port*, Bit>& element)
    {
        Bit state;

        switch (element.index())
        {
            case 0:
            {
                ///////////////////
                /// It's a port ///
                ///////////////////
                state = std::get<0>(element)->GetState();
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
        }

        return state;
    };

    /////////////////////////////////////////////////////////////
    /// Verifies for each line if the truth table is verified ///
    /////////////////////////////////////////////////////////////
    for (std::size_t line_index = 0;
         line_index < input_truth_table.size();
         line_index++)
    {
        const auto& elements_on_line = input_truth_table[line_index];
        bool verifiesTruthTable      = false;

        //////////////////////////////////////////////////////////////////
        /// We are selecting the element/value in the truth table and  ///
        /// check if the input is the same as it is on the truth table ///
        //////////////////////////////////////////////////////////////////
        for (std::size_t column_index = 0;
             column_index < elements_on_line.size();
             column_index++)
        {
            const auto& element = elements_on_line[column_index];

            auto state = GetBitState(element);

            if (inputPorts[column_index]->GetState() == state)
            {
                verifiesTruthTable = true;
                break;
            }
        }

        ///////////////////////////////////////////////////////////////
        /// We found one match, apply all the outputs and exit loop ///
        ///////////////////////////////////////////////////////////////
        if (verifiesTruthTable)
        {
            for (std::size_t column_index = 0;
                 column_index < outputPorts.size();
                 column_index++)
            {
                outputPorts[column_index]->SetState(
                  GetBitState(output_truth_table[column_index]));
            }

            break;
        }
    }
}

std::vector<Port*> LogicGate::Encoded::GetInputPorts(
  const std::vector<Port*>& allPorts)
{
    std::vector<Port*> inputPorts;

    std::for_each_n(reinterpret_cast<encoded_index_t*>(
                      reinterpret_cast<std::uintptr_t>(this)
                      + offset_to_first_input_port),
                    number_of_input_ports,
                    [&](const auto& index)
                    {
                        inputPorts.push_back(allPorts.at(index));
                    });

    return inputPorts;
}

std::vector<Port*> LogicGate::Encoded::GetOutputPorts(
  const std::vector<Port*>& allPorts)
{
    std::vector<Port*> outputPorts;

    std::for_each_n(reinterpret_cast<encoded_index_t*>(
                      reinterpret_cast<std::uintptr_t>(this)
                      + offset_to_first_output_port),
                    number_of_output_ports,
                    [&](const auto& index)
                    {
                        outputPorts.push_back(allPorts.at(index));
                    });

    return outputPorts;
}

LogicGate::Decoded LogicGate::Encoded::DecodeLogicFunction(
  const std::vector<Port*>& allPorts)
{
    LogicGate::Decoded decoded;
    std::vector<std::variant<Port*, Bit>> values;

    //////////////////////////////
    /// Decode the truth table ///
    //////////////////////////////
    auto truthTableElements = reinterpret_cast<TruthTableElement*>(
      reinterpret_cast<std::uintptr_t>(this) + offset_to_truth_table);

    ////////////////////////////////////////////////////////////////
    /// An element/value can be either a boolean value (0 or 1°) ///
    /// or a Port encoded as an indexed port                     ///
    ////////////////////////////////////////////////////////////////
    static const auto DecodedElements =
      [&](TruthTableElement& truthTableElement)
    {
        switch (truthTableElement.type)
        {
            case TruthTableElement::type_t::BOOL:
            {
                const auto boolElement = reinterpret_cast<
                  BoolInTruthTable*>(&truthTableElement);
                values.push_back(boolElement->value);
                break;
            }

            case TruthTableElement::type_t::PORT:
            {
                const auto portElement = reinterpret_cast<
                  PortInTruthTable*>(&truthTableElement);
                values.push_back(allPorts.at(portElement->port_index));
                break;
            }
        }
    };

    ///////////////////////////////////////////////////////////
    /// First read the input ports that the logic gate uses ///
    ///////////////////////////////////////////////////////////
    std::size_t i = 0;
    for (; i < number_of_lines_in_truth_table - 1; i++)
    {
        std::for_each_n(truthTableElements + (i * number_of_input_ports),
                        number_of_input_ports,
                        DecodedElements);

        decoded.input_truth_table.push_back(values);
        values.clear();
    }

    /////////////////////////////////////////////////////////////
    /// Then, read the output ports that the logic gate uses  ///
    /// It's the last elements inside the truth table encoded ///
    /////////////////////////////////////////////////////////////
    std::for_each_n(truthTableElements + (i * number_of_input_ports),
                    number_of_output_ports,
                    DecodedElements);

    decoded.output_truth_table = values;

    return decoded;
}

LogicGate LogicGate::Encoded::ToLogicGate(
  const std::vector<Port*>& allPorts)
{
    const auto inputPorts  = GetInputPorts(allPorts);
    const auto outputPorts = GetOutputPorts(allPorts);
    const auto decoded     = DecodeLogicFunction(allPorts);

    return { inputPorts, outputPorts, decoded };
}

LogicGate::LogicGate(const std::vector<Port*>& inputPorts,
                     const std::vector<Port*>& outputPorts,
                     const Decoded& decoded)
 : _input_ports { inputPorts },
   _output_ports { outputPorts },
   _decoded { decoded }
{
}

void LogicGate::Simulate()
{
    _decoded.RunLogicFunction(_input_ports, _output_ports);
}
