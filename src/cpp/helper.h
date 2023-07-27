#ifndef JSIGHT_HELPER_H
#define JSIGHT_HELPER_H

#include <jni.h>
#include "jvalidationerrorbox.hpp"

struct Header ** init_headers(JNIEnv * env, jobject jheaders);
void free_headers(struct Header ** headers);
char * init_body(JNIEnv * env, jbyteArray jbytes);
void free_body(char * body);

jobject new_jValidationError(JNIEnv * env, struct ValidationError * error);

struct ValidationError * init_validation_error(JNIEnv * env, jobject jerror);
void free_ValidationError(struct ValidationError * error);

char * empty_string();

void println(JNIEnv * env, const char * str);
void println(JNIEnv * env, jstring str);

#endif	/* JSIGHT_HELPER_H */
