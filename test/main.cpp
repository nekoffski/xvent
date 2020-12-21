#include <iostream>

#include <xvent/Event.h>
#include <xvent/EventEngine.h>

struct A : xvent::EventBase<A> {
};

struct B : xvent::EventBase<B> {
};

void dispatch(std::shared_ptr<xvent::Event> event) {
    event->on<A>([](std::shared_ptr<A> a) {
        std::cout << "EventA\n";
    });

    event->on<B>([](std::shared_ptr<B> b) {
        std::cout << "EventB\n";
    });

    if (event->is<A>()) {
        std::cout << "EventA\n";
    }
}

int main() {
    auto ev = std::make_shared<A>();
    dispatch(ev);

    std::cout << "Hello world\n";
    return 0;
}
