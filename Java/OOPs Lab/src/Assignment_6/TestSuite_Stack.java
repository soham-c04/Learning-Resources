package Assignment_6;

import org.junit.platform.suite.api.SelectClasses;
import org.junit.platform.suite.api.Suite;

@Suite
@SelectClasses({ Test_pushpeek.class, Test_pop.class, Test_isEmpty.class})
public class TestSuite_Stack {
}
