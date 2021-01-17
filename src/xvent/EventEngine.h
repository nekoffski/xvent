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
	std::shared_ptr<EventEmitter> createEmitter();

    void spreadEvents();
    void registerEventListener(std::shared_ptr<EventListener> eventListener);
    void unregisterEventListener(std::shared_ptr<EventListener> eventListener);
    void unregisterEventListener(const std::string& ident);

    bool isListenerRegistered(const std::string& ident) const;

private:
    EventListeners m_eventListeners;
    EventContainer m_eventContainer;
};
}
