#ifndef FPGA_SIMULATOR_ERROR_H
#define FPGA_SIMULATOR_ERROR_H

#include "StandardHeaders.h"

class Error
{
  public:
    enum class msg_t : int
    {
        NO_ERROR,
        UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE,
	FPGA_LOGIC_GATE_DECODER_NOT_CORRECT
    };

    [[noreturn]] static void ExitWithMsg(msg_t&& msg);
};

#endif
