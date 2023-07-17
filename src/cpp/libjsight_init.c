#ifdef _GNU_SOURCE
#undef _GNU_SOURCE // ensure we're using XSI strerror_r(), not GNU one
#endif

#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "libjsight_init.h"

#define LIBJSIGHT "/usr/local/lib/libjsight.so"

pthread_once_t libjapi_once = PTHREAD_ONCE_INIT;

static void libjsight_init_imp()
{
	libjsight_handle = dlopen(LIBJSIGHT, RTLD_NOW | RTLD_GLOBAL);
	if (!libjsight_handle) {
		fprintf(stderr, "libjsight_init: dlopen failed for %s: %s\n", LIBJSIGHT, dlerror());
	}
}

int libjsight_init()
{
	int err = pthread_once(&libjapi_once, libjsight_init_imp);
	if (err != 0) {
		char buf[256];
		if (strerror_r(err, buf, sizeof(buf)) == -1) {
			strcpy(buf, "unknown error");
		}
		fprintf(stderr, "libjsight_init: pthread_once failed: %s\n", buf);
		return -1;
	}

	if (!libjsight_handle) {
		return -1;
	}

	return 0;
}
