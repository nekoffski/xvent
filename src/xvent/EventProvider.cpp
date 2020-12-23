#include "EventProvider.h"

namespace xvent {

EventProvider::EventProvider(CategoryToEventQueue& categoryToEventQueue)
    : m_categoryToEventQueue(categoryToEventQueue) {
}

EventQueue EventProvider::getAll() const {
    EventQueue events;
    for (auto& [_, eventQueue] : m_categoryToEventQueue)
        events.insert(events.begin(), eventQueue.begin(), eventQueue.end());

    return events;
}
}
