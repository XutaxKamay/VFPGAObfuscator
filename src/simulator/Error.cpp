#include "Error.h"

using namespace FPGASimulator;

void Error::ExitWithMsg(Msg msg)
{
    std::exit(static_cast<int>(msg));
}
