#include <gtest/gtest.h>

#include <xvent/Category.h>
#include <xvent/Event.h>

namespace {

struct CustomCategory : xvent::Category {};

struct EventA : xvent::EventBase<EventA> {};

struct EventB : xvent::EventBase<EventA, CustomCategory> {};

TEST(EventTests, givenEventWithoutCategory_whenGettingCategory_shouldReturnDefault) {
    auto eventA = std::make_shared<EventA>();
    EXPECT_EQ(eventA->getCategoryTypeIndex(), typeid(xvent::DefaultCategory));
}

TEST(EventTests, givenEventWithCustomCategory_whenGettingCategory_shouldReturnProperCategory) {
    auto eventB = std::make_shared<EventB>();
    EXPECT_EQ(eventB->getCategoryTypeIndex(), typeid(CustomCategory));
}

TEST(EventTests, givenEvent_whenCallingOnWithCallback_shouldCallProperCallback) {
    auto eventA = std::make_shared<EventA>();

    int aCalls = 0;
    int bCalls = 0;

    auto dispatch = [&aCalls, &bCalls](std::shared_ptr<xvent::Event> event) {
        event->on<EventA>([&aCalls](std::shared_ptr<EventA>) {
            ++aCalls;
        });

        event->on<EventB>([&bCalls](std::shared_ptr<EventB>) {
            ++bCalls;
        });
    };

    dispatch(eventA);

    EXPECT_EQ(aCalls, 1);
    EXPECT_EQ(bCalls, 0);
}

TEST(EventTests, givenEvent_whenCallingIsWithProperEvent_shouldReturnTrue) {
    auto eventA = std::make_shared<EventA>();

    [](std::shared_ptr<xvent::Event> event) {
        EXPECT_TRUE(event->is<EventA>());
    }(eventA);
}

TEST(EventTests, givenEvent_whenCallingIsWithAnotherEvent_shouldReturnFalse) {
    auto eventA = std::make_shared<EventA>();

    [](std::shared_ptr<xvent::Event> event) {
        EXPECT_FALSE(event->is<EventB>());
    }(eventA);
}

TEST(EventTests, givenEvent_whenCallingAs_shouldCastEvent) {
    auto eventA = std::make_shared<EventA>();

    [](std::shared_ptr<xvent::Event> event) {
        ASSERT_EQ(typeid(*event->as<EventA>()), typeid(EventA));
    }(eventA);
}

TEST(EventTests, givenEvent_whenCallingAsWithWrongType_shouldReturnNullptr) {
    auto eventA = std::make_shared<EventA>();

    [](std::shared_ptr<xvent::Event> event) {
        ASSERT_EQ(event->as<EventB>(), nullptr);
    }(eventA);
}
}
