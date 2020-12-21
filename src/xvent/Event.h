#pragma once

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

template <typename Ev, typename Category = DefaultCategory>
struct EventBase : Event {
    std::type_index getEventTypeIndex() const final override {
        return typeid(Ev);
    }

    std::type_index getCategoryTypeIndex() const final override {
        return typeid(Category);
    }
};
}
