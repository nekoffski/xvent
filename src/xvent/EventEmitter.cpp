#include "EventEmitter.h"

namespace xvent {

EventEmitter::EventEmitter(EventContainer& eventContainer)
    : m_eventContainer(eventContainer) {
}

void EventEmitter::pushEvent(std::shared_ptr<Event> event, CategoryToEventQueue& categoryMap) {
    auto categoryTypeIndex = event->getCategoryTypeIndex();
    if (not categoryMap.contains(categoryTypeIndex))
        categoryMap[categoryTypeIndex] = EventQueue{};

    auto& queue = categoryMap[categoryTypeIndex];
    queue.push(event);
}
}
