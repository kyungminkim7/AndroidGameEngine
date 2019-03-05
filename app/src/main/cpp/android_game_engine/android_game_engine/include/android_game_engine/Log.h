#pragma once

#include <string>

namespace age {
namespace Log {

extern std::string tag;

void info(const std::string &msg);
void warn(const std::string &msg);
void error(const std::string &msg);
void fatal(const std::string &msg);

} // namespace log
} // namespace age