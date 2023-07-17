#include <jni.h>
#include <dlfcn.h>
#include "io_jsight_JSight.h"
#include "libjsight_init.h"
#include "libjsight.h" // Go library .h file.

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

JNIEXPORT jstring JNICALL Java_io_jsight_validator_1demo_JSightMonitor_Stat (JNIEnv * env, jclass theClass) {
    libjsight_init();
    return env->NewStringUTF(JSightStat());
}

/*
// ValidateHttpRequest
JNIEXPORT jstring JNICALL Java_io_jsight_validator_1demo_JSightMonitor_ValidateHttpRequest
  (JNIEnv * env, jclass theClass, jstring jsightFilePath, jstring requestMethod, jstring requestUri, jstring requestPayload, jstring logFilePath) {

    jboolean isCopy;

	char* nativeJsightFilePath  = (char*)env->GetStringUTFChars(jsightFilePath, &isCopy);
	char* nativeRequestMethod   = (char*)env->GetStringUTFChars(requestMethod , &isCopy);
	char* nativeRequestUri      = (char*)env->GetStringUTFChars(requestUri    , &isCopy); 
	char* nativeRequestPayload  = (char*)env->GetStringUTFChars(requestPayload, &isCopy);
	char* nativeLogFilePath     = (char*)env->GetStringUTFChars(logFilePath   , &isCopy);

    char* error = go_func_JapiValidateInput(nativeJsightFilePath, nativeRequestMethod, nativeRequestUri, nativeRequestPayload, nativeLogFilePath);

    env->ReleaseStringUTFChars(jsightFilePath, nativeJsightFilePath);
    env->ReleaseStringUTFChars(requestMethod , nativeRequestMethod);
    env->ReleaseStringUTFChars(requestUri    , nativeRequestUri);
    env->ReleaseStringUTFChars(requestPayload, nativeRequestPayload);
    env->ReleaseStringUTFChars(logFilePath   , nativeLogFilePath);

    return env->NewStringUTF(error);
}

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