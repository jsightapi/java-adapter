package io.jsight;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import io.jsight.JSight;
import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;
import java.io.File;
import org.skyscreamer.jsonassert.JSONAssert;
import org.skyscreamer.jsonassert.JSONCompareMode;
import org.json.JSONException;

public class AppTest
{
    private static String testSpecPath = "";

    static {
        JSight.Init();

        String testSpecName = "test.jst";

        ClassLoader classLoader = AppTest.class.getClassLoader();
        File specFile = new File(classLoader.getResource(testSpecName).getFile());
        testSpecPath = specFile.getAbsolutePath();
        System.out.printf("Test specification file: %s\n", testSpecPath);
    }

    @Test
    public void testStat() {
        String stat = JSight.Stat();
        // System.out.println( stat );
        assertNotNull( stat );
    }

    @Test
    public void testClearCache() {
        JSight.ClearCache();
    }

    @Test
    public void testApiSpecError() throws JSONException {
        ValidationError error = JSight.ValidateHttpRequest("not-existing-api-spec.jst", "GET", "/users", null, null);
        String expected = ""
            + " {                                                  "
            + "    \"reportedBy\": \"HTTP Request validation\",    "
            + "    \"type\": \"api_spec_error\",                   "
            + "    \"code\": 10001,                                "
            + "    \"title\": \"API spec file not found\",         "
            + "    \"detail\": \"\",                               "
            + "    \"position\": {                                 "
            + "        \"filepath\": \"not-existing-api-spec.jst\" "
            + "    }                                               "
            + " }                                                  ";
        JSONAssert.assertEquals(expected, error.toJSON(), JSONCompareMode.STRICT);
    }

    @Test
    public void testBodyError() throws JSONException {
        Map headers = new HashMap<String, List<String>>();
        List<String> values = new ArrayList<String>();
        values.add("application/json");
        headers.put("Content-Type", values);
        byte[] body = "{\"id1\": 123, \"name\": \"John\"}".getBytes();
        ValidationError error = JSight.ValidateHttpRequest(this.testSpecPath, "POST", "/users", headers, body);
        // System.out.printf("String expected = \"\"\n%sn", error.toJSON());
        String expected = ""
            + "{                                                           \n"
            + "  \"reportedBy\": \"HTTP Request validation\",              \n"
            + "  \"type\": \"http_body_error\",                            \n"
            + "  \"code\": 32001,                                          \n"
            + "  \"title\": \"HTTP body error\",                           \n"
            + "  \"detail\": \"Schema does not support key \\\"id1\\\"\",  \n"
            + "  \"position\": {                                           \n"
            + "    \"index\": 1,                                           \n"
            + "    \"line\": 1,                                            \n"
            + "    \"col\": 2                                              \n"
            + "  }                                                         \n"
            + "}                                                           \n";
        JSONAssert.assertEquals(expected, error.toJSON(), JSONCompareMode.STRICT);
    }


    @Test
    public void ValidateHttpRequest() {
        Map requestHeaders = new HashMap<String, List<String>>();
        List<String> xHeaders = new ArrayList<String>();
        xHeaders.add("x-header-value-1");
        xHeaders.add("x-header-value-2");
        requestHeaders.put("X-header", xHeaders);
        List<String> yHeaders = new ArrayList<String>();
        yHeaders.add("y-header-value-1");
        requestHeaders.put("Y-header", yHeaders);
        byte[] requestBody = "the body".getBytes();

        ValidationError error = JSight.ValidateHttpRequest(this.testSpecPath, "String requestMethod", "String requestUri", requestHeaders, requestBody);

        //System.out.printf("Error:\n%s\n", error.toJSON());
    }

    @Test
    public void ValidateHttpResponse() {
        Map responseHeaders = new HashMap<String, List<String>>();
        List<String> xHeaders = new ArrayList<String>();
        xHeaders.add("x-header-value-1");
        xHeaders.add("x-header-value-2");
        responseHeaders.put("X-header", xHeaders);
        List<String> yHeaders = new ArrayList<String>();
        yHeaders.add("y-header-value-1");
        responseHeaders.put("Y-header", yHeaders);
        byte[] responseBody = "the body".getBytes();

        ValidationError error = JSight.ValidateHttpResponse(this.testSpecPath, "String requestMethod", "String requestUri", 200, responseHeaders, responseBody);

        //System.out.printf("Error:\n%s\n", error.toJSON());
    }
}
