#pragma once

#include <utility>
#include <vector>

#include "Error.h"
#include "Event.h"
#include "fwd.h"

namespace xvent {

class EventEmitter {
public:
    explicit EventEmitter(EventContainer& eventContaine);

    template <typename Ev, typename... Args>
    void emit(Args&&... args) {
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);
        for (auto& [_, categoryMap] : m_eventContainer)
            pushEvent(event, categoryMap);
    }

    template <typename Ev, typename... Args>
    void emitTo(std::string destination, Args&&... args) {
        emitTo<Ev, Args...>(std::vector<std::string>{ destination }, std::forward<Args>(args)...);
    }

    template <typename Ev, typename... Args>
    void emitTo(std::vector<std::string> destinations, Args&&... args) {
        auto event = std::make_shared<Ev>(std::forward<Args>(args)...);

        for (auto& destination : destinations) {
            if (not m_eventContainer.contains(destination))
                throw ListenerNotFound{};

            pushEvent(event, m_eventContainer[destination]);
        }
    }

private:
    void pushEvent(std::shared_ptr<Event> event, CategoryToEventQueue& categoryMap);

    EventContainer& m_eventContainer;
};
}
