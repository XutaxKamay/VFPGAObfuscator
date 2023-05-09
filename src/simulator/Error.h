#ifndef FPGA_SIMULATOR_ERROR_H
#define FPGA_SIMULATOR_ERROR_H

#include "StandardHeaders.h"

namespace FPGASimulator
{
    class Error
    {
      public:
        enum class Msg : int
        {
            NO_ERROR,
            UNKNOWN_ELEMENT_TYPE_IN_TRUTH_TABLE,
            FPGA_LOGIC_GATE_DESERIALIZED_INPUTS_IN_TURH_TABLE_NOT_CORRECT,
            FPGA_LOGIC_GATE_DESERIALIZED_OUTPUTS_IN_TURH_TABLE_NOT_CORRECT,
            FPGA_LOGIC_GATE_DESERIALIZED_TRUTH_TABLE_NOT_CORRECT,
	    LOGIC_GATE_DESERIALIZER_READ_FAILED
        };

        [[noreturn]] static void ExitWithMsg(Msg&& msg);
    };
}

#endif
