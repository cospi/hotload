#ifndef HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_KEYS_HH_
#define HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_KEYS_HH_

#include <Windows.h>

#include <common/input/key.hh>

Key win32_get_key(WPARAM win32_key);

#endif // HOTLOAD_PLATFORM_WINDOW_SYSTEM_WIN32_WIN32_KEYS_HH_