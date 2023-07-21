#include <jni.h>
#include <list>
#include <cstring>
#include "helper.h"

struct Header ** init_headers(JNIEnv * env, jobject jheaders) {
	struct Header ** headers = NULL;
	if( jheaders != NULL ) {

		std::list<struct Header *> headers_list = {};

		// java: Set<String> jkey_set = jheaders.keySet();
		jclass map_class = env->GetObjectClass(jheaders);
		jmethodID map_keySet_mid = env->GetMethodID(map_class, "keySet", "()Ljava/util/Set;");
		jobject jkey_set = env->CallObjectMethod(jheaders, map_keySet_mid);

		// java: Object[] jkeys = jkey_set.toArray()
		jclass set_class = env->GetObjectClass(jkey_set);
		jmethodID set_toArray_mid = env->GetMethodID(set_class, "toArray", "()[Ljava/lang/Object;");
		jobjectArray jkeys = static_cast<jobjectArray>(env->CallObjectMethod(jkey_set, set_toArray_mid));

		jsize keys_count = env->GetArrayLength(jkeys);

		// java: for( jkey: jkeys )
		for(int i = 0; i < keys_count; i++) {
			jstring jkey = static_cast<jstring>(env->GetObjectArrayElement(jkeys, i));

			// java: List<String> jvalues_list = jheaders.get(jkey)
			jmethodID map_get_mid = env->GetMethodID(map_class, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
			jobject jvalues_list = env->CallObjectMethod(jheaders, map_get_mid, jkey);

			// java: String[] jvalues = jvalues_list.toArray();
			jclass list_class = env->GetObjectClass(jvalues_list);
			jmethodID list_toArray_mid = env->GetMethodID(list_class, "toArray", "()[Ljava/lang/Object;");
			jobjectArray jvalues = static_cast<jobjectArray>(env->CallObjectMethod(jvalues_list, list_toArray_mid));

			jsize values_count = env->GetArrayLength(jvalues);

			for(int i = 0; i < values_count; i++) {
				jstring jvalue = static_cast<jstring>(env->GetObjectArrayElement(jvalues, i));
				jboolean isCopy;
				struct Header * header = new Header();
				header->Name  = (char*)env->GetStringUTFChars(jkey, &isCopy);
				header->Value = (char*)env->GetStringUTFChars(jvalue, &isCopy);

				headers_list.push_back(header);			
			}
		}

		// char * buf = new char[1000];
		// sprintf(buf, "HEADERS LIST LENGTH: %d\n", headers_list.size());
		// println(env, buf);
		// free(buf);

		headers = (struct Header **)malloc(sizeof(struct Header *) * (headers_list.size() + 1)); // Plus one place for NULL termination.
		int i = 0;
		for(struct Header * h : headers_list) {
			headers[i] = h;
			// println(env, h->Name);
			// println(env, h->Value);
			i++;
		}
		headers[i] = NULL; // Last element must be NULL
	}
	
	return headers;
}

void free_headers(struct Header ** headers) {
	if( headers != NULL ) {
		int i = 0;
		while(headers[i] != NULL) {
			free(headers[i]);
			i++;
		}
		free(headers);
	}
}

char * jbyte_array_to_c_str(JNIEnv * env, jbyteArray jbytes) {
	if( jbytes == NULL ) return NULL;
	jsize num_bytes = env->GetArrayLength(jbytes);
	char * buffer = (char *) malloc(num_bytes + 1);
	jboolean isCopy;
	jbyte * elements = env->GetByteArrayElements(jbytes, &isCopy);
	memcpy(buffer, elements, num_bytes);
	buffer[num_bytes] = 0; // the last element must be NULL
	env->ReleaseByteArrayElements(jbytes, elements, JNI_ABORT);
	return buffer;
}

char * init_body(JNIEnv * env, jbyteArray body) {
	return jbyte_array_to_c_str(env, body);
}

void free_body(char * body) {
	if( body != NULL ) free(body);
}

jobject new_jValidationError(JNIEnv * env, struct ValidationError * error) {
	if( error == NULL ) return NULL;

	jclass integer_class = env->FindClass("java/lang/Integer");
	jmethodID integer_ctor_mid = env->GetMethodID(integer_class, "<init>", "(I)V");

	// making ErrorPosition
	jclass error_position_class = env->FindClass("io/jsight/ErrorPosition");
	jmethodID error_position_ctor_mid = env->GetMethodID(
		error_position_class, "<init>", "(Ljava/lang/String;Ljava/lang/Integer;Ljava/lang/Integer;Ljava/lang/Integer;)V");

	jobject jErrorPosition = NULL;
	struct ErrorPosition * position = error->Position;
	if( position != NULL ) {
		jobject jline = NULL;
		if( position->Line != NULL ) {
			jline = env->NewObject(integer_class, integer_ctor_mid, *position->Line);
		}

		jobject jcol = NULL;
		if( position->Col != NULL ) {
			jcol = env->NewObject(integer_class, integer_ctor_mid, *position->Col);
		}

		jobject jindex = NULL;
		if( position->Index != NULL ) {
			jindex = env->NewObject(integer_class, integer_ctor_mid, *position->Index);
		}

		jErrorPosition = env->NewObject(error_position_class, error_position_ctor_mid,
			env->NewStringUTF(error->Position->Filepath), jline, jcol, jindex);
	}

	jclass validation_error_class = env->FindClass("io/jsight/ValidationError");
	jmethodID validation_error_ctor_mid = env->GetMethodID(
		validation_error_class, "<init>", "(Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;Lio/jsight/ErrorPosition;[Ljava/lang/String;)V");

	jobjectArray jtrace = NULL;

	if( error->Trace != NULL ) {
		int i = 0;
		while(error->Trace[i]) {
			i++;
		}
		int trace_size = i;
		jclass string_class = env->FindClass("java/lang/String");
		
		jtrace = env->NewObjectArray(trace_size, string_class, env->NewStringUTF(""));

		for(i=0; i < trace_size; i++) {
			env->SetObjectArrayElement(jtrace, i, env->NewStringUTF(error->Trace[i]));
		}
	}

	jobject jValidationError = env->NewObject(validation_error_class, validation_error_ctor_mid,
		env->NewStringUTF(error->ReportedBy),
		env->NewStringUTF(error->Type),
		error->Code,
		env->NewStringUTF(error->Title),
		env->NewStringUTF(error->Detail),
		jErrorPosition,
		jtrace
	);
	
	return jValidationError;
}

char * empty_string() {
	char * str = (char *)malloc(sizeof(char));
	str[0] = '\0';
	return str;
}

void println(JNIEnv * env, const char * cstr) {
    jstring str = env->NewStringUTF(cstr);
    println(env, str);
}

void println(JNIEnv * env, jstring str) {
    jclass system_class = env->FindClass("java/lang/System");
    jfieldID fid = env->GetStaticFieldID(system_class, "out", "Ljava/io/PrintStream;");
	// java: out = System.out;
    jobject out = env->GetStaticObjectField(system_class, fid);
    jclass out_class = env->GetObjectClass(out);
    jmethodID mid = env->GetMethodID(out_class, "println", "(Ljava/lang/String;)V");
	// java: out.println( stat );
    env->CallVoidMethod(out, mid, str);
}