#include <android_game_engine/Log.h>

#include <cstdarg>
#include <android/log.h>

namespace age {
namespace Log {

std::string tag = "Game";

void info(const std::string &msg) {
    __android_log_print(ANDROID_LOG_INFO, tag.c_str(), "%s\n", msg.c_str());
}

void warn(const std::string &msg) {
    __android_log_print(ANDROID_LOG_WARN, tag.c_str(), "%s\n", msg.c_str());
}

void error(const std::string &msg) {
    __android_log_print(ANDROID_LOG_ERROR, tag.c_str(), "%s\n", msg.c_str());
}

void fatal(const std::string &msg) {
    __android_log_print(ANDROID_LOG_FATAL, tag.c_str(), "%s\n", msg.c_str());
}

} // namespace log
} // namespace age