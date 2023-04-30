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
