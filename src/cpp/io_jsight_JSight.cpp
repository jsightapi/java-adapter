#include <jni.h>
#include <dlfcn.h>
#include "io_jsight_JSight.h"
#include "libjsight_init.h"
#include "helper.h"

/*
 * Class:     io_jsight_JSight
 * Method:    initSharedLibrary
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT void JNICALL Java_io_jsight_JSight_initSharedLibrary
(JNIEnv * env, jclass theClass, jstring dirPath) {
    jboolean isCopy;

	char * dir_path  = (char*)env->GetStringUTFChars(dirPath, &isCopy);
    libjsight_init(dir_path);
    env->ReleaseStringUTFChars(dirPath, dir_path);
}

/*
 * Class:     io_jsight_JSight
 * Method:    ClearCache
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_io_jsight_JSight_ClearCache(JNIEnv *, jclass) {
    JSightClearCache();
}

/*
 * Class:     io_jsight_JSight
 * Method:    Stat
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_jsight_JSight_Stat (JNIEnv * env, jclass theClass) {
    return env->NewStringUTF(JSightStat());
}

/*
 * Class:     io_jsight_JSight
 * Method:    ValidateHttpRequest
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/util/Map;Ljava/lang/String;)Lio/jsight/ValidationError;
 */
JNIEXPORT jobject JNICALL Java_io_jsight_JSight_ValidateHttpRequest
  (JNIEnv * env, jclass theClass, jstring apiSpecFilePath, jstring requestMethod, jstring requestUri, jobject requestHeaders, jbyteArray requestBody)
{
    jboolean isCopy;

	char* api_spec_file_path  = (char*)env->GetStringUTFChars(apiSpecFilePath, &isCopy);
	char* request_method      = (char*)env->GetStringUTFChars(requestMethod  , &isCopy);
	char* request_uri         = (char*)env->GetStringUTFChars(requestUri     , &isCopy);

    struct Header** request_headers = init_headers(env, requestHeaders);
    char * request_body = init_body(env, requestBody);
    struct ValidationError * error = JSightValidateHttpRequest(api_spec_file_path, request_method, request_uri, request_headers, request_body);
    env->ReleaseStringUTFChars(apiSpecFilePath, api_spec_file_path);
    env->ReleaseStringUTFChars(requestMethod  , request_method);
    env->ReleaseStringUTFChars(requestUri     , request_uri);
    free_headers(request_headers);
    free_body(request_body);
     
    if( error == NULL ) return NULL;

    // println(env, error->Title);
    jobject jerror = new_jValidationError(env, error);
    
    freeValidationError(error);

    return jerror;
}

/*
 * Class:     io_jsight_JSight
 * Method:    ValidateHttpResponse
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/util/Map;[B)Lio/jsight/ValidationError;
 */
JNIEXPORT jobject JNICALL Java_io_jsight_JSight_ValidateHttpResponse
  (JNIEnv * env, jclass, jstring apiSpecFilePath, jstring requestMethod, jstring requestUri, jint responseStatusCode, jobject responseHeaders, jbyteArray responseBody) 
{
    jboolean isCopy;

	char* api_spec_file_path  = (char*)env->GetStringUTFChars(apiSpecFilePath, &isCopy);
	char* request_method      = (char*)env->GetStringUTFChars(requestMethod  , &isCopy);
	char* request_uri         = (char*)env->GetStringUTFChars(requestUri     , &isCopy);

    struct Header** response_headers = init_headers(env, responseHeaders);
    char * response_body = init_body(env, responseBody);
    struct ValidationError * error = JSightValidateHttpResponse(api_spec_file_path, request_method, request_uri, responseStatusCode, response_headers, response_body);
    env->ReleaseStringUTFChars(apiSpecFilePath, api_spec_file_path);
    env->ReleaseStringUTFChars(requestMethod  , request_method);
    env->ReleaseStringUTFChars(requestUri     , request_uri);
    free_headers(response_headers);
    free_body(response_body);
     
    if( error == NULL ) return NULL;

    // println(env, error->Title);
    jobject jerror = new_jValidationError(env, error);
    
    freeValidationError(error);

    return jerror;
}

/*
 * Class:     io_jsight_JSight
 * Method:    SerializeError
 * Signature: (Ljava/lang/String;Lio/jsight/ValidationError;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_io_jsight_JSight_SerializeError
  (JNIEnv * env, jclass cls, jstring jformat, jobject jerror)
{
    // We use JValidationErrorBox here in order to release all the resources,
    // thanks to its destructor
    JValidationErrorBox * jerror_box = new JValidationErrorBox( env, jerror );
    jboolean isCopy;
	char * format  = (char*)env->GetStringUTFChars(jformat, &isCopy);

    char * json = JSightSerializeError(format, jerror_box->getValidationError());
    jstring jjson = env->NewStringUTF(json);

    // free resources
    env->ReleaseStringUTFChars(jformat, format);
    delete(jerror_box);
    free(json);

    return jjson;
}
