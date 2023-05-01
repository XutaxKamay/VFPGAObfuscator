#include "FPGA.h"

int main()
{
    FPGA fpga(2, 1, 0);

    auto logicGate = fpga.MakeLogicGate(
      {
        fpga.Ports().at(0),
        fpga.Ports().at(1)
    },
      { fpga.Ports().at(2) },
      { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0 }, { 0 }, { 0 }, { 1 } } });

    for (auto&& inputPort : logicGate->InputPorts())
    {
        inputPort->SetHigh();
    }

    fpga.PrepareStages();
    fpga.Simulate();

    std::cout << fpga.Ports().at(2)->GetState().to_string() << '\n';

    return 0;
}
