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

    struct Stage
    {
        std::vector<LogicGate> logic_gates;
    };

  private:
    ///////////////////////////////////////////////////////////
    /// Number of pins and ports doesn't change,            ///
    /// so we can get them by references as it is allocated ///
    /// during only the constructor                         ///
    ///////////////////////////////////////////////////////////
    std::vector<Port> _ports;

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
    std::vector<LogicGate> _logic_gates;
    std::vector<Stage> _stages;

  public:
    FPGA(std::size_t numberOfInputPins,
         std::size_t numberOfOutputPins,
         std::size_t numberOfOthersPorts);
    ~FPGA();

    Port* GetPort(std::size_t index);
    decltype(_logic_gates)& LogicGates();

    void InsertLogicGate(const LogicGate& logicGate);

    void InsertLogicGate(const std::vector<Port*>& inputPorts,
                         const std::vector<Port*>& outputPorts,
                         const LogicGate::Decoded& decoder);

    void PrepareStages();
    void Simulate();

  private:
    ///////////////////////////////////////////////////////////////
    /// This should be compiled before,                         ///
    /// since this can be created before running the simulator. ///
    /// With a lot of logic gates, it may take a while.         ///
    ///////////////////////////////////////////////////////////////
    void CheckDependencyAndCreateStages();
};

#endif
