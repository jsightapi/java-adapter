package io.jsight;

import java.util.Map;
import java.util.List;
import io.jsight.ValidationError;

public class JSight {

    static {
        System.loadLibrary("jsightjava");
    }

    private static native void initSharedLibrary(String libDir);

    public static void Init() {
        Init("/usr/local/lib");
    }

    public static void Init(String jsightLibDir) {
        initSharedLibrary(jsightLibDir);
    }

    public static native String Stat();
    public static native void ClearCache();
    
    public static native ValidationError ValidateHttpRequest (String apiSpecFilePath, String requestMethod, String requestUri, Map<String,List<String>> requestHeaders, byte[] requestBody);
    public static native ValidationError ValidateHttpResponse(String apiSpecFilePath, String requestMethod, String requestUri, int responseStatusCode, Map<String,List<String>> responseHeaders, byte[] responseBody);
    public static native String SerializeError(String format, ValidationError error);
}