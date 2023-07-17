/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_jsight_JSight */

#ifndef _Included_io_jsight_JSight
#define _Included_io_jsight_JSight
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     io_jsight_JSight
 * Method:    Init
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_io_jsight_JSight_Init
  (JNIEnv *, jclass);

/*
 * Class:     io_jsight_JSight
 * Method:    Stat
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_jsight_JSight_Stat
  (JNIEnv *, jclass);

/*
 * Class:     io_jsight_JSight
 * Method:    ClearCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_jsight_JSight_ClearCache
  (JNIEnv *, jclass);

/*
 * Class:     io_jsight_JSight
 * Method:    ValidateHttpRequest
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;Ljava/lang/String;)Lio/jsight/ValidationError;
 */
JNIEXPORT jobject JNICALL Java_io_jsight_JSight_ValidateHttpRequest
  (JNIEnv *, jclass, jstring, jstring, jstring, jobject, jstring);

/*
 * Class:     io_jsight_JSight
 * Method:    ValidateHttpResponse
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/util/Map;Ljava/lang/String;)Lio/jsight/ValidationError;
 */
JNIEXPORT jobject JNICALL Java_io_jsight_JSight_ValidateHttpResponse
  (JNIEnv *, jclass, jstring, jstring, jstring, jint, jobject, jstring);

/*
 * Class:     io_jsight_JSight
 * Method:    SerializeError
 * Signature: (Ljava/lang/String;Lio/jsight/ValidationError;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_jsight_JSight_SerializeError
  (JNIEnv *, jclass, jstring, jobject);

#ifdef __cplusplus
}
#endif
#endif