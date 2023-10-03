#ifndef HOTLOAD_PLATFORM_SHARED_LIBRARY_POSIX_POSIX_SHARED_LIBRARY_HH_
#define HOTLOAD_PLATFORM_SHARED_LIBRARY_POSIX_POSIX_SHARED_LIBRARY_HH_

#include <dlfcn.h> // \note Expose flags: RTLD_LAZY, RTLD_NOW, etc.

#include <common/log/i_logger.hh>

class PosixSharedLibrary final {
public:
	explicit PosixSharedLibrary(ILogger &logger);
	~PosixSharedLibrary();

	PosixSharedLibrary(const PosixSharedLibrary &) = delete;
	PosixSharedLibrary &operator=(const PosixSharedLibrary &) = delete;

	bool init(const char *filename, int flag);
	void fini();

	bool try_get_symbol(const char *symbol_name, void **out_symbol) const;

private:
	ILogger &logger_;
	void *handle_;
};

#endif // HOTLOAD_PLATFORM_SHARED_LIBRARY_POSIX_POSIX_SHARED_LIBRARY_HH_
