#include "FPGA.h"
#include "Timer.h"

int main()
{
    FPGA fpga(2, 1, 1000000);

    auto logicGate = LogicGate(
      {
        fpga.GetPort(0),
        fpga.GetPort(1)
    },
      { fpga.GetPort(2) },
      { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } },
        { { 0 }, { 0 }, { 0 }, { 1 } } });

    fpga.InsertLogicGate(logicGate);

    for (auto inputPort : logicGate.InputPorts())
    {
        inputPort->SetHigh();
    }

    fpga.InsertLogicGate(
      {
        fpga.GetPort(2)
    },
      { fpga.GetPort(2) },
      { { { 0 }, { 1 } }, { { 1 }, { 0 } } });

    for (std::size_t i = 3; i < 1000000 - 1;)
    {
        fpga.InsertLogicGate(
          {
            fpga.GetPort(i)
        },
          { fpga.GetPort(i + 1) },
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
