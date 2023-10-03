#include "posix_shared_library.hh"

#include <cassert>
#include <cinttypes>

PosixSharedLibrary::PosixSharedLibrary(ILogger &logger)
	: logger_(logger)
	, handle_(nullptr)
{ }

PosixSharedLibrary::~PosixSharedLibrary()
{
	fini();
}

bool PosixSharedLibrary::init(const char *const filename, const int flag)
{
	assert(handle_ == nullptr);

	void *const handle = dlopen(filename, flag);
	if (handle == nullptr) {
		logger_.log(LogLevel::ERR, dlerror());
		return false;
	}

	handle_ = handle;
	logger_.log(
		LogLevel::INFO,
		"Opened POSIX shared library (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(handle)
	);
	return true;
}

void PosixSharedLibrary::fini()
{
	if (void *const handle = handle_; handle != nullptr) {
		dlclose(handle);
		handle_ = nullptr;
		logger_.log(
			LogLevel::INFO,
			"Closed POSIX shared library (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(handle)
		);
	}
}

bool PosixSharedLibrary::try_get_symbol(const char *const symbol_name, void **const out_symbol) const
{
	assert(handle_ != nullptr);
	assert(symbol_name != nullptr);
	assert(out_symbol != nullptr);

	dlerror(); // \note Clear existing errors.
	void *const symbol = dlsym(handle_, symbol_name);
	if (const char *const error = dlerror(); error != nullptr) {
		logger_.log(LogLevel::ERR, error);
		return false;
	}

	*out_symbol = symbol;
	return true;
}
