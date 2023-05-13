#include "VFPGA.h"
#include "Deserializer.h"

using namespace VFPGAObfuscatorLibrary;
using namespace VFPGAObfuscatorSimulator;

VFPGA VFPGA::Deserializer::Deserialize(
  const std::vector<std::byte>& serialized)
{
    ::Deserializer deserializer { serialized };

    const auto hasStages = deserializer.ReadAndCheckStatus<bool>();

    const auto numberOfPorts = deserializer
                                 .ReadAndCheckStatus<EncodedIndex>();

    VFPGA vfpga { numberOfPorts };

    const auto ReadLogicGates = [&]()
    {
        std::vector<LogicGate> logicGates;
        const auto numberOfLogicGates = deserializer.ReadAndCheckStatus<
          EncodedIndex>();

        for (EncodedIndex index = 0; index < numberOfLogicGates; index++)
        {
            const auto logicGateSerialized = deserializer
                                               .ReadAndCheckStatus<
                                                 std::vector<std::byte>>();

            LogicGate::Deserializer logicGateDeserializer;
            logicGates.push_back(
              logicGateDeserializer.Deserialize(logicGateSerialized,
                                                &vfpga));
        }

        return logicGates;
    };

    if (hasStages)
    {
        const auto numberOfStages = deserializer
                                      .ReadAndCheckStatus<EncodedIndex>();

        for (EncodedIndex index = 0; index < numberOfStages; index++)
        {
            const auto logicGates = ReadLogicGates();

            vfpga.InsertLogicGates(logicGates);
            vfpga.stages.push_back({ logicGates });
        }
    }
    else
    {
        vfpga.InsertLogicGates(ReadLogicGates());
    }

    return vfpga;
}

VFPGA::VFPGA(const std::size_t numberOfPorts)
 : _ports { numberOfPorts }
{
}

Port* VFPGA::GetPort(std::size_t index)
{
    return &_ports[index];
}

std::size_t VFPGA::NumberOfPorts() const
{
    return _ports.size();
}

const std::vector<LogicGate>& VFPGA::LogicGates() const
{
    return _logic_gates;
}

void VFPGA::CheckLogicGateValidity(const LogicGate& logicGate)
{
    const auto numberOfInputPorts  = logicGate.input_ports.size();
    const auto numberOfOutputPorts = logicGate.output_ports.size();

    ///////////////////////////////////////////////////////
    /// Check if the truth table got the same amount of ///
    /// columns in every lines which is equal to the    ///
    /// input ports inside the logic gates              ///
    ///////////////////////////////////////////////////////
    const auto differentNumberOfInputPorts = std::find_if(
      std::execution::par_unseq,
      logicGate.input_truth_table.begin(),
      logicGate.input_truth_table.end(),
      [&](const std::vector<LogicGate::ElementType>& elements)
      {
          return elements.size() != numberOfInputPorts;
      });

    if (differentNumberOfInputPorts != logicGate.input_truth_table.end())
    {
        Error::ExitWithMsg(
          Error::Msg::
            VFPGA_LOGIC_GATE_DESERIALIZED_INPUTS_IN_TRUTH_TABLE_NOT_CORRECT);
    }

    //////////////////////////////////////////
    /// Same as above but for output ports ///
    //////////////////////////////////////////
    const auto differentNumberOfOutputPorts = std::find_if(
      std::execution::par_unseq,
      logicGate.output_truth_table.begin(),
      logicGate.output_truth_table.end(),
      [&](const std::vector<LogicGate::ElementType>& elements)
      {
          return elements.size() != numberOfOutputPorts;
      });

    if (differentNumberOfOutputPorts
        != logicGate.output_truth_table.end())
    {
        Error::ExitWithMsg(
          Error::Msg::
            VFPGA_LOGIC_GATE_DESERIALIZED_OUTPUTS_IN_TRUTH_TABLE_NOT_CORRECT);
    }

    /////////////////////////////////////////////////////////
    /// Check if there's the same number of lines for the ///
    /// outputs and inputs truth table                    ///
    /// We don't want some inputs condition               ///
    /// that has no outputs and vice versa ...            ///
    /////////////////////////////////////////////////////////
    if (logicGate.input_truth_table.size()
        != logicGate.output_truth_table.size())
    {
        Error::ExitWithMsg(
          Error::Msg::VFPGA_LOGIC_GATE_DESERIALIZED_TRUTH_TABLE_NOT_CORRECT);
    }
}

void VFPGA::InsertLogicGate(const LogicGate& logicGate)
{
    CheckLogicGateValidity(logicGate);
    _logic_gates.push_back(logicGate);
}

void VFPGA::InsertLogicGates(const std::vector<LogicGate>& logicGates)
{
    std::ranges::transform(logicGates.begin(),
                           logicGates.end(),
                           std::back_inserter(_logic_gates),
                           [&](const LogicGate& logicGate)
                           {
                               CheckLogicGateValidity(logicGate);
                               return logicGate;
                           });
}

void VFPGA::PrepareStages()
{
    if (not stages.size())
    {
        stages = CheckDependencyAndCreateStages<LogicGate, Port*, Stage>(
          _logic_gates);
    }
}

void VFPGA::Simulate()
{
    const auto StageExecution = [](std::vector<LogicGate>& logicGates)
    {
        /////////////////////////////////////////////////
        /// TODO:                                     ///
        /// 2 in theory but this is only if we think  ///
        /// that there's no thread starting time cost ///
        /// and this could be adjusted in the future. ///
        /// Adjust automatically ?                    ///
        /////////////////////////////////////////////////
        static constexpr auto minLG = 2; /// std::thread::hardware_concurrency();

        const auto SimulateFunction = [](LogicGate& logicGate)
        {
            logicGate.Simulate();
        };

        //////////////////////////////////////////
        /// Only run in parallel if necessary. ///
        /// Check the comment bottom.          ///
        //////////////////////////////////////////
        if (logicGates.size() >= minLG)
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
                  stages.begin(),
                  stages.end(),
                  [&](Stage& stage)
                  {
                      StageExecution(stage.logic_gates);
                  });
}
