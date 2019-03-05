#include <android_game_engine/Utilities.h>

#include <algorithm>
#include <cctype>

namespace age {

void ltrim(std::string *s) {
    s->erase(s->begin(), std::find_if(s->begin(), s->end(),
            [](char ch){ return !std::isspace(ch); }));
}

void rtrim(std::string *s) {
    s->erase(std::find_if(s->rbegin(), s->rend(),
            [](char ch){ return !std::isspace(ch); }).base(), s->end());
}

} // namespace age