#ifndef JSIGHT_HELPER_H
#define JSIGHT_HELPER_H

#include <jni.h>
#include "libjsight.h" // Go library .h file.

struct Header ** init_Headers(JNIEnv * env, jobject jheaders);
// void free_Headers(struct Header ** headers);

/*void set_return_value_error(zval * return_value, struct ValidationError * error);

struct ValidationError * init_ValidationError(zval * error_zv);
void free_ValidationError(struct ValidationError * error);
*/ 

#endif	/* JSIGHT_HELPER_H */
