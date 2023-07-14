package io.jsight;

public class ErrorPosition() {
    public String getFilepath();
    public int getCol();
    public int getIndex();
    public int getLine();
}

public class ValidationError {
    public int getCode();
    public String getDetail();
    public ErrorPosition getPosition();
    public String getReportedBy();
    public String getTitle();
    public String getType();
    public String ToJSON();
}

public class JSight {
    static {
        System.loadLibrary("jsightjava");
        Init();
    }    

    public static native boolean Init();
    public static native String Stat();
    public static native ClearCache();
    
    public static native ValidationError ValidateHttpRequest (String apiSpecFilePath, String requestMethod, String requestUri, requestHeaders, String requestBody);
    public static native ValidationError ValidateHttpResponse(String apiSpecFilePath, String requestMethod, String requestUri, int responseStatusCode, String responseHeaders, String responseBody);
    public static native String SerializeError(String format, error);
}
