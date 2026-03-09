package Assignment_6;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.*;

public class Test_push {
    void single_test(ArrayList<Integer> inputs){    // Series of insertions with last as member to be checked
        Stack<Integer> stk = new Stack<>();
        assertEquals(oracle.contains(inputs.get(i)), code.member(inputs.get(i)));
    }

    // Does both branch and sentence coverage
    @Test
    void increase(){ single_test(new ArrayList<>(Arrays.asList(1,2,3,4))); }
    @Test
    void decrease(){ single_test(new ArrayList<>(Arrays.asList(4,3,2,1))); }
    @Test
    void constant(){ single_test(new ArrayList<>(Arrays.asList(1,1,1,1))); }

    @Test
    void multi_test() {
        int T = 10, N = 5;
        Random rand = new Random();

        while (T-- > 0){
            int n = 1+rand.nextInt(N);
            ArrayList<Integer> input = new ArrayList<Integer>();
            while(n-- > 0){
                input.add(rand.nextInt(N));
                single_test(input);
            }
        }
    }
}