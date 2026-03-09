package Assignment_6;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.*;

public class Test_isEmpty {
    void single_test(ArrayList<Integer> inputs){    // Series of insertions with last as member to be checked
        Stack<Integer> oracle = new Stack<>();
        MyStack<Integer> code = new MyStack<>();
        for(int i:inputs){
            if(i>0){
                oracle.push(i);
                code.push(i);
            }
            else{
                if(!oracle.empty()) oracle.pop();
                code.pop();
            }
            assertEquals(oracle.empty(), code.isEmpty());
        }
    }

    // Does both branch and sentence coverage
    @Test
    void positive(){ single_test(new ArrayList<>(Arrays.asList(1,2,-3,4))); }
    @Test
    void zero(){ single_test(new ArrayList<>(Arrays.asList(5,6,-1,-1))); }
    @Test
    void underflow(){ single_test(new ArrayList<>(Arrays.asList(7,-1,-1,-1,8))); }

    @Test
    void multi_test() {
        int T = 10, N = 6;
        Random rand = new Random();

        while (T-- > 0){
            int n = 1+rand.nextInt(N);
            ArrayList<Integer> input = new ArrayList<Integer>();
            while(n-- > 0){
                input.add(rand.nextInt(N)-N/2);
                single_test(input);
            }
        }
    }
}