package Assignment_6;

import java.util.ArrayList;

public class MyStack<T>{
    private ArrayList<T> stack;

    public MyStack() {
        stack = new ArrayList<>();
    }
    public void push(T item){
        stack.add(item);
    }
    public boolean isEmpty(){
        return (stack.size()==0);
    }
    public T peek(){
        if(isEmpty()) return null;
        T TOP = stack.getLast();
        return TOP;
    }
    public T pop(){
        T TOP = peek();
        if(TOP == null) return TOP;
        stack.removeLast();
        return TOP;
    }
}
