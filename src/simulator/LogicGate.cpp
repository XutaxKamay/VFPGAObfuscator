#include "LogicGate.h"
#include "Error.h"

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
            default:
            {
                ///////////////////////////
                /// Should never happen ///
                ///////////////////////////
                Error::ExitWithMsg(
                  Error::msg_t::UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE);
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

            if (inputPorts[columnIndex]->GetState() == state)
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
                 columnIndex < outputPorts.size();
                 columnIndex++)
            {
                outputPorts[columnIndex]->SetState(GetBitState(
                  output_truth_table[lineIndex][columnIndex]));
            }

            break;
        }
    }
}

LogicGate::LogicGate(const std::vector<Port*>& inputPorts,
                     const std::vector<Port*>& outputPorts,
                     const Decoded& decoded)
 : _input_ports { inputPorts },
   _output_ports { outputPorts },
   _decoded { decoded }
{
}

decltype(LogicGate::_input_ports)& LogicGate::InputPorts()
{
    return _input_ports;
}

decltype(LogicGate::_output_ports)& LogicGate::OutputPorts()
{
    return _output_ports;
}

void LogicGate::Simulate()
{
    _decoded.RunLogicFunction(_input_ports, _output_ports);
}

