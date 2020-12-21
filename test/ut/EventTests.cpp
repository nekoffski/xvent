#include <gtest/gtest.h>

#include <xvent/Category.h>
#include <xvent/Event.h>

namespace {

struct CustomCategory {};

struct EventA : xvent::EventBase<EventA> {};

struct EventB : xvent::EventBase<EventA, CustomCategory> {};

TEST(EventTests, givenEventWithoutCategory_whenGettingCategory_shouldReturnDefault) {
    auto eventA = std::make_shared<EventA>();
    EXPECT_EQ(eventA->getCategoryTypeIndex(), typeid(xvent::DefaultCategory));
}

TEST(EventTests, giventEventWithCustomCategory_whenGettingCategory_shouldReturnProperCategory) {
    auto eventB = std::make_shared<EventB>();
    EXPECT_EQ(eventB->getCategoryTypeIndex(), typeid(CustomCategory));
}
}
