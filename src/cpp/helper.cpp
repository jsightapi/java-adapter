#include <jni.h>
#include "helper.h"

struct Header ** init_Headers(JNIEnv * env, jobject jheaders) {
	struct Header ** headers = NULL;
	int item_n = 0;
	if( jheaders != NULL ) {

		printf("In C, about to enter Java\n");

		// java: Set<String> jkey_set = jheaders.keySet();
		jclass map_class = env->GetObjectClass(jheaders);
		jmethodID map_keySet_mid = env->GetMethodID(map_class, "keySet", "()Ljava/util/Set;");
		jobject jkey_set = env->CallObjectMethod(jheaders, map_keySet_mid);

		// java: empty_string_array = new String[0];
		jobjectArray empty_string_array = env->NewObjectArray(0, env->FindClass("java/lang/String"), NULL);

		// java: String[] jkeys = jkey_set.toArray(empty_string_array)
		jclass set_class = env->GetObjectClass(jkey_set);
		jmethodID map_toArray_mid = env->GetMethodID(set_class, "toArray", "([Ljava/lang/String;)[Ljava/lang/String;");
		jobject jkeys_object = env->CallObjectMethod(jkey_set, map_toArray_mid, empty_string_array);
		jobjectArray jkeys = static_cast<jobjectArray>(jkeys_object);

		jsize keys_count = env->GetArrayLength(jkeys);

		printf("ARRAY LENGTH: %d\n", keys_count);

		for(int i = 0; i < keys_count; i++) {

		}
	}
	
	return NULL;


		/*

		HashTable* headers_ht = Z_ARR_P(headers_zv);
		int headers_count = zend_hash_num_elements(headers_ht);

		headers = malloc(sizeof(struct Header *) * (headers_count + 1)); // Plus one place for NULL termination.

		zend_ulong num_key;
		zend_string * str_key;
		zval * item_zv;

		ZEND_HASH_FOREACH_KEY_VAL(headers_ht, num_key, str_key, item_zv) {
			if( str_key == NULL || Z_TYPE_P(item_zv) == IS_NULL ) {
				continue;
			}

			if( Z_TYPE_P(item_zv) == IS_ARRAY ) {
				HashTable* items_ht = Z_ARR_P(item_zv);

				if( zend_hash_num_elements(items_ht) == 0 ) {
					continue;
				}

				if( zend_hash_index_exists(items_ht, 0) == 0 ) {
					continue;
				}

				item_zv = zend_hash_index_find(items_ht, 0);
			}

			if( Z_TYPE_P(item_zv) != IS_STRING ) {
				continue;
			}

			headers[item_n] = malloc(sizeof(struct Header));
			headers[item_n]->Name = ZSTR_VAL(str_key);
			headers[item_n]->Value = Z_STRVAL_P(item_zv);

			item_n ++;
		} ZEND_HASH_FOREACH_END();

		headers[item_n] = NULL; // NULL termination
	}
	return headers;
	*/
}

/*
void free_Headers(struct Header ** headers) {
	if( headers != NULL ) {
		int i = 0;
		while(headers[i] != NULL) {
			free(headers[i]);
			i++;
		}
		free(headers);
	}
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