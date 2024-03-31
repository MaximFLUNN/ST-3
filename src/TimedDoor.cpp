// Copyright 2024 Filatov Maxim

#include "TimedDoor.h"
#include <stdexcept>
#include <thread> // NOLINT [build/c++11]
#include <chrono> // NOLINT [build/c++11]
#include <condition_variable> // NOLINT [build/c++11]
#include <mutex> // NOLINT [build/c++11]

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
  std::condition_variable cv;
  std::mutex m;
  Timer timer;
  timer.tregister(iTimeout, adapter, cv, m);

  std::unique_lock<std::mutex> lock(m);
  cv.wait(lock);
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

void Timer::tregister(int timeout, TimerClient* client, std::condition_variable& cv, std::mutex& m) {
  sleep(timeout);
  client->Timeout();
  cv.notify_all();
}

void Timer::sleep(int timeout) {
  std::this_thread::sleep_for(std::chrono::seconds(timeout));
}
