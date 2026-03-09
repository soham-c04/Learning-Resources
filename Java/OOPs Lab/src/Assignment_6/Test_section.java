package Assignment_6;

import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

import java.util.*;

public class Test_section {
    void single_test(ArrayList<Integer> a, ArrayList<Integer> b){    // Series of insertions for 2 sets
        TreeSet<Integer> oracle = new TreeSet<Integer>();
        Set codea = new Set();
        Set codeb = new Set();
        for(int i:a){
            oracle.add(i);
            codea.insert(i);
        }
        for(int i:b){
            oracle.remove(i);
            codeb.insert(i);
        }

        codea.section(codeb);
        assertEquals(Arrays.toString(oracle.stream().mapToInt(Integer::intValue).toArray()),Arrays.toString(codea.toArray()));
    }

    // Does both branch and sentence coverage
    @Test
    void increase(){ single_test(new ArrayList<>(Arrays.asList(1,2,3,4)), new ArrayList<>(Arrays.asList(1,2,3,4))); }
    @Test
    void decrease(){ single_test(new ArrayList<>(Arrays.asList(4,3,2,1)), new ArrayList<>(Arrays.asList(4,3,2,1))); }
    @Test
    void constant(){ single_test(new ArrayList<>(Arrays.asList(1,1,1,1)), new ArrayList<>(Arrays.asList(1))); }

    @Test
    void multi_test() {
        int T = 10, N = 5;
        Random rand = new Random();

        while (T-- > 0){
            ArrayList<Integer> input1 = new ArrayList<Integer>();
            ArrayList<Integer> input2 = new ArrayList<Integer>();
            int n = 1+rand.nextInt(N);
            while(n-- > 0) input1.add(rand.nextInt(N));
            n = 1+rand.nextInt(N);
            while(n-- > 0){
                input2.add(rand.nextInt(N));
                single_test(input1, input2);
            }
        }
    }
}