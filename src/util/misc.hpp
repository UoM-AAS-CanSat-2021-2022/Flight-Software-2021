#pragma once

#include <algorithm>
#include <optional>

#include <Arduino.h>
#undef b1

namespace util {
    template<typename T>
    std::optional<T> median(std::vector<T>& v) {
        if (v.empty()) {
            return {};
        }

        const auto idx = v.size() / 2;
        auto m = v.begin() + idx;
        std::nth_element(v.begin(), m, v.end());
        return { v[idx] };
    }

    time_t getTeensy3Time();
}
