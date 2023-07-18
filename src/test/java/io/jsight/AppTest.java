package io.jsight;

import static org.junit.Assert.assertTrue;
import static org.junit.Assert.assertNotNull;

import org.junit.Test;
import io.jsight.JSight;

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
}
