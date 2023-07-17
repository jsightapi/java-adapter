package io.jsight;

import java.util.Map;
import io.jsight.ValidationError;

public class JSight {
    static {
        System.loadLibrary("jsightjava");
        Init();
    }    

    public static native boolean Init();
    public static native String Stat();
    public static native void ClearCache();
    
    public static native ValidationError ValidateHttpRequest (String apiSpecFilePath, String requestMethod, String requestUri, Map<String, String[]> requestHeaders, String requestBody);
    public static native ValidationError ValidateHttpResponse(String apiSpecFilePath, String requestMethod, String requestUri, int responseStatusCode, Map<String, String[]> responseHeaders, String responseBody);
    public static native String SerializeError(String format, ValidationError error);
}