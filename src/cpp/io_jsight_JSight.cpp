#include <jni.h>
#include <dlfcn.h>
#include "io_jsight_JSight.h"
#include "libjsight_init.h"
#include "helper.h"

/* linked go functions pointers */
/*char* (*go_func_JapiValidateInput)        (char*, char*, char*, char*, char*);
int   (*go_func_JapiValidateInputTimeMks) (char*, char*, char*, char*, char*);
char* (*go_func_JapiValidateOutput)       (char*, char*, char*, int, char*, char*);
int   (*go_func_JapiValidateOutputTimeMks)(char*, char*, char*, int, char*, char*); 

char* (*go_func_Stat)();
*/
/*
void io_jsight_JSight_init() {
    libjsight_init();

   	go_func_JapiValidateInput         = (char* (*)(char*, char*, char*, char*, char*     ))dlsym(libjsight_handle, "JapiValidateInput");
   	go_func_JapiValidateInputTimeMks  = (int   (*)(char*, char*, char*, char*, char*     ))dlsym(libjsight_handle, "JapiValidateInputTimeMks");
	go_func_JapiValidateOutput        = (char* (*)(char*, char*, char*, int, char*, char*))dlsym(libjsight_handle, "JapiValidateOutput");
	go_func_JapiValidateOutputTimeMks = (int   (*)(char*, char*, char*, int, char*, char*))dlsym(libjsight_handle, "JapiValidateOutputTimeMks");
	

    go_func_Stat                  = (char* (*)()                                      )dlsym(libjsight_handle, "Stat");

    return JNI_TRUE;
} */

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

    println(env, "===================================================");
    println(env, "Hello, this World!");

	char* api_spec_file_path  = (char*)env->GetStringUTFChars(apiSpecFilePath, &isCopy);
	char* request_method      = (char*)env->GetStringUTFChars(requestMethod  , &isCopy);
	char* request_uri         = (char*)env->GetStringUTFChars(requestUri     , &isCopy);

    struct Header** request_headers = init_headers(env, requestHeaders);
    char * request_body = jbyte_array_to_c_str(env, requestBody);
    struct ValidationError * error = JSightValidateHttpRequest(api_spec_file_path, request_method, request_uri, request_headers, request_body);
    env->ReleaseStringUTFChars(apiSpecFilePath, api_spec_file_path);
    env->ReleaseStringUTFChars(requestMethod  , request_method);
    env->ReleaseStringUTFChars(requestUri     , request_uri);
    free_headers(request_headers);
    if( request_body != NULL ) free(request_body);
    
    if( error == NULL ) return NULL;

    println(env, error->Type);
    println(env, error->Title);
    println(env, error->Detail);
    free(error);
}

/*
// ValidateHttpRequestTimeMks
JNIEXPORT jint JNICALL Java_io_jsight_validator_1demo_JSightMonitor_ValidateHttpRequestTimeMks
  (JNIEnv * env, jclass theClass, jstring jsightFilePath, jstring requestMethod, jstring requestUri, jstring requestPayload, jstring logFilePath) {
    jboolean isCopy;

	char* nativeJsightFilePath  = (char*)env->GetStringUTFChars(jsightFilePath, &isCopy);
	char* nativeRequestMethod   = (char*)env->GetStringUTFChars(requestMethod , &isCopy);
	char* nativeRequestUri      = (char*)env->GetStringUTFChars(requestUri    , &isCopy); 
	char* nativeRequestPayload  = (char*)env->GetStringUTFChars(requestPayload, &isCopy);
	char* nativeLogFilePath     = (char*)env->GetStringUTFChars(logFilePath   , &isCopy);

    int timeMks = go_func_JapiValidateInputTimeMks(nativeJsightFilePath, nativeRequestMethod, nativeRequestUri, nativeRequestPayload, nativeLogFilePath);

    env->ReleaseStringUTFChars(jsightFilePath, nativeJsightFilePath);
    env->ReleaseStringUTFChars(requestMethod , nativeRequestMethod);
    env->ReleaseStringUTFChars(requestUri    , nativeRequestUri);
    env->ReleaseStringUTFChars(requestPayload, nativeRequestPayload);
    env->ReleaseStringUTFChars(logFilePath   , nativeLogFilePath);

    return (jint) timeMks;
}

// ValidateHttpResponse
JNIEXPORT jstring JNICALL Java_io_jsight_validator_1demo_JSightMonitor_ValidateHttpResponse
  (JNIEnv * env, jclass theClass, jstring jsightFilePath, jstring requestMethod, jstring requestUri, jint responseStatusCode, jstring responsePayload, jstring logFilePath) {

    jboolean isCopy;

	char* nativeJsightFilePath  = (char*)env->GetStringUTFChars(jsightFilePath , &isCopy);
	char* nativeRequestMethod   = (char*)env->GetStringUTFChars(requestMethod  , &isCopy);
	char* nativeRequestUri      = (char*)env->GetStringUTFChars(requestUri     , &isCopy); 
	char* nativeResponsePayload = (char*)env->GetStringUTFChars(responsePayload, &isCopy);
	char* nativeLogFilePath     = (char*)env->GetStringUTFChars(logFilePath    , &isCopy);

    char* error = go_func_JapiValidateOutput(nativeJsightFilePath, nativeRequestMethod, nativeRequestUri, (int)responseStatusCode, nativeResponsePayload, nativeLogFilePath);

    env->ReleaseStringUTFChars(jsightFilePath , nativeJsightFilePath);
    env->ReleaseStringUTFChars(requestMethod  , nativeRequestMethod);
    env->ReleaseStringUTFChars(requestUri     , nativeRequestUri);
    env->ReleaseStringUTFChars(responsePayload, nativeResponsePayload);
    env->ReleaseStringUTFChars(logFilePath    , nativeLogFilePath);

    return env->NewStringUTF(error);
}

// ValidateHttpResponseTimeMks
JNIEXPORT jint JNICALL Java_io_jsight_validator_1demo_JSightMonitor_ValidateHttpResponseTimeMks
  (JNIEnv * env, jclass theClass, jstring jsightFilePath, jstring requestMethod, jstring requestUri, jint responseStatusCode, jstring responsePayload, jstring logFilePath) {

    jboolean isCopy;

	char* nativeJsightFilePath  = (char*)env->GetStringUTFChars(jsightFilePath , &isCopy);
	char* nativeRequestMethod   = (char*)env->GetStringUTFChars(requestMethod  , &isCopy);
	char* nativeRequestUri      = (char*)env->GetStringUTFChars(requestUri     , &isCopy); 
	char* nativeResponsePayload = (char*)env->GetStringUTFChars(responsePayload, &isCopy);
	char* nativeLogFilePath     = (char*)env->GetStringUTFChars(logFilePath    , &isCopy);

    int timeMks = go_func_JapiValidateOutputTimeMks(nativeJsightFilePath, nativeRequestMethod, nativeRequestUri, (int)responseStatusCode, nativeResponsePayload, nativeLogFilePath);

    env->ReleaseStringUTFChars(jsightFilePath , nativeJsightFilePath);
    env->ReleaseStringUTFChars(requestMethod  , nativeRequestMethod);
    env->ReleaseStringUTFChars(requestUri     , nativeRequestUri);
    env->ReleaseStringUTFChars(responsePayload, nativeResponsePayload);
    env->ReleaseStringUTFChars(logFilePath    , nativeLogFilePath);

    return (jint)timeMks;
}
*/