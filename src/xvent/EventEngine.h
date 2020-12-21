#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include "Event.h"
#include "EventEmitter.h"
#include "EventListener.h"
#include "EventProvider.h"

namespace xvent {

class EventEngine {
    using EventQueue = std::queue<std::shared_ptr<Event>>;

public:
    void spreadEvents() {
    }

    void registerEventListener(std::shared_ptr<EventListener> eventListener) {
        const auto ident = eventListener->getIdent();
        if (m_eventListeners.contains(ident) || m_eventQueues.contains(ident)) {
            // err
        }

        m_eventListeners[ident] = eventListener;
        m_eventQueues[ident] = EventQueue{};
    }

private:
    std::unordered_map<std::string, std::shared_ptr<EventListener>> m_eventListeners;
    std::unordered_map<std::string, EventQueue> m_eventQueues;
};
}
