package Assignment_6;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.*;

public class Test_pushpeek {
    <T> void single_test(ArrayList<T> inputs){    // Series of insertions with last as member to be checked
        Stack<T> oracle = new Stack<>();
        MyStack<T> code = new MyStack<>();
        for(T i:inputs){
            oracle.push(i);
            code.push(i);
            assertEquals(oracle.peek(), code.peek());
        }
    }

    // Does both branch and sentence coverage
    @Test
    void intarray(){ single_test(new ArrayList<>(Arrays.asList(1,2,3,4))); }
    @Test
    void strarray(){ single_test(new ArrayList<>(Arrays.asList("a","e","w","b"))); }
    @Test
    void floatarr(){ single_test(new ArrayList<>(Arrays.asList(3.14, 2.71, 1.41, 2.23))); }

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