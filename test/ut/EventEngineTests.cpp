#include <gtest/gtest.h>

#include <memory>

#include "xvent/EventEngine.h"

namespace {

class EventEngineTests : public testing::Test {
protected:
    xvent::EventEngine eventEngine;
};

class ConcreteEventListener : public xvent::EventListener {
public:
    ConcreteEventListener()
        : xvent::EventListener(ident) {
    }

    void handleEvents(const xvent::EventProvider&) {
    }

    static inline std::string ident = "ident";
};

TEST_F(EventEngineTests, givenEventEngine_whenCheckingIfUnregisteredListenerIsRegistered_shouldReturnFalse) {
    EXPECT_FALSE(eventEngine.isListenerRegistered("notExistingListener"));
}

TEST_F(EventEngineTests, givenEventEngine_whenUnregisteringNotExistingListener_shouldThrow) {
    ASSERT_THROW(eventEngine.unregisterEventListener("notExistingListener"), xvent::ListenerNotFound);
}

TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListener_shouldRegister) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener);

    EXPECT_TRUE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));
}

TEST_F(EventEngineTests, giventEventEngine_whenUnregisteringService_shouldUnregister) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener);

    EXPECT_TRUE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));

    eventEngine.unregisterEventListener(listener);

    EXPECT_FALSE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));
}

TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListenerTwice_shouldThrow) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener);

    ASSERT_THROW(eventEngine.registerEventListener(listener), xvent::ListenerAlreadyRegistered);
}

struct EventA : xvent::EventBase<EventA> {};
struct EventB : xvent::EventBase<EventB> {};

struct Listener1 : public xvent::EventListener {
    using xvent::EventListener::EventListener;

    void handleEvents(const xvent::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

struct Listener2 : public xvent::EventListener {
    using xvent::EventListener::EventListener;

    void handleEvents(const xvent::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

class EventEngineEventsTest : public testing::Test {
protected:
    void SetUp() override {
        m_listener1 = std::make_shared<Listener1>("listener1");
        m_listener2 = std::make_shared<Listener2>("listener2");

        m_listener1->events = 0;
        m_listener2->events = 0;

        m_eventEngine.registerEventListener(m_listener1);
        m_eventEngine.registerEventListener(m_listener2);
    }

    xvent::EventEngine m_eventEngine;

    std::shared_ptr<Listener1> m_listener1;
    std::shared_ptr<Listener2> m_listener2;
};

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsWithoutAnyEventEmitted_everyListenerShouldNotGetEvent) {
    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 0);
    EXPECT_EQ(m_listener2->events, 0);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEvents_everyListenerShouldGetOneEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emit<EventA>();

    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 1);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsTwiceWithoutNewEvents_everyListenerShouldGetOneEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emit<EventA>();

    m_eventEngine.spreadEvents();
    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 1);
}

TEST_F(EventEngineEventsTest, givenEngine_whenSpreadingEventsEmittedToOnlyOneListener_onlyThisListenerShouldGetEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    eventEmitter->emitTo<EventA>("listener1");

    m_eventEngine.spreadEvents();

    EXPECT_EQ(m_listener1->events, 1);
    EXPECT_EQ(m_listener2->events, 0);
}

struct Listener3 : public xvent::EventListener {
    using xvent::EventListener::EventListener;

    void handleEvents(const xvent::EventProvider& eventProvider) {
        for (auto& event : eventProvider.getAll())
            ++events;
    }

    int events = 0;
};

TEST_F(EventEngineEventsTest, giventEventListenerRegisteredLate_shouldGetEvent) {
    auto eventEmitter = m_eventEngine.createEmitter();
    
    auto listener3 = std::make_shared<Listener3>("listener3");
    m_eventEngine.registerEventListener(listener3);

    eventEmitter->emit<EventA>();

    m_eventEngine.spreadEvents();
    EXPECT_EQ(listener3->events, 1);

    eventEmitter->emit<EventA>();

    m_eventEngine.spreadEvents();
    EXPECT_EQ(listener3->events, 2);
}
}
