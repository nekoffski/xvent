#pragma once

#include <memory>
#include <queue>
#include <typeindex>
#include <unordered_map>

namespace xvent {

struct Event;
class EventListener;

using EventListeners = std::unordered_map<std::string, std::shared_ptr<EventListener>>;
using EventQueue = std::queue<std::shared_ptr<Event>>;
using CategoryToEventQueue = std::unordered_map<std::type_index, EventQueue>;
using EventContainer = std::unordered_map<std::string, CategoryToEventQueue>;
};
