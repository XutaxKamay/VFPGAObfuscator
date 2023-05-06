#include "FPGA.h"
#include "Error.h"

FPGA::FPGA(std::size_t numberOfInputPins,
           std::size_t numberOfOutputPins,
           std::size_t numberOfOthersPorts)
 : _ports { numberOfInputPins + numberOfOutputPins + numberOfOthersPorts }
{
}

FPGA::~FPGA()
{
}

void FPGA::Simulate()
{
    static const auto StageExecution =
      [](std::vector<LogicGate>& logicGates)
    {
        static const auto minThreads = std::thread::hardware_concurrency();
        static const auto SimulateFunction = [](LogicGate& logicGate)
        {
            logicGate.Simulate();
        };

        //////////////////////////////////////////
        /// Only run in parallel if necessary. ///
        /// Check the comment bottom.          ///
        //////////////////////////////////////////
        if (logicGates.size() >= minThreads)
        {
            std::for_each(std::execution::par_unseq,
                          logicGates.begin(),
                          logicGates.end(),
                          SimulateFunction);
        }
        else
        {
            std::for_each(std::execution::seq,
                          logicGates.begin(),
                          logicGates.end(),
                          SimulateFunction);
        }
    };

    ///////////////////////////////////////////////////
    ///                                             ///
    /// It may be not advantageous here to use      ///
    /// parallelism for stages.                     ///
    /// First reason is for debugging purpose,      ///
    /// but the other reason is performance.        ///
    ///                                             ///
    /// This is because if we process stages        ///
    /// in parallel, the locking and unlocking      ///
    /// mechanisms will be expensive                ///
    /// compared to a simple iteration              ///
    /// due to the fact that we need to             ///
    /// synchronize stages together.                ///
    /// Never underestimate the power of CPU cache. ///
    ///                                             ///
    /// On top of that, the logic gates are         ///
    /// already simulated in parallel,              ///
    /// so if we create more threads                ///
    /// we will be already processing               ///
    /// logic gates, which will increase context    ///
    /// switchs, which will make maybe the program  ///
    /// more responsive to when processing another  ///
    /// stage, but in the end it will be slower.    ///
    /// Of course, this is only efficient           ///
    /// if there's a large enough of logic gates.   ///
    /// If each stages gets only one logic gate,    ///
    /// the whole thing will be slower than just    ///
    /// running it on one thread.                   ///
    /// But this is compensated above.              ///
    ///                                             ///
    /// If you want another reason, it's            ///
    /// because the CPU frequency might be          ///
    /// reducing depending on some computers        ///
    /// when there's too much threads taking        ///
    /// CPU.                                        ///
    ///                                             ///
    ///////////////////////////////////////////////////
    std::for_each(std::execution::seq,
                  _stages.begin(),
                  _stages.end(),
                  [](Stage& stage)
                  {
                      StageExecution(stage.logic_gates);
                  });
}

Port* FPGA::GetPort(std::size_t index)
{
    return &_ports[index];
}

const decltype(FPGA::_logic_gates)& FPGA::LogicGates() const
{
    return _logic_gates;
}

void FPGA::InsertLogicGate(const LogicGate& logicGate)
{
    const auto numberOfInputPorts  = logicGate.InputPorts().size();
    const auto numberOfOutputPorts = logicGate.OutputPorts().size();

    ///////////////////////////////////////////////////////
    /// Check if the truth table got the same amount of ///
    /// columns in every lines which is equal to the    ///
    /// input ports inside the logic gates              ///
    ///////////////////////////////////////////////////////
    const auto differentNumberOfInputPorts = std::find_if(
      std::execution::par_unseq,
      logicGate.Decoded().input_truth_table.begin(),
      logicGate.Decoded().input_truth_table.end(),
      [&](const std::vector<LogicGate::Decoder::ElementType>& elements)
      {
          return elements.size() != numberOfInputPorts;
      });

    if (differentNumberOfInputPorts
        != logicGate.Decoded().input_truth_table.end())
    {
        Error::ExitWithMsg(
          Error::Msg::
            FPGA_LOGIC_GATE_DECODER_INPUTS_IN_TURH_TABLE_NOT_CORRECT);
    }

    //////////////////////////////////////////
    /// Same as above but for output ports ///
    //////////////////////////////////////////
    const auto differentNumberOfOutputPorts = std::find_if(
      std::execution::par_unseq,
      logicGate.Decoded().output_truth_table.begin(),
      logicGate.Decoded().output_truth_table.end(),
      [&](const std::vector<LogicGate::Decoder::ElementType>& elements)
      {
          return elements.size() != numberOfOutputPorts;
      });

    if (differentNumberOfOutputPorts
        != logicGate.Decoded().output_truth_table.end())
    {
        Error::ExitWithMsg(
          Error::Msg::
            FPGA_LOGIC_GATE_DECODER_OUTPUTS_IN_TURH_TABLE_NOT_CORRECT);
    }

    /////////////////////////////////////////////////////////
    /// Check if there's the same number of lines for the ///
    /// outputs and inputs truth table                    ///
    /// We don't want some inputs condition               ///
    /// that has no outputs and vice versa ...            ///
    /////////////////////////////////////////////////////////
    if (logicGate.Decoded().input_truth_table.size()
        != logicGate.Decoded().output_truth_table.size())
    {
        Error::ExitWithMsg(
          Error::Msg::FPGA_LOGIC_GATE_DECODER_TRUTH_TABLE_NOT_CORRECT);
    }

    _logic_gates.push_back(logicGate);
}

void FPGA::InsertLogicGate(const std::vector<Port*>& inputPorts,
                           const std::vector<Port*>& outputPorts,
                           const LogicGate::Decoder& decoder)
{
    InsertLogicGate({ inputPorts, outputPorts, decoder });
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
    std::vector<LogicGate> logicGatesLeft = _logic_gates;

    ///////////////////////////////////
    /// We can get it by reference, ///
    /// it's safe enough here       ///
    ///////////////////////////////////
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
        /////////////////////////////////////////////////////////////
        /// Check if any previous logic gates have outputs ports  ///
        /// linked to the logic gates left in their inputs ports. ///
        /////////////////////////////////////////////////////////////
        std::ranges::for_each(
          logicGatesLeft,
          [&](LogicGate& logicGate)
          {
              const auto foundPort = std::ranges::find_if(
                logicGate.InputPorts(),
                [&](Port* inputPort)
                {
                    //////////////////////////////////
                    /// Check if there's any links ///
                    //////////////////////////////////
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
              if (foundPort == logicGate.InputPorts().end())
              {
                  currentOutputPorts.insert(
                    currentOutputPorts.begin(),
                    logicGate.OutputPorts().begin(),
                    logicGate.OutputPorts().end());

                  currentLogicGates.push_back(&logicGate);
              }
          });

        ////////////////////////////////////////////////////////////////
        /// Push back the array inside a new stage,                  ///
        /// And process again all the ignored logic gates previously ///
        ////////////////////////////////////////////////////////////////

        Stage stage;
        std::ranges::transform(currentLogicGates.begin(),
                               currentLogicGates.end(),
                               std::back_inserter(stage.logic_gates),
                               [](LogicGate* logicGate)
                               {
                                   return *logicGate;
                               });
        _stages.push_back(stage);

        ////////////////////////////////////////////////////////////////
        /// In order to get the logic gates left, we need to remove
        /// /// the current one we pushed earlier ///
        ////////////////////////////////////////////////////////////////
        logicGatesLeft.erase(
          std::remove_if(std::execution::par_unseq,
                         logicGatesLeft.begin(),
                         logicGatesLeft.end(),
                         [&currentLogicGates](LogicGate& logicGate)
                         {
                             return std::find(std::execution::par_unseq,
                                              currentLogicGates.begin(),
                                              currentLogicGates.end(),
                                              &logicGate)
                                    != currentLogicGates.end();
                         }),
          logicGatesLeft.end());

        currentLogicGates.clear();
        currentOutputPorts.clear();
    }
}
