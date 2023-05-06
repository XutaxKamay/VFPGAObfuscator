#ifndef FPGA_SIMULATOR_ERROR_H
#define FPGA_SIMULATOR_ERROR_H

#include "StandardHeaders.h"

class Error
{
  public:
    enum class Msg : int
    {
        NO_ERROR,
        UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE,
        FPGA_LOGIC_GATE_DECODER_INPUTS_IN_TURH_TABLE_NOT_CORRECT,
        FPGA_LOGIC_GATE_DECODER_OUTPUTS_IN_TURH_TABLE_NOT_CORRECT,
	FPGA_LOGIC_GATE_DECODER_TRUTH_TABLE_NOT_CORRECT
    };

    [[noreturn]] static void ExitWithMsg(Msg&& msg);
};

#endif
