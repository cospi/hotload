#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_KEYS_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_KEYS_HH_

#include <X11/Xlib.h>

#include <common/input/key.hh>

Key x11_get_key_event_key(XKeyEvent &key_event);

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_X11_X11_KEYS_HH_
