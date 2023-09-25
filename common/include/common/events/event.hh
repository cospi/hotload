#ifndef HOTLOAD_COMMON_EVENTS_EVENT_HH_
#define HOTLOAD_COMMON_EVENTS_EVENT_HH_

#include "../input/key.hh"

enum class EventType {
	KEY_DOWN,
	KEY_UP
};

struct KeyDownEvent {
	KeyDownEvent() = default;

	explicit KeyDownEvent(const Key in_key)
		: key(in_key)
	{ }

	Key key;
};

struct KeyUpEvent {
	KeyUpEvent() = default;

	explicit KeyUpEvent(const Key in_key)
		: key(in_key)
	{ }

	Key key;
};

struct Event {
	EventType type;
	union {
		KeyDownEvent key_down;
		KeyUpEvent key_up;
	} event;
};

#endif // HOTLOAD_COMMON_EVENTS_EVENT_HH_
