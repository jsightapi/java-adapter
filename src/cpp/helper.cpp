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

/*void set_return_value_error(zval * return_value, struct ValidationError * error) {
	array_init_size(return_value, 7);

	add_assoc_string(return_value, "reportedBy", error->ReportedBy);
	add_assoc_string(return_value, "type"      , error->Type      );
	add_assoc_long  (return_value, "code"      , error->Code      );
	add_assoc_string(return_value, "title"     , error->Title     );
	add_assoc_string(return_value, "detail"    , error->Detail    );

	if(error->Position != NULL) {
		zval pos;
		array_init_size(&pos, 4);
		if(error->Position->Filepath != NULL) {
			add_assoc_string(&pos, "filepath", error->Position->Filepath);
		}
		if(error->Position->Index != NULL) {
			add_assoc_long(&pos, "index", * error->Position->Index);
		}
		if(error->Position->Line != NULL) {
			add_assoc_long(&pos, "line", * error->Position->Line);
		}
		if(error->Position->Col != NULL) {
			add_assoc_long(&pos, "col", * error->Position->Col);
		}
		add_assoc_zval(return_value, "position", &pos);
	}

	if( error->Trace != NULL ) {
		zval trace;
		array_init(&trace);
		int i = 0;
		while(error->Trace[i] != NULL) {
			add_next_index_string(&trace, error->Trace[i]);
			i++;
		}
		add_assoc_zval(return_value, "trace", &trace);
	}
}

struct ValidationError * init_ValidationError(zval * error_zv) {
	struct ValidationError * error = malloc(sizeof(struct ValidationError));

  error->ReportedBy = NULL;
	error->Type       = NULL;
	error->Code       = -1;
	error->Title      = NULL;
	error->Detail     = NULL;
	error->Position   = NULL;
	error->Trace      = NULL;

	HashTable* error_ht = Z_ARRVAL_P(error_zv);

	if( zend_hash_str_exists(error_ht, "reportedBy", sizeof("reportedBy")-1) ) {
		zval* type_zv = zend_hash_str_find(error_ht, "reportedBy", sizeof("reportedBy")-1);
		error->ReportedBy = Z_STRVAL_P(type_zv);
	}

	if( zend_hash_str_exists(error_ht, "type", sizeof("type")-1) ) {
		zval* type_zv = zend_hash_str_find(error_ht, "type", sizeof("type")-1);
		error->Type = Z_STRVAL_P(type_zv);
	}

	if( zend_hash_str_exists(error_ht, "code", sizeof("code")-1) ) {
		zval* code_zv = zend_hash_str_find(error_ht, "code", sizeof("code")-1);
		error->Code = Z_LVAL_P(code_zv);
	}

	if( zend_hash_str_exists(error_ht, "title", sizeof("title")-1) ) {
		zval* title_zv = zend_hash_str_find(error_ht, "title", sizeof("title")-1);
		error->Title = Z_STRVAL_P(title_zv);
	}

	if( zend_hash_str_exists(error_ht, "detail", sizeof("detail")-1) ) {
		zval* detail_zv = zend_hash_str_find(error_ht, "detail", sizeof("detail")-1);
		error->Detail = Z_STRVAL_P(detail_zv);
	}

	if( zend_hash_str_exists(error_ht, "position", sizeof("position")-1) ) {
		zval* position_zv = zend_hash_str_find(error_ht, "position", sizeof("position")-1);
		HashTable* position_ht = Z_ARRVAL_P(position_zv);

		struct ErrorPosition * error_position = malloc(sizeof(struct ErrorPosition));

		error_position->Filepath = NULL;
		error_position->Index    = NULL;
		error_position->Line     = NULL;
		error_position->Col      = NULL;

		if( zend_hash_str_exists(position_ht, "filepath", sizeof("filepath")-1) ) {
			zval* filepath_zv = zend_hash_str_find(position_ht, "filepath", sizeof("filepath")-1);
			error_position->Filepath = Z_STRVAL_P(filepath_zv);
		}

		if( zend_hash_str_exists(position_ht, "index", sizeof("index")-1) ) {
			zval* index_zv = zend_hash_str_find(position_ht, "index", sizeof("index")-1);
			error_position->Index = malloc(sizeof(int));
			*(error_position->Index) = Z_LVAL_P(index_zv);
		}

		if( zend_hash_str_exists(position_ht, "line", sizeof("line")-1) ) {
			zval* line_zv = zend_hash_str_find(position_ht, "line", sizeof("line")-1);
			error_position->Line = malloc(sizeof(int));
			*(error_position->Line) = Z_LVAL_P(line_zv);
		}

		if( zend_hash_str_exists(position_ht, "col", sizeof("col")-1) ) {
			zval* col_zv = zend_hash_str_find(position_ht, "col", sizeof("col")-1);
			error_position->Col = malloc(sizeof(int));
			*(error_position->Col) = Z_LVAL_P(col_zv);
		}

		error->Position = error_position;
	}

	if( zend_hash_str_exists(error_ht, "trace", sizeof("trace")-1) ) {
		zval* trace_zv = zend_hash_str_find(error_ht, "trace", sizeof("trace")-1);
		HashTable* trace_ht = Z_ARR_P(trace_zv);
		int len = zend_hash_num_elements(trace_ht);

		char** trace = malloc(sizeof(char *) * (len + 1));

		for(int i = 0; i < len; i++ ) {
			zval* item_zv = zend_hash_index_find(trace_ht, i);
			trace[i] = Z_STRVAL_P(item_zv);
		}
		trace[len] = NULL; // NULL-terminated array

		error->Trace = trace;
	}

	return error;
}

void free_ValidationError(struct ValidationError * error) {
	// Note: no need to free strings, made by Z_STRVAL_P macros.
	if( error->Position != NULL ) {
		free(error->Position->Index);
		free(error->Position->Line);
		free(error->Position->Col);
		free(error->Position);
	}
	free(error->Trace);
	free(error);
}*/


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



