#ifndef HOTLOAD_PLATFORM_SHARED_LIBRARY_WIN32_WIN32_SHARED_LIBRARY_HH_
#define HOTLOAD_PLATFORM_SHARED_LIBRARY_WIN32_WIN32_SHARED_LIBRARY_HH_

#include <libloaderapi.h>

#include <common/log/i_logger.hh>

class Win32SharedLibrary final {
public:
	explicit Win32SharedLibrary(ILogger &logger);
	~Win32SharedLibrary();

	Win32SharedLibrary(const Win32SharedLibrary &) = delete;
	Win32SharedLibrary &operator=(const Win32SharedLibrary &) = delete;

	bool init(const char *path);
	void fini();

	bool try_get_symbol(const char *symbol_name, void **out_symbol) const;

private:
	ILogger &logger_;
	HMODULE handle_;
};

#endif // HOTLOAD_PLATFORM_SHARED_LIBRARY_WIN32_WIN32_SHARED_LIBRARY_HH_
