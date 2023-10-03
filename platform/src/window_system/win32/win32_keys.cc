#include "win32_keys.hh"

#include <cstddef>

struct Win32KeyToKey {
	WPARAM win32_key;
	Key key;
};

static const Win32KeyToKey WIN32_KEY_TO_KEY[] {
	{ VK_UP, Key::UP_ARROW },
	{ VK_DOWN, Key::DOWN_ARROW },
	{ VK_LEFT, Key::LEFT_ARROW },
	{ VK_RIGHT, Key::RIGHT_ARROW },

	{ VK_SPACE, Key::SPACE },

	{ 0x30, Key::ALPHA_0 },
	{ 0x31, Key::ALPHA_1 },
	{ 0x32, Key::ALPHA_2 },
	{ 0x33, Key::ALPHA_3 },
	{ 0x34, Key::ALPHA_4 },
	{ 0x35, Key::ALPHA_5 },
	{ 0x36, Key::ALPHA_6 },
	{ 0x37, Key::ALPHA_7 },
	{ 0x38, Key::ALPHA_8 },
	{ 0x39, Key::ALPHA_9 },

	{ 0x41, Key::A },
	{ 0x42, Key::B },
	{ 0x43, Key::C },
	{ 0x44, Key::D },
	{ 0x45, Key::E },
	{ 0x46, Key::F },
	{ 0x47, Key::G },
	{ 0x48, Key::H },
	{ 0x49, Key::I },
	{ 0x4A, Key::J },
	{ 0x4B, Key::K },
	{ 0x4C, Key::L },
	{ 0x4D, Key::M },
	{ 0x4E, Key::N },
	{ 0x4F, Key::O },
	{ 0x50, Key::P },
	{ 0x51, Key::Q },
	{ 0x52, Key::R },
	{ 0x53, Key::S },
	{ 0x54, Key::T },
	{ 0x55, Key::U },
	{ 0x56, Key::V },
	{ 0x57, Key::W },
	{ 0x58, Key::X },
	{ 0x59, Key::Y },
	{ 0x5A, Key::Z }
};

Key win32_get_key(const WPARAM win32_key)
{
	const std::size_t count = sizeof(WIN32_KEY_TO_KEY) / sizeof(WIN32_KEY_TO_KEY[0]);
	for (std::size_t i = 0; i < count; ++i) {
		const Win32KeyToKey &win32_key_to_key = WIN32_KEY_TO_KEY[i];
		if (win32_key_to_key.win32_key == win32_key) {
			return win32_key_to_key.key;
		}
	}
	return Key::NONE;
}
