#pragma once

#include <concepts>

namespace xvent {

struct Category {
};

struct DefaultCategory : Category {
};

template <typename T>
concept CategoryType = std::derived_from<T, Category>;
}
