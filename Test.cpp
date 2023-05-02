#include "FPGA.h"

int main()
{
    FPGA fpga(2, 1, 6);

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

    fpga.MakeLogicGate(
      {
        fpga.Ports().at(2)
    },
      { fpga.Ports().at(2) },
      { { { 0 }, { 1 } }, { { 1 }, { 0 } } });

    fpga.MakeLogicGate(
      {
        fpga.Ports().at(3)
    },
      { fpga.Ports().at(4) },
      { { { 0 }, { 1 } }, { { 1 }, { 0 } } });

    fpga.PrepareStages();
    fpga.Simulate();

    std::cout << fpga.Ports().at(2)->GetState() << '\n';
    std::cout << fpga.Ports().at(4)->GetState() << '\n';

    return 0;
}
