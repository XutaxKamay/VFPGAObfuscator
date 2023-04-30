#ifndef FPGA_SIMULATOR_FPGA_H
#define FPGA_SIMULATOR_FPGA_H

#include "LogicGate.h"

/*-----------------------------------------------------------------------
 |                                                                      |
 |                                FPGA                                  |
 |               ---------------------------------------                |
 |               |                                     |                |
 |               |     ██--|   CONNECTIONS   |--██     |                |
 |               |         |   ===========   |         |                |
 |               |     ██--|                 |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|    ---------    |--██     |                |
 |               |         |    | LOGIC |    |         |                |
 |               |     ██--|====|   -   |====|--██     |                |
 |               |         |    | GATES |    |         |                |
 |               |     ██--|    ---------    |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|                 |--██     |                |
 |               |         |                 |         |                |
 |               |     ██--|                 |--██     |                |
 |               |                                     |                |
 |               | INPUT PINS              OUTPUT PINS |                |
 |               ---------------------------------------                |
 |                                                                      |
 |                                                                      |
 |                    A Small Part Of Logic Hell Gates                  |
 |      ---------------------------------------------------------       |
 |      | STAGE X       ║ STAGE X+1 ║ STAGE X+2 ║     STAGE X+3 |       |
 |      |               ║           ║           ║               |       |
 |  (1)-|>========|     ║           ║           ║               |       |
 |      |         |=====$-|>[and]---$-|         ║     |=========|>-(1)  |
 |  (0)-|>========|     ║           ║ |         ║     |         |       |
 |      |               ║           ║ |         ║     |=========|>-(1)  |
 |  (0)-|>========|     ║           ║ -|>[or]---$=====|         |       |
 |      |         |=====$-|>[or]--|-$-|         ║     |=========|>-(1)  |
 |  (1)-|>========|     ║         | ║           ║               |       |
 |      |               ║         | ║           ║               |       |
 |      |               ║         |-$-|         ║     |=========|>-(0)  |
 |  (1)-|>========|     ║           ║ |         ║     |         |       |
 |      |         |=====$-|>[xor]-| ║ -|>[and]--$=====|=========|>-(0)  |
 |  (1)-|>========|     ║         |-$-|         ║     |                 |
 |      |               ║           ║           ║     |=========|>-(0)  |
 |      |               ║           ║           ║               |       |
 |      | INPUT PORTS   ║           ║           ║  OUTPUT PORTS |       |
 |      ---------------------------------------------------------       |
 |                                                                      |
 |                                                                      |
 |      In this scheme just above,                                      |
 |      the input ports represents                                      |
 |      the previous logic gates output ports.                          |
 |      The '$' character is a signal that says                         |
 |      that we can go onto the next stage.                             |
 |      All '$' characters must be reached before                       |
 |      going on another stage.                                         |
 |      This permits to get parallelism                                 |
 |      on our circuit since the logic gates                            |
 |      in each stages can be processed seperately                      |
 |      and are independent of each others.                             |
 |      The stages can be generated automatically by looking            |
 |      at the ports,                                                   |
 |      The more there is ports, the more parallelism we can get.       |
 |      Since our machine is virtual and                                |
 |      there's no physical limitation,                                 |
 |      we can get a very high parallelism                              |
 |      depending on the architecture.                                  |
 |                                                                      |
 -----------------------------------------------------------------------*/

class FPGA
{
  public:
    ////////////////////////////////////////////////////
    /// A pin is also a port, just a pin is exposed, ///
    /// a port may be not exposed                    ///
    ////////////////////////////////////////////////////
    using Pin = Port;

  private:
    ////////////////////////////////////////////////////////////////////
    /// Oh no! Xutax, you're using raw pointers, this is really bad. ///
    /// I know, but there's precise reasons, mostly performance.     ///
    /// Technically we should use std::atomic<std::shared_ptr<>>.    ///
    /// Because yes, this will be done multithreaded,                ///
    /// but the lock might make loose our performance                ///
    /// quite a bit since there can be, quite a LOT, of logic gates. ///
    /// std::shared_ptr is mostly behind the scene a counter         ///
    /// that keeps track of the number of references of the pointer, ///
    /// if that keeps decreasing and increasing for nothing          ///
    /// with locks, even though they're fast between lock            ///
    /// and unlocking, it will make even loose some seconds          ///
    /// depending on the amount of logic gates.                      ///
    /// And I don't want that.                                       ///
    /// Fortunately, it's relatively easy to do so with constructors ///
    /// and destructors to keep it safe.                             ///
    ////////////////////////////////////////////////////////////////////
    std::vector<Pin*> _input_pins;
    std::vector<Pin*> _output_pins;
    std::vector<Port*> _ports;

    ///////////////////////////////////////////////////////////////
    ///                                                         ///
    /// Double vectors for parallelism.                         ///
    ///                                                         ///
    /// The idea is here is to process logic gates in parallel, ///
    /// hence why there is double vectors.                      ///
    /// Ideally, they represent different stages where we know  ///
    /// that each logic gates are not dependent on each others  ///
    ///                                                         ///
    ///////////////////////////////////////////////////////////////
    std::vector<LogicGate*> _logic_gates;
    std::vector<std::vector<LogicGate*>> _stages;

  public:
    FPGA(std::size_t numberOfInputPins,
         std::size_t numberOfOutputPins,
         std::size_t numberOfOthersPorts);
    ~FPGA();

    decltype(_ports)& Ports();
    decltype(_logic_gates)& LogicGates();

    LogicGate* MakeLogicGate(const std::vector<Port*>& inputPorts,
                             const std::vector<Port*>& outputPorts,
			     const LogicGate::Decoded& decoder);

    void PrepareStages();
    void Simulate();

  private:
    void CheckDependencyAndCreateStages();
};

#endif
