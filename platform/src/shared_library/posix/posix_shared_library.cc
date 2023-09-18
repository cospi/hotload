#include "posix_shared_library.hh"

#include <cassert>
#include <cinttypes>
#include <cstdio>

PosixSharedLibrary::PosixSharedLibrary()
	: handle_(nullptr)
{ }

PosixSharedLibrary::~PosixSharedLibrary()
{
	if (void *const handle = handle_; handle != nullptr) {
		dlclose(handle);
		std::fprintf(
			stdout,
			"Closed POSIX shared library (%" PRIxPTR ").\n",
			reinterpret_cast<std::uintptr_t>(handle)
		);
	}
}

bool PosixSharedLibrary::init(const char *const filename, const int flag)
{
	assert(handle_ == nullptr);

	void *const handle = dlopen(filename, flag);
	if (handle == nullptr) {
		std::fputs(dlerror(), stderr);
		std::fputc('\n', stderr);
		return false;
	}

	handle_ = handle;
	std::fprintf(
		stdout,
		"Opened POSIX shared library (%" PRIxPTR ").\n",
		reinterpret_cast<std::uintptr_t>(handle)
	);
	return true;
}

bool PosixSharedLibrary::try_get_symbol(const char *const symbol_name, void **const out_symbol) const
{
	assert(handle_ != nullptr);
	assert(symbol_name != nullptr);
	assert(out_symbol != nullptr);

	dlerror(); // \note Clear existing errors.
	void *const symbol = dlsym(handle_, symbol_name);
	if (const char *const error = dlerror(); error != nullptr) {
		std::fputs(error, stderr);
		std::fputc('\n', stderr);
		return false;
	}

	*out_symbol = symbol;
	return true;
}
