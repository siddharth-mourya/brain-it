#pragma once
#include <functional>
#include <vector>
#include <map>
#include <queue>

enum class EventType {
  NONE = 0,
  TOUCH_SINGLE,
  TOUCH_DOUBLE,
  BOOT_COMPLETE,
  WIFI_CONNECTED,
  WIFI_DISCONNECTED,
};

using EventCallback = std::function<void()>;

class EventBus {
public:
  void emit(EventType e);
  void subscribe(EventType e, EventCallback cb);
  void dispatch();
private:
  std::map<EventType, std::vector<EventCallback>> listeners;
  std::queue<EventType> queue_;
};
