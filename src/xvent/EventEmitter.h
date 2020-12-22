#pragma once

#include <utility>
#include <vector>

#include "Error.h"
#include "fwd.h"

namespace xvent {

class EventEmitter {
public:
    explicit EventEmitter(EventContainer& eventContainer)
        : m_eventContainer(eventContainer) {
    }

    template <typename Ev, typename... Args>
    void emit(Args&&... args) {
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);
        for (auto& [_, categoryMap] : m_eventContainer) {
            auto& queue = categoryMap[event->getCategoryTypeIndex()];
            queue.push(event);
        }
    }

    template <typename Ev, typename... Args>
    void emitTo(std::string destination, Args&&... args) {
        emitTo<Ev, Args...>({ destination }, std::forward<Args>(args)...);
    }

    template <typename Ev, typename... Args>
    void emitTo(std::vector<std::string> destinations, Args&&... args) {
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);

        for (auto& destination : destinations) {
            if (not m_eventContainer.contains(destination))
                throw ListenerNotFound{};

            auto& destinationContainer = m_eventContainer[destination];
            auto& queue = destinationContainer[event->getCategoryTypeIndex()];
            queue.push(event);
        }
    }

private:
    EventContainer& m_eventContainer;
};
}
