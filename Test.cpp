#include "FPGA.h"
#include "Timer.h"

int main()
{
    FPGA fpga(2, 1, 1000000);

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

    for (int i = 3; i < 1000000 - 1;)
    {
        fpga.MakeLogicGate(
          {
            fpga.Ports().at(i)
        },
          { fpga.Ports().at(i + 1) },
          { { { 0 }, { 1 } }, { { 1 }, { 0 } } });

        i += 2;
    }

    fpga.PrepareStages();

    while (true)
    {
        Timer timer;

        timer.Start();
	fpga.Simulate();
	timer.End();

        std::cout << timer.Difference() << '\n';
    }

    return 0;
}
