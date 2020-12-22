#pragma once

#include <memory>
#include <queue>
#include <unordered_map>

#include "Error.h"
#include "Event.h"
#include "EventEmitter.h"
#include "EventListener.h"
#include "EventProvider.h"
#include "fwd.h"

namespace xvent {

class EventEngine {
public:
    void spreadEvents() {
        for (auto& [ident, eventListener] : m_eventListeners) {

            auto eventProvider = EventProvider{};

            eventListener->handleEvents(eventProvider);
        }
    }

    EventEmitter createEmitter() {
        return EventEmitter{ m_eventContainer };
    }

    void registerEventListener(std::shared_ptr<EventListener> eventListener) {
        const auto ident = eventListener->getIdent();
        if (m_eventListeners.contains(ident) || m_eventContainer.contains(ident))
            throw ListenerAlreadyRegistered{};

        m_eventListeners[ident] = eventListener;
        m_eventContainer[ident] = CategoryToEventQueue{};
    }

    void unregisterEventListener(std::shared_ptr<EventListener> eventListener) {
        unregisterEventListener(eventListener->getIdent());
    }

    void unregisterEventListener(const std::string& ident) {}

    bool isListenerRegistered(const std::string& ident) const {
        return m_eventListeners.contains(ident) && m_eventContainer.contains(ident);
    }

private:
    EventListeners m_eventListeners;
    EventContainer m_eventContainer;
};
}
