#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "Containers.hpp"

namespace xvent {

struct Event;
class EventListener;

using EventListeners = AtomicUnorderedMap<std::string, std::shared_ptr<EventListener>>;
using EventQueue = AtomicVector<std::shared_ptr<Event>>;
using CategoryToEventQueue = AtomicUnorderedMap<std::type_index, EventQueue>;
using EventContainer = AtomicUnorderedMap<std::string, CategoryToEventQueue>;
};
