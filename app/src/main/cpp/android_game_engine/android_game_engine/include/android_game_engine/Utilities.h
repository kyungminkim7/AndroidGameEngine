#pragma once

#include <string>

namespace age {

void ltrim(std::string *s);
void rtrim(std::string *s);

template <typename T>
T clip(const T &n, const T &lowerBound, const T &upperBound);

template <typename T>
T clip(const T &n, const T &lowerBound, const T &upperBound) {
    return std::max(lowerBound, std::min(n, upperBound));
}

} // namespace age