#include "FPGA.h"

FPGA::FPGA(std::size_t numberOfInputPins,
           std::size_t numberOfOutputPins,
           std::size_t numberOfOthersPorts)
 : _input_pins { numberOfInputPins },
   _output_pins { numberOfOutputPins },
   _ports { numberOfInputPins + numberOfOutputPins + numberOfOthersPorts }
{
    std::generate(std::execution::par_unseq,
                  _input_pins.begin(),
                  _input_pins.end(),
                  []
                  {
                      return new Pin;
                  });

    std::generate(std::execution::par_unseq,
                  _output_pins.begin(),
                  _output_pins.end(),
                  []
                  {
                      return new Pin;
                  });

    std::copy(std::execution::par_unseq,
              _input_pins.begin(),
              _input_pins.end(),
              _ports.begin());
    std::copy(std::execution::par_unseq,
              _output_pins.begin(),
              _output_pins.end(),
              _ports.begin() + numberOfInputPins);

    std::generate_n(std::execution::par_unseq,
                    _ports.begin() + numberOfInputPins
                      + numberOfOutputPins,
                    numberOfOthersPorts,
                    []
                    {
                        return new Port;
                    });
}

FPGA::~FPGA()
{
    std::for_each(std::execution::par_unseq,
                  _ports.begin(),
                  _ports.end(),
                  [](Port* port)
                  {
                      delete port;
                  });

    std::for_each(std::execution::par_unseq,
                  _logic_gates.begin(),
                  _logic_gates.end(),
                  [](LogicGate* logicGate)
                  {
                      delete logicGate;
                  });

    std::for_each(std::execution::par_unseq,
                  _stages.begin(),
                  _stages.end(),
                  [](Stage* stage)
                  {
                      delete stage;
                  });
}

void FPGA::Simulate()
{
    static const auto StageExecution =
      [](const std::vector<LogicGate*>& logicGates)
    {
        std::for_each(std::execution::par_unseq,
                      logicGates.begin(),
                      logicGates.end(),
                      [&](LogicGate* logicGate)
                      {
                          logicGate->Simulate();
                      });
    };

    std::ranges::for_each(_stages,
                          [](Stage* stage)
                          {
                              StageExecution(stage->logic_gates);
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
                    //////////////////////////////////;
                    return std::find(std::execution::par_unseq,
                                     currentOutputPorts.begin(),
                                     currentOutputPorts.end(),
                                     inputPort)
                           != currentOutputPorts.end();
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
        _stages.push_back(new Stage { currentLogicGates });

        ////////////////////////////////////////////////////////////////
        /// In order to get the logic gates left, we need to remove  ///
        /// the current one we pushed earlier                        ///
        ////////////////////////////////////////////////////////////////
        logicGatesLeft.erase(
          std::remove_if(logicGatesLeft.begin(),
                         logicGatesLeft.end(),
                         [&currentLogicGates](LogicGate* logicGate)
                         {
                             return std::find(std::execution::par_unseq,
                                              currentLogicGates.begin(),
                                              currentLogicGates.end(),
                                              logicGate)
                                    != currentLogicGates.end();
                         }),
          logicGatesLeft.end());

        currentLogicGates.clear();
        currentOutputPorts.clear();
    }
}
