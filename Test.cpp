#include "FPGA.h"

int main()
{
    FPGA fpga(2, 1, 0);

    fpga.MakeLogicGate(
      {
        fpga.Ports().at(0),
        fpga.Ports().at(1)
    },
      { fpga.Ports().at(2) },
      { { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } }, { 0, 0, 0, 1 } });

    fpga.Simulate();

    return 0;
}
