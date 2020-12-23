#pragma once

#include "Event.h"
#include "fwd.h"

namespace xvent {

namespace detail {
    using TypeVector = std::vector<std::type_index>;

    template <typename Category, typename... Rest>
    struct Extractor {
        static void extract(TypeVector& categories) {
            categories.emplace_back(typeid(Category));
            Extractor<Rest...>::extract(categories);
        }
    };

    template <typename Category>
    struct Extractor<Category> {
        static void extract(TypeVector& categories) {
            categories.emplace_back(typeid(Category));
        }
    };
}

class EventProvider {
public:
    explicit EventProvider(CategoryToEventQueue& categoryToEventQueue)
        : m_categoryToEventQueue(categoryToEventQueue) {
    }

    EventQueue getAll() {
        EventQueue events;
        for (auto& [_, eventQueue] : m_categoryToEventQueue)
            events.insert(events.begin(), eventQueue.begin(), eventQueue.end());

        return events;
    }

    template <typename... Categories>
    EventQueue getByCategories() {
        detail::TypeVector categories;
        detail::Extractor<Categories...>::extract(categories);

        EventQueue events;
        for (const auto& categoryIndex : categories) {
            if (not m_categoryToEventQueue.contains(categoryIndex))
                continue;

            auto& queue = m_categoryToEventQueue[categoryIndex];
            events.insert(events.begin(), queue.begin(), queue.end());
        }

        return events;
    }

private:
    CategoryToEventQueue& m_categoryToEventQueue;
};
}
