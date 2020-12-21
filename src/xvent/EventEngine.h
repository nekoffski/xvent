#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include "Error.h"
#include "Event.h"
#include "EventEmitter.h"
#include "EventListener.h"
#include "EventProvider.h"

namespace xvent {

class EventEngine {
    using EventQueue = std::queue<std::shared_ptr<Event>>;

public:
    void spreadEvents() {
        for (auto& [ident, eventListener] : m_eventListeners) {

            auto eventProvider = EventProvider{};

            eventListener->handleEvents(eventProvider);
        }
    }

    void registerEventListener(std::shared_ptr<EventListener> eventListener) {
        const auto ident = eventListener->getIdent();
        if (m_eventListeners.contains(ident) || m_eventQueues.contains(ident))
            throw ListenerAlreadyRegistered{};

        m_eventListeners[ident] = eventListener;
        m_eventQueues[ident] = EventQueue{};
    }

    bool isListenerRegistered(const std::string& ident) const {
        return m_eventListeners.contains(ident) && m_eventQueues.contains(ident);
    }

private:
    std::unordered_map<std::string, std::shared_ptr<EventListener>> m_eventListeners;
    std::unordered_map<std::string, EventQueue> m_eventQueues;
};
}
