#include "win32_shared_library.hh"

#include <cassert>
#include <cinttypes>

Win32SharedLibrary::Win32SharedLibrary(ILogger &logger)
	: logger_(logger)
	, handle_(nullptr)
{ }

Win32SharedLibrary::~Win32SharedLibrary()
{
	fini();
}

bool Win32SharedLibrary::init(const char *const path)
{
	assert(handle_ == nullptr);
	assert(path != nullptr);

	const HMODULE handle = LoadLibraryA(path);
	if (handle == nullptr) {
		logger_.log(LogLevel::ERROR, "Opening Win32 shared library \"%s\" failed.", path);
		return false;
	}

	handle_ = handle;
	logger_.log(
		LogLevel::INFO,
		"Opened Win32 shared library (%" PRIxPTR ").",
		reinterpret_cast<std::uintptr_t>(handle)
	);
	return true;
}

void Win32SharedLibrary::fini()
{
	if (const HMODULE handle = handle_; handle != nullptr) {
		FreeLibrary(handle);
		logger_.log(
			LogLevel::INFO,
			"Closed Win32 shared library (%" PRIxPTR ").",
			reinterpret_cast<std::uintptr_t>(handle)
		);
	}
}

bool Win32SharedLibrary::try_get_symbol(const char *const symbol_name, void **const out_symbol) const
{
	assert(handle_ != nullptr);
	assert(symbol_name != nullptr);
	assert(out_symbol != nullptr);

	const FARPROC symbol = GetProcAddress(handle_, symbol_name);
	if (symbol == nullptr) {
		logger_.log(LogLevel::ERROR, "Getting symbol \"%s\" failed.", symbol_name);
		return false;
	}

	*reinterpret_cast<FARPROC *>(out_symbol) = symbol;
	return true;
}
