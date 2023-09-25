#include "posix_time.hh"

#include <time.h>

long long posix_time_nsec()
{
	struct timespec time;
	if (clock_gettime(CLOCK_MONOTONIC, &time) == -1) {
		return -1LL;
	}
	return (time.tv_sec * NSEC_PER_SEC) + time.tv_nsec;
}
