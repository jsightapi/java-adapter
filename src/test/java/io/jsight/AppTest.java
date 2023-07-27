package io.jsight;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

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
        if (new File("/home/config/test1").isFile()) {
            JSight.Init();
        } else if (new File("/home/config/test2").isFile()){
            JSight.Init("/home/lib");
        }

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
    public void testRequestError() throws JSONException {
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
    public void testResponseError() throws JSONException {
        Map headers = new HashMap<String, List<String>>();
        List<String> values = new ArrayList<String>();
        values.add("application/json");
        headers.put("Content-Type", values);
        byte[] body = "{\"id1\": 123, \"name\": \"John\"}".getBytes();
        ValidationError error = JSight.ValidateHttpResponse(this.testSpecPath, "POST", "/users", 200, headers, body);
        // System.out.printf("String expected = \"\"\n%sn", error.toJSON());
        String expected = ""
            + "{                                                           \n"
            + "  \"reportedBy\": \"HTTP Response validation\",              \n"
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
    public void testSerializeError() throws JSONException {
        ValidationError error = new ValidationError(
            "reportedBy", "type", 0, "title", "detail",
            new ErrorPosition("filePath", 1, 2, 3),
            new String[] {"one", "two", "three"}
        );
        // System.out.printf("String expected = \"\"\n%sn", error.toJSON());
        String expected = ""
            + " {                                   \n"
            + "   \"reportedBy\": \"reportedBy\",   \n"
            + "   \"type\": \"type\",               \n"
            + "   \"code\": 0,                      \n"
            + "   \"title\": \"title\",             \n"
            + "   \"detail\": \"detail\",           \n"
            + "   \"position\": {                   \n"
            + "     \"filepath\": \"filePath\",     \n"
            + "     \"index\": 3,                   \n"
            + "     \"line\": 1,                    \n"
            + "     \"col\": 2                      \n"
            + "   },                                \n"
            + "   \"trace\": [                      \n"
            + "     \"one\",                        \n"
            + "     \"two\",                        \n"
            + "     \"three\"                       \n"
            + "   ]                                 \n"
            + "}                                    \n";
        JSONAssert.assertEquals(expected, error.toJSON(), JSONCompareMode.STRICT);
    }

    @Test
    public void testSuccessfulRequest() throws JSONException {
        Map headers = new HashMap<String, List<String>>();
        List<String> values = new ArrayList<String>();
        values.add("application/json");
        headers.put("Content-Type", values);
        byte[] body = "{\"id\": 123, \"name\": \"John\"}".getBytes();
        ValidationError error = JSight.ValidateHttpRequest(this.testSpecPath, "POST", "/users", headers, body);
        assertNull(error);
    }

    @Test
    public void testSuccessfulResponse() throws JSONException {
        Map headers = new HashMap<String, List<String>>();
        List<String> values = new ArrayList<String>();
        values.add("application/json");
        headers.put("Content-Type", values);
        byte[] body = "{\"id\": 123, \"name\": \"John\"}".getBytes();
        ValidationError error = JSight.ValidateHttpResponse(this.testSpecPath, "POST", "/users", 200, headers, body);
        assertNull(error);
    }

    @Test
    public void testEmptyHeadersAndBodies() throws JSONException {
        ValidationError error = JSight.ValidateHttpRequest(this.testSpecPath, "GET", "/empty", null, null);
        assertNull(error);
        error = JSight.ValidateHttpRequest(this.testSpecPath, "GET", "/empty", new HashMap<String, List<String>>(), new byte[0]);
        assertNull(error);
        error = JSight.ValidateHttpResponse(this.testSpecPath, "GET", "/empty", 200, null, null);
        assertNull(error);
        error = JSight.ValidateHttpResponse(this.testSpecPath, "GET", "/empty", 200, new HashMap<String, List<String>>(), new byte[0]);
        assertNull(error);    
    }

    @Test
    public void testNullPointerResistance() throws JSONException {
        ValidationError error = JSight.ValidateHttpRequest(null, null, null, null, null);
        assertNotNull(error);
        error = JSight.ValidateHttpResponse(null, null, null, 0, null, null);
        assertNotNull(error);
        String some = JSight.SerializeError(null, null);
        assertNotNull(some);
        String json = new ValidationError(
                null, null, 0, null, null,
                new ErrorPosition(null, null, null, null),
                new String[] {null, null, null}
            ).toJSON();
        assertNotNull(json);
        json = new ValidationError(null, null, 0, null, null, null, null).toJSON();
        assertNotNull(json);
    }
}
