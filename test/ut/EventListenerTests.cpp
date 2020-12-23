#include <gtest/gtest.h>

#include "xvent/EventListener.h"

namespace {

const std::string ident = "concreteIdent123";

class ConcreteEventListener : public xvent::EventListener {
public:
    ConcreteEventListener()
        : xvent::EventListener(ident) {
    }
    void handleEvents(const xvent::EventProvider&) {}
};

TEST(EventListenerTests, givenEventListener_whenGettingIdent_expectCorrectIdent) {
    ASSERT_EQ(ident, ConcreteEventListener{}.getIdent());
}
}
