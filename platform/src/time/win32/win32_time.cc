#include "win32_time.hh"

LONGLONG win32_performance_frequency()
{
	LARGE_INTEGER result;
	if (QueryPerformanceFrequency(&result) != 0) {
		return result.QuadPart;
	}
	return -1LL;
}

LONGLONG win32_performance_counter()
{
	LARGE_INTEGER result;
	if (QueryPerformanceCounter(&result) != 0) {
		return result.QuadPart;
	}
	return -1LL;
}
