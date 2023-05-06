#include "Error.h"

void Error::ExitWithMsg(Msg&& msg)
{
    std::exit(static_cast<int>(msg));
}
