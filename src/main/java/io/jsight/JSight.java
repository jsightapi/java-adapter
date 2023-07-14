package io.jsight;


public class JSight {
    static {
        System.loadLibrary("jsightjava");
        Init();
    }    

    public static native boolean Init();
    public static native String Stat();
    public static native String ValidateHttpRequest(String schemaFilePath, String requestMethod, String requestUri, String requestPayload, String logFilePath);
    public static native int ValidateHttpRequestTimeMks(String schemaFilePath, String requestMethod, String requestUri, String requestPayload, String logFilePath);
    public static native String ValidateHttpResponse(String schemaFilePath, String requestMethod, String requestUri, int responseStatusCode, String responsePayload, String logFilePath);
    public static native int ValidateHttpResponseTimeMks(String schemaFilePath, String requestMethod, String requestUri, int responseStatusCode, String responsePayload, String logFilePath);
}
