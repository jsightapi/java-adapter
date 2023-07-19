#ifndef JSIGHT_JVALIDATION_ERROR_BOX_H
#define JSIGHT_JVALIDATION_ERROR_BOX_H

#include <jni.h>
#include "libjsight.h" // Go library .h file.

class JValidationErrorBox {
    private:
        JNIEnv * env         = NULL;
        jobject jerror       = NULL;
        jstring jreported_by = NULL;
        jstring jtype        = NULL;
        jint    jcode        = -1;
        jstring jtitle       = NULL;
        jstring jdetail      = NULL;

        jobject jposition    = NULL;
        jstring jfilepath    = NULL;
        jobject jline        = NULL;
        jobject jcol         = NULL;
        jobject jindex       = NULL;

        jobjectArray jtrace    = NULL;
        jsize       trace_size = 0;
        jstring * jtrace_items = NULL;

        struct ValidationError * error = NULL;

    public: 
        JValidationErrorBox(JNIEnv * env, jobject jerror);
        ~JValidationErrorBox();
        struct ValidationError * getValidationError();
};

#endif	/* JSIGHT_JVALIDATION_ERROR_BOX_H */