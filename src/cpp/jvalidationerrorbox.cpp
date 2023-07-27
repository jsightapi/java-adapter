#include "jvalidationerrorbox.hpp"
#include "helper.h"

JValidationErrorBox::JValidationErrorBox(JNIEnv * env, jobject jerror) {
    
    this->env = env;
	this->error = new ValidationError();

    this->error->ReportedBy = NULL;
	this->error->Type       = NULL;
	this->error->Code       = -1;
	this->error->Title      = NULL;
	this->error->Detail     = NULL;
	this->error->Position   = NULL;
	this->error->Trace      = NULL;

	if( jerror != NULL ) {
		jclass error_class = env->GetObjectClass(jerror);

		jmethodID get_reported_by_mid = env->GetMethodID(error_class, "getReportedBy", "()Ljava/lang/String;");
		jmethodID get_type_mid        = env->GetMethodID(error_class, "getType"      , "()Ljava/lang/String;");
		jmethodID get_code_mid        = env->GetMethodID(error_class, "getCode"      , "()I");
		jmethodID get_title_mid       = env->GetMethodID(error_class, "getTitle"     , "()Ljava/lang/String;");
		jmethodID get_detail_mid      = env->GetMethodID(error_class, "getDetail"    , "()Ljava/lang/String;");
		jmethodID get_position_mid    = env->GetMethodID(error_class, "getPosition"  , "()Lio/jsight/ErrorPosition;");
		jmethodID get_trace_mid       = env->GetMethodID(error_class, "getTrace"     , "()[Ljava/lang/String;");

		this->jreported_by = static_cast<jstring>(env->CallObjectMethod(jerror, get_reported_by_mid));
		this->jtype        = static_cast<jstring>(env->CallObjectMethod(jerror, get_type_mid       ));
		this->jcode        = env->CallIntMethod(jerror, get_code_mid);
		this->jtitle       = static_cast<jstring>(env->CallObjectMethod(jerror, get_title_mid ));
		this->jdetail      = static_cast<jstring>(env->CallObjectMethod(jerror, get_detail_mid));
		this->jposition    = env->CallObjectMethod(jerror, get_position_mid);
		this->jtrace       = static_cast<jobjectArray>(env->CallObjectMethod(jerror, get_trace_mid));

		jboolean isCopy;

		if( this->jreported_by == NULL ) { this->error->ReportedBy = empty_string(); }
		else { this->error->ReportedBy = (char*)env->GetStringUTFChars(this->jreported_by, &isCopy);}

		if( this->jtype == NULL ) { this->error->Type = empty_string(); }
		else { this->error->Type = (char*)env->GetStringUTFChars(this->jtype, &isCopy);}

		this->error->Code = this->jcode;

		if( this->jtitle == NULL ) { this->error->Title = empty_string(); }
		else { this->error->Title = (char*)env->GetStringUTFChars(this->jtitle, &isCopy);}

		if( this->jdetail == NULL ) { this->error->Detail = empty_string(); }
		else { this->error->Detail = (char*)env->GetStringUTFChars(this->jdetail, &isCopy);}

		if( this->jposition != NULL ) {
			this->error->Position = new ErrorPosition();

			this->error->Position->Filepath = NULL;
			this->error->Position->Index    = NULL;
			this->error->Position->Line     = NULL;
			this->error->Position->Col      = NULL;

			jclass position_class = env->GetObjectClass(jposition);

			jmethodID get_filepath = env->GetMethodID(position_class, "getFilepath", "()Ljava/lang/String;");
			jmethodID get_line     = env->GetMethodID(position_class, "getLine"    , "()Ljava/lang/Integer;");
			jmethodID get_col      = env->GetMethodID(position_class, "getCol"     , "()Ljava/lang/Integer;");
			jmethodID get_index    = env->GetMethodID(position_class, "getIndex"   , "()Ljava/lang/Integer;");

			this->jfilepath = static_cast<jstring>(env->CallObjectMethod(jposition, get_filepath));
			this->jline     = env->CallObjectMethod(jposition, get_line    );
			this->jcol      = env->CallObjectMethod(jposition, get_col     );
			this->jindex    = env->CallObjectMethod(jposition, get_index   );

			if( this->jfilepath == NULL ) { this->error->Position->Filepath = empty_string(); }
			else { this->error->Position->Filepath = (char*)env->GetStringUTFChars(this->jfilepath, &isCopy);}

			jclass integer_class = env->FindClass("java/lang/Integer");
			jmethodID integer_intvalue_mid = env->GetMethodID(integer_class, "intValue", "()I");

			if( this->jline != NULL ) {
				this->error->Position->Line = (int *) malloc(sizeof(int));
				* this->error->Position->Line = env->CallIntMethod(this->jline, integer_intvalue_mid);
			}

			if( this->jcol != NULL ) {
				this->error->Position->Col = (int *) malloc(sizeof(int));
				* this->error->Position->Col = env->CallIntMethod(this->jcol, integer_intvalue_mid);
			}

			if( this->jindex != NULL ) {
				this->error->Position->Index = (int *) malloc(sizeof(int));
				* this->error->Position->Index = env->CallIntMethod(this->jindex, integer_intvalue_mid);
			}
		}

		if( this->jtrace != NULL ) {
			this->trace_size = env->GetArrayLength(this->jtrace);
			this->error->Trace = (char **) malloc(sizeof(char  *) * (trace_size + 1));
			this->jtrace_items = (jstring *) malloc(sizeof(jstring) * (trace_size + 1));

			for(int i = 0; i < trace_size; i++ ) {
				this->jtrace_items[i] = static_cast<jstring>(env->GetObjectArrayElement(this->jtrace, i));
				if( this->jtrace_items[i] == NULL ) { this->error->Trace[i] = empty_string(); }
				else { this->error->Trace[i] = (char *) env->GetStringUTFChars(this->jtrace_items[i], &isCopy);}
			}
			this->error->Trace[trace_size] = NULL; // Null terminated array of strings.
		}
	}
}

JValidationErrorBox::~JValidationErrorBox() {

    if(this->error != NULL) {
        if(this->jreported_by == NULL) { free(this->error->ReportedBy); }
        else { this->env->ReleaseStringUTFChars(this->jreported_by, this->error->ReportedBy);}

        if(this->jtype == NULL) { free(this->error->Type); }
        else { this->env->ReleaseStringUTFChars(this->jtype, this->error->Type);}

        if(this->jtitle == NULL) { free(this->error->Title); }
        else { this->env->ReleaseStringUTFChars(this->jtitle, this->error->Title);}

        if(this->jdetail == NULL) { free(this->error->Detail); }
        else { this->env->ReleaseStringUTFChars(this->jdetail, this->error->Detail);}

        if( this->error->Position != NULL ) {
            if(this->error->Position->Index != NULL) free(this->error->Position->Index);
            if(this->error->Position->Line  != NULL) free(this->error->Position->Line);
            if(this->error->Position->Col   != NULL) free(this->error->Position->Col);
            
            if(this->jfilepath == NULL) { free(this->error->Position->Filepath); }
            else { this->env->ReleaseStringUTFChars(this->jfilepath, this->error->Position->Filepath);}

            free(this->error->Position);
        }

        if( this->error->Trace != NULL ) {
            for(int i = 0; i < this->trace_size; i++) {
                if( this->jtrace_items[i] == NULL ) { free(this->error->Trace[i]); }
				else { this->env->ReleaseStringUTFChars(this->jtrace_items[i], this->error->Trace[i]);}
            }
            free(this->error->Trace);
        }
    
    	free(error);
    }
}

struct ValidationError * JValidationErrorBox::getValidationError() {
    return this->error;
}