#include "EventBus.h"
#include <utility>

void EventBus::emit(EventType e) {
  queue_.push(e);
}

void EventBus::subscribe(EventType e, EventCallback cb) {
  listeners[e].push_back(std::move(cb));
}

void EventBus::dispatch() {
  // Drain a local list to avoid holding lock while invoking callbacks
  std::vector<EventType> pending;
  while (!queue_.empty()) {
    pending.push_back(queue_.front());
    queue_.pop();
  }

  for (auto &e : pending) {
    auto it = listeners.find(e);
    if (it != listeners.end()) {
      for (auto &cb : it->second) cb();
    }
  }
}
