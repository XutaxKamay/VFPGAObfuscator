#include "FPGA.h"

std::vector<Port*> FPGA::PinsRecord::ReadPins()
{
}

std::vector<Port*> FPGA::LogicGateRecord::GetInputPorts(
  const std::vector<Port*>& allPorts,
  const FPGA::Record* const encodedInputPorts)
{
}

std::vector<Port*> FPGA::LogicGateRecord::GetOutputPorts(
  const std::vector<Port*>& allPorts,
  const FPGA::Record* const encodedOutputPorts)
{
}

LogicGate::Decoder FPGA::LogicGateRecord::DecodeLogicFunction(
  const std::vector<Port*>& allPorts,
  const FPGA::Record* const encodedLogicFunction)
{
}

LogicGate FPGA::LogicGateRecord::ToLogicGate(
  const std::vector<Port*>& allPorts,
  const FPGA::Record* const encodedLogicGate)
{
    const auto inputPorts = GetInputPorts(allPorts);
}

