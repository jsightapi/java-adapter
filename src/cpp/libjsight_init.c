#ifdef _GNU_SOURCE
#undef _GNU_SOURCE // ensure we're using XSI strerror_r(), not GNU one
#endif

#include <dlfcn.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include "libjsight_init.h"

static char libjsight_path[1024];
static void* libjsight_handle = NULL;
pthread_once_t libjsight_once = PTHREAD_ONCE_INIT;

static void libjsight_init_imp()
{
	libjsight_handle = dlopen(libjsight_path, RTLD_NOW | RTLD_GLOBAL);
	if (!libjsight_handle) {
		fprintf(stderr, "libsight_init: dlopen failed for %s: %s\n", libjsight_path, dlerror());
	}
}

int libjsight_init(char * dir)
{
	snprintf(libjsight_path, sizeof(libjsight_path), "%s/libjsight.so", dir);

	int err = pthread_once(&libjsight_once, libjsight_init_imp);
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
