/**
@file utils/console.cpp
*/

#pragma once

#include <string>

namespace Foreground {
const std::string BLACK = "\u001b[30ma";
const std::string RED = "\u001b[31m";
const std::string GREEN = "\u001b[32m";
const std::string YELLOW = "\u001b[33m";
const std::string BLUE = "\u001b[34m";
const std::string MAGENTA = "\u001b[35m";
const std::string CYAN = "\u001b[36m";
const std::string WHITE = "\u001b[37m";

const std::string BRIGHT_BLACK = "\u001b[1;30m";
const std::string BRIGHT_RED = "\u001b[1;31m";
const std::string BRIGHT_GREEN = "\u001b[1;32m";
const std::string BRIGHT_YELLOW = "\u001b[1;33m";
const std::string BRIGHT_BLUE = "\u001b[1;34m";
const std::string BRIGHT_MAGENTA = "\u001b[1;35m";
const std::string BRIGHT_CYAN = "\u001b[1;36m";
const std::string BRIGHT_WHITE = "\u001b[1;37m";
} // namespace Foreground

namespace Background {
const std::string BLACK = "\u001b[40m";
const std::string RED = "\u001b[41m";
const std::string GREEN = "\u001b[42m";
const std::string YELLOW = "\u001b[43m";
const std::string BLUE = "\u001b[44m";
const std::string MAGENTA = "\u001b[45m";
const std::string CYAN = "\u001b[46m";
const std::string WHITE = "\u001b[47m";

const std::string BRIGHT_BLACK = "\u001b[1;40m";
const std::string BRIGHT_RED = "\u001b[1;41m";
const std::string BRIGHT_GREEN = "\u001b[1;42m";
const std::string BRIGHT_YELLOW = "\u001b[1;43m";
const std::string BRIGHT_BLUE = "\u001b[1;44m";
const std::string BRIGHT_MAGENTA = "\u001b[1;45m";
const std::string BRIGHT_CYAN = "\u001b[1;46m";
const std::string BRIGHT_WHITE = "\u001b[1;47m";
} // namespace Background

namespace Style {
const std::string RESET = "\u001b[0m";
const std::string BOLD = "\u001b[1m";
const std::string DIM = "\u001b[2m";
const std::string ITALIC = "\u001b[3m";
const std::string UNDERLINE = "\u001b[4m";
const std::string BLINKING = "\u001b[5m";
const std::string REVERSE = "\u001b[7m";
const std::string INVISIBLE = "\u001b[8m";
const std::string STRIKE_THROUGH = "\u001b[9m";
} // namespace Style