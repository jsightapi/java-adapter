#ifndef JSIGHT_HELPER_H
#define JSIGHT_HELPER_H

#include <jni.h>
#include "libjsight.h" // Go library .h file.

struct Header ** init_headers(JNIEnv * env, jobject jheaders);
void free_headers(struct Header ** headers);
char * init_body(JNIEnv * env, jbyteArray jbytes);
void free_body(char * body);

jobject new_jValidationError(JNIEnv * env, struct ValidationError * error);


/*void set_return_value_error(zval * return_value, struct ValidationError * error);

struct ValidationError * init_ValidationError(zval * error_zv);
void free_ValidationError(struct ValidationError * error);
*/ 

void println(JNIEnv * env, const char * str);
void println(JNIEnv * env, jstring str);

#endif	/* JSIGHT_HELPER_H */
