#pragma once

#include <memory>
#include <mutex>
#include <utility>
#include <vector>

#include "Error.h"
#include "Event.h"
#include "fwd.h"

namespace xvent {

class EventEmitter {
public:
    explicit EventEmitter(EventContainer& eventContainer);

    std::shared_ptr<EventEmitter> clone() {
        return std::make_shared<EventEmitter>(m_eventContainer);
    }

    template <EventType Ev, typename... Args>
    Result emit(Args&&... args) {
        std::lock_guard guard{ m_emitterMutex };
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);
        for (auto& [_, categoryMap] : m_eventContainer)
            pushEvent(event, categoryMap);

        return Result{ event->m_result.get_future() };
    }

    template <EventType Ev, typename... Args>
    Result emitTo(std::string destination, Args&&... args) {
        return emitTo<Ev, Args...>(std::vector<std::string>{ destination }, std::forward<Args>(args)...);
    }

    template <EventType Ev, typename... Args>
    Result emitTo(std::vector<std::string> destinations, Args&&... args) {
        std::lock_guard guard{ m_emitterMutex };
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);

        for (auto& destination : destinations) {
            if (not m_eventContainer.contains(destination))
                throw ListenerNotFound{};

            pushEvent(event, m_eventContainer[destination]);
        }

        return Result{ event->m_result.get_future() };
    }

private:
    void pushEvent(std::shared_ptr<Event> event, CategoryToEventQueue& categoryMap);

    EventContainer& m_eventContainer;
    std::mutex m_emitterMutex;
};
}
