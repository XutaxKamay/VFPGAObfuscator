#include "Error.h"

void Error::ExitWithMsg(msg_t&& msg)
{
    std::exit(static_cast<int>(msg));
}
