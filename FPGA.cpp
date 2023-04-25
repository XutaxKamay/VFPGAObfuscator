#include "FPGA.h"

LogicGate FPGA::LogicGateRecord::ToLogicGate(
  std::array<Port*, 2> inputPort,
  Port* outputPort)
{
    LogicGate logicGate(
      inputPort,
      outputPort,
      [&](std::array<Port*, 2> inputPort, Port* outputPort)
      {
          std::array<std::array<Bit, 3>, 4> realTruthTable;

          for (std::size_t column = 0; column < 3; column++)
          {
              const auto realColumn = truth_table_ports_column[column];

              for (std::size_t line = 0; line < 4; line++)
              {
                  const auto value = truth_table[column][line];

                  realTruthTable[realColumn][line] = value;
              }
          }

          std::array<Bit, 2> currentStates = {
              inputPort[0]->GetState(),
              inputPort[1]->GetState(),
          };

          std::array<std::array<bool, 2>, 4> output;

          /////////////////////////////////////////////////
          /// TODO: This is not memory cache efficient. ///
          /////////////////////////////////////////////////
          for (std::size_t column = 0; column < 2; column++)
          {
              for (std::size_t line = 0; line < 4; line++)
              {
                  if (currentStates[column]
                      == realTruthTable[column][line])
                  {
                      output[line][column] = true;
                  }
                  else
                  {
                      output[line][column] = false;
                  }
              }
          }

          const auto laVerite = std::find_if(
            output.begin(),
            output.end(),
            [](const std::array<bool, 2>& value)
            {
                if (value[0] and value[1])
                {
                    return true;
                }

                return false;
            });

          if (laVerite != nullptr)
          {
              const auto index = laVerite - output.begin();
              outputPort->SetState(realTruthTable[2][index]);
          }
          else
          {
              throw std::runtime_error("Couldn't find la vérité");
          }
      });

    return logicGate;
}
