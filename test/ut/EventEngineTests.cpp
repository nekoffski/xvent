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

TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListener_shouldRegister) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener);

    EXPECT_TRUE(eventEngine.isListenerRegistered(ConcreteEventListener::ident));
}

TEST_F(EventEngineTests, givenEventEngine_whenRegisteringListenerTwice_shouldThrow) {
    auto listener = std::make_shared<ConcreteEventListener>();
    eventEngine.registerEventListener(listener);

    ASSERT_THROW(eventEngine.registerEventListener(listener), xvent::ListenerAlreadyRegistered);
}
}
