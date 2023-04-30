#include "FPGA.h"

FPGA::FPGA(std::size_t numberOfInputPins,
           std::size_t numberOfOutputPins,
           std::size_t numberOfOthersPorts)
 : _input_pins { numberOfInputPins },
   _output_pins { numberOfOutputPins }
{
    std::ranges::generate(_input_pins,
                          []
                          {
                              return new Pin;
                          });

    std::ranges::generate(_output_pins,
                          []
                          {
                              return new Pin;
                          });

    _ports.insert(_ports.end(), _input_pins.begin(), _input_pins.end());
    _ports.insert(_ports.end(), _output_pins.begin(), _output_pins.end());

    std::ranges::generate_n(_ports.begin() + numberOfInputPins
                              + numberOfOthersPorts,
                            numberOfOthersPorts,
                            []
                            {
                                return new Port;
                            });
}

FPGA::~FPGA()
{
    for (auto&& port : _ports)
    {
        delete port;
    }

    for (auto&& logicGate : _logic_gates)
    {
        delete logicGate;
    }
}

void FPGA::Simulate()
{
    std::ranges::for_each(_stages,
                          [](std::vector<LogicGate*>& stage)
                          {
                              std::ranges::for_each(
                                stage,
                                [](LogicGate* logicGate)
                                {
                                    logicGate->Simulate();
                                });
                          });
}

decltype(FPGA::_ports)& FPGA::Ports()
{
    return _ports;
}

decltype(FPGA::_logic_gates)& FPGA::LogicGates()
{
    return _logic_gates;
}

LogicGate* FPGA::MakeLogicGate(const std::vector<Port*>& inputPorts,
                               const std::vector<Port*>& outputPorts,
                               const LogicGate::Decoded& decoder)
{
    ////////////////////////////////////////////////
    /// TODO: Check if decoder is correct;       ///
    /// truth table is written correctly etc ... ///
    ////////////////////////////////////////////////
    auto logicGate = new LogicGate { inputPorts, outputPorts, decoder };

    _logic_gates.push_back(logicGate);

    return logicGate;
}

void FPGA::PrepareStages()
{
    CheckDependencyAndCreateStages();
}

void FPGA::CheckDependencyAndCreateStages()
{
    ////////////////////////////////////////////
    /// Each stages will be represented as a ///
    /// vector of LogicGate*                 ///
    ////////////////////////////////////////////
    std::vector<LogicGate*> logicGatesLeft = _logic_gates;
    std::vector<LogicGate*> currentLogicGates;
    std::vector<Port*> currentOutputPorts;

    ////////////////////////////////////////////////////////////////////
    /// Each stages will contain logic gates that are not depending  ///
    /// on each others which is useful for parallelism.              ///
    ///                                                              ///
    /// In order to divide the logic gates in multiple stages,       ///
    /// we need to check if the previous logic gates                 ///
    /// has outputs has links/connections                            ///
    /// to the next logic gates inputs.                              ///
    ///                                                              ///
    /// If any previous logic gates we registered in an array        ///
    /// have links/connections to the next ones,                     ///
    /// then we ignore the next logic gate                           ///
    /// and we go on again on another logic gate                     ///
    /// until we traversed all the possible logic gates.             ///
    ///                                                              ///
    /// If not, we keep track of that next logic gate                ///
    /// inside an array and we keep searching for links/connections. ///
    ///                                                              ///
    /// This permits to simulate logic gates fully in parallel       ///
    /// (without locks) and stages themselves (with locks).          ///
    ////////////////////////////////////////////////////////////////////
    while (logicGatesLeft.size())
    {
        ////////////////////////////////////////////////////////////
        /// Check if any previous logic gates have outputs ports ///
        /// linkedto the logic gates left in their inputs ports. ///
        ////////////////////////////////////////////////////////////
        std::ranges::for_each(
          logicGatesLeft,
          [&](LogicGate* logicGate)
          {
              const auto foundPort = std::ranges::find_if(
                logicGate->InputPorts(),
                [&](Port* inputPort)
                {
                    //////////////////////////////////
                    /// Check if there's any links ///
                    //////////////////////////////////
                    const auto foundPort = std::ranges::find_if(
                      currentOutputPorts,
                      [&](Port* outputPort)
                      {
                          return inputPort == outputPort;
                      });

                    return foundPort != currentOutputPorts.end();
                });

              /////////////////////////////////////////////////
              /// If we found a previous                    ///
              /// logic gates output port linked            ///
              /// to an input port in the logic gates left, ///
              /// We simply skip it for another stage.      ///
              /// If not found, we insert the current       ///
              /// logic gate inside the array.              ///
              /////////////////////////////////////////////////
              if (foundPort == logicGate->InputPorts().end())
              {
                  currentOutputPorts.insert(
                    currentOutputPorts.begin(),
                    logicGate->OutputPorts().begin(),
                    logicGate->OutputPorts().end());

                  currentLogicGates.push_back(logicGate);
              }
          });

        ////////////////////////////////////////////////////////////////
        /// Push back the array inside a new stage,                  ///
        /// And process again all the ignored logic gates previously ///
        ////////////////////////////////////////////////////////////////
        _stages.push_back(currentLogicGates);

        ////////////////////////////////////////////////////////////////
        /// In order to get the logic gates left, we need to remove  ///
        /// the current one we pushed earlier                        ///
        ////////////////////////////////////////////////////////////////
        logicGatesLeft.erase(std::remove_if(
          logicGatesLeft.begin(),
          logicGatesLeft.end(),
          [&currentLogicGates](LogicGate* logicGate)
          {
              const auto foundLogicGate = std::ranges::find_if(
                currentLogicGates,
                [&](LogicGate* logicGate2)
                {
                    return logicGate == logicGate2;
                });

              return foundLogicGate != currentLogicGates.end();
          }));

        currentLogicGates.clear();
        currentOutputPorts.clear();
    }
}
