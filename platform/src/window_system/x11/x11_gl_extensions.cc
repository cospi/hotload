#include "x11_gl_extensions.hh"

#include <cassert>
#include <cstring>

bool x11_gl_has_extension(const char *const extensions, const char *const extension)
{
	assert(extensions != nullptr);
	assert(extension != nullptr);
	const std::size_t extension_length = std::strlen(extension);
	assert(extension_length > 0);
	assert(std::strchr(extension, ' ') == nullptr);

	if (*extensions == '\0') {
		return false;
	}

	for (const char *start = extensions;;) {
		const char *const found = std::strstr(start, extension);
		if (found == nullptr) {
			break;
		}

		const char *const remaining = found + extension_length;
		if ((found == start) || (found[-1] == ' ')) {
			const char terminator = *remaining;
			if ((terminator == '\0') || (terminator == ' ')) {
				return true;
			}
		}

		start = remaining;
	}

	return false;
}
