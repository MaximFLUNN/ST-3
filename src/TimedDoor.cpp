// Copyright 2024 Filatov Maxim

#include "TimedDoor.h"
#include <stdexcept>
#include <thread>
#include <chrono>

DoorTimerAdapter::DoorTimerAdapter(TimedDoor& door) : door(door) {}

void DoorTimerAdapter::Timeout() {
  if (door.isDoorOpened()) {
    door.throwState();
  }
}

TimedDoor::TimedDoor(int timeout) : iTimeout(timeout), isOpened(false) {
  adapter = new DoorTimerAdapter(*this);
}

bool TimedDoor::isDoorOpened() {
  return isOpened;
}

void TimedDoor::unlock() {
  isOpened = true;
  Timer timer;
  timer.tregister(iTimeout, adapter);
}

void TimedDoor::lock() {
  isOpened = false;
}

int TimedDoor::getTimeOut() {
  return iTimeout;
}

void TimedDoor::throwState() {
  throw std::runtime_error("Door is still open!");
}

void Timer::tregister(int timeout, TimerClient* client) {
  sleep(timeout);
  client->Timeout();
}

void Timer::sleep(int timeout) {
  std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
