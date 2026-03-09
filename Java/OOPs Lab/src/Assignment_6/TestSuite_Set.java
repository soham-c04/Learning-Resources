package Assignment_6;

import org.junit.platform.suite.api.SelectClasses;
import org.junit.platform.suite.api.Suite;

@Suite
@SelectClasses({ Test_insert.class, Test_member.class, Test_section.class, Test_containsArithTriple.class})
public class TestSuite_Set {
}