#pragma once

#include <memory>
#include <vector>
#include <typeindex>
#include <unordered_map>

namespace xvent {

struct Event;
class EventListener;

using EventListeners = std::unordered_map<std::string, std::shared_ptr<EventListener>>;
using EventQueue = std::vector<std::shared_ptr<Event>>;
using CategoryToEventQueue = std::unordered_map<std::type_index, EventQueue>;
using EventContainer = std::unordered_map<std::string, CategoryToEventQueue>;
};
