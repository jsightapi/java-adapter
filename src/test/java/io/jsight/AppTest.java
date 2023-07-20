package io.jsight;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import io.jsight.JSight;
import java.util.Map;
import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;

/**
 * Unit test for simple App.
 */
public class AppTest 
{
    static {
        JSight.Init();
    }

    /**
     * Rigorous Test :-)
     */
    @Test
    public void shouldAnswerWithTrue()
    {
        assertTrue( true );
    }

    @Test
    public void testJSightStat() {
        String stat = JSight.Stat();
        System.out.println( stat );
        assertNotNull( stat );
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

        ValidationError error = JSight.ValidateHttpRequest("String apiSpecFilePath", "String requestMethod", "String requestUri", requestHeaders, requestBody);

        System.out.printf("Error: %s\n", error.toJSON());
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

        ValidationError error = JSight.ValidateHttpResponse("String apiSpecFilePath", "String requestMethod", "String requestUri", 200, responseHeaders, responseBody);

        System.out.printf("Error: %s\n", error.toJSON());
    }
}
