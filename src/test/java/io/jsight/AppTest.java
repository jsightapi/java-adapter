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
        JSight.Init();
        String stat = JSight.Stat();
        System.out.println( stat );
        assertNotNull( stat );
    }

    @Test
    public void ValidateHttpRequest() {
        JSight.Init();

        Map requestHeaders = new HashMap<String, List<String>>();
        List<String> xHeaders = new ArrayList<String>();
        xHeaders.add("x-header-value-1");
        xHeaders.add("x-header-value-2");
        requestHeaders.put("X-header", xHeaders);

        ValidationError error = JSight.ValidateHttpRequest("String apiSpecFilePath", "String requestMethod", "String requestUri", requestHeaders, null);
    }
}
