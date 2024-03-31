// Copyright 2024 Filatov Maxim

#include <gtest/gtest.h>
#include <cstdint>
#include "TimedDoor.h"

TEST(TimedDoorTest, TestInitialState) {
    TimedDoor door(5);
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimedDoorTest, TestDoorOpens) {
    TimedDoor door(5);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, TestDoorCloses) {
    TimedDoor door(5);
    door.unlock();
    door.lock();
    EXPECT_FALSE(door.isDoorOpened());
}

TEST(TimedDoorTest, TestExceptionThrownWhenDoorStaysOpen) {
    TimedDoor door(0);
    EXPECT_THROW(door.unlock(), std::runtime_error);
}

TEST(TimedDoorTest, TestNoExceptionWhenDoorClosesInTime) {
    TimedDoor door(5);
    door.unlock();
    door.lock();
    EXPECT_NO_THROW(door.throwState());
}

TEST(TimedDoorTest, TestTimerSetCorrectly) {
    TimedDoor door(5);
    EXPECT_EQ(door.getTimeOut(), 5);
}

TEST(TimedDoorTest, TestDoorStaysOpenWithoutTimer) {
    TimedDoor door(5);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}

TEST(TimedDoorTest, TestDoorClosesAfterTimeout) {
    TimedDoor door(0);
    door.unlock();
    EXPECT_THROW(door.throwState(), std::runtime_error);
}

TEST(TimedDoorTest, TestDoorStaysOpenIfTimeNotElapsed) {
    TimedDoor door(5);
    door.unlock();
    EXPECT_TRUE(door.isDoorOpened());
}
