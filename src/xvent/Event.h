#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <typeindex>

#include "Category.h"

namespace xvent {

struct Event : std::enable_shared_from_this<Event> {
    virtual std::type_index getEventTypeIndex() const = 0;
    virtual std::type_index getCategoryTypeIndex() const = 0;

    template <typename T>
    void on(std::function<void(std::shared_ptr<T>)> callback) {
        if (typeid(T) == getEventTypeIndex())
            callback(std::dynamic_pointer_cast<T>(shared_from_this()));
    }

    template <typename T>
    bool is() {
        return typeid(T) == getEventTypeIndex();
    }

    template <typename T>
    std::shared_ptr<T> as() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }
};

template <typename T>
concept EventType = std::derived_from<T, Event>;

// clang-format off
template <typename Ev, CategoryType Category = DefaultCategory>
struct EventBase : Event {
    std::type_index getEventTypeIndex() const final override {
        return typeid(Ev);
    }

    std::type_index getCategoryTypeIndex() const final override {
        return typeid(Category);
    }
};
// clang-format on
}
