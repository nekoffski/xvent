#include "EventEngine.h"
#include "Error.h"

namespace xvent {

void EventEngine::spreadEvents() {
    for (auto& [ident, eventListener] : m_eventListeners) {
        auto eventProvider = EventProvider{ m_eventContainer[ident] };
        eventListener->handleEvents(eventProvider);
    }

    for (auto& [_, events] : m_eventContainer)
        events.clear();
}

std::shared_ptr<EventEmitter> EventEngine::createEmitter() {
    return std::make_shared<EventEmitter>(m_eventContainer);
}

void EventEngine::registerEventListener(std::shared_ptr<EventListener> eventListener) {
    const auto ident = eventListener->getIdent();
    if (m_eventListeners.contains(ident) || m_eventContainer.contains(ident))
        throw ListenerAlreadyRegistered{};

    m_eventListeners[ident] = eventListener;
    m_eventContainer[ident] = CategoryToEventQueue{};
}

void EventEngine::unregisterEventListener(std::shared_ptr<EventListener> eventListener) {
    unregisterEventListener(eventListener->getIdent());
}

void EventEngine::unregisterEventListener(const std::string& ident) {
    if (not m_eventContainer.contains(ident) && not m_eventListeners.contains(ident))
        throw ListenerNotFound{};

    m_eventListeners.erase(ident);
    m_eventContainer.erase(ident);
}

bool EventEngine::isListenerRegistered(const std::string& ident) const {
    return m_eventListeners.contains(ident) && m_eventContainer.contains(ident);
}
}
