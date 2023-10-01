#include "x11_keys.hh"

#include <X11/keysym.h>

#include <cstddef>

struct KeySymToKey {
	KeySym key_sym;
	Key key;
};

static const KeySymToKey KEY_SYM_TO_KEY[] {
	{ NoSymbol, Key::NONE },

	{ XK_Up, Key::UP_ARROW },
	{ XK_Down, Key::DOWN_ARROW },
	{ XK_Left, Key::LEFT_ARROW },
	{ XK_Right, Key::RIGHT_ARROW },

	{ XK_space, Key::SPACE },

	{ XK_0, Key::ALPHA_0 },
	{ XK_1, Key::ALPHA_1 },
	{ XK_2, Key::ALPHA_2 },
	{ XK_3, Key::ALPHA_3 },
	{ XK_4, Key::ALPHA_4 },
	{ XK_5, Key::ALPHA_5 },
	{ XK_6, Key::ALPHA_6 },
	{ XK_7, Key::ALPHA_7 },
	{ XK_8, Key::ALPHA_8 },
	{ XK_9, Key::ALPHA_9 },

	{ XK_a, Key::A },
	{ XK_b, Key::B },
	{ XK_c, Key::C },
	{ XK_d, Key::D },
	{ XK_e, Key::E },
	{ XK_f, Key::F },
	{ XK_g, Key::G },
	{ XK_h, Key::H },
	{ XK_i, Key::I },
	{ XK_j, Key::J },
	{ XK_k, Key::K },
	{ XK_l, Key::L },
	{ XK_m, Key::M },
	{ XK_n, Key::N },
	{ XK_o, Key::O },
	{ XK_p, Key::P },
	{ XK_q, Key::Q },
	{ XK_r, Key::R },
	{ XK_s, Key::S },
	{ XK_t, Key::T },
	{ XK_u, Key::U },
	{ XK_v, Key::V },
	{ XK_w, Key::W },
	{ XK_x, Key::X },
	{ XK_y, Key::Y },
	{ XK_z, Key::Z }
};

Key x11_get_key_event_key(XKeyEvent &key_event)
{
	const KeySym key_sym = XLookupKeysym(&key_event, 0);
	const std::size_t count = sizeof(KEY_SYM_TO_KEY) / sizeof(KEY_SYM_TO_KEY[0]);
	for (std::size_t i = 0; i < count; ++i) {
		const KeySymToKey &key_sym_to_key = KEY_SYM_TO_KEY[i];
		if (key_sym_to_key.key_sym == key_sym) {
			return key_sym_to_key.key;
		}
	}
	return Key::NONE;
}
