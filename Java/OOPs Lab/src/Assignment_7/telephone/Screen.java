package Assignment_7.telephone;

import java.util.List;

/**
 * Prints things out to the screen, when needed
 * Printing to the screen:
 *  System.out.println("hello");
 */
public class Screen {       // Has 2 Observers
    private final PhoneModel model;
    private static final int DIGITS = 10;

    public Screen(PhoneModel model) {
        this.model = model;

        Observer press = new Observer() {
            @Override
            public void update(List<Integer> phoneNumber) {
                System.out.println("Pressed: " + phoneNumber.get(phoneNumber.size()-1));
            }
        };

        Observer dial = new Observer() {
            @Override
            public void update(List<Integer> phoneNumber) {
                if(phoneNumber.size()==DIGITS){
                    StringBuilder call = new StringBuilder();
                    for(int digit:phoneNumber) call.append(digit);
                    System.out.println("\nNow dialing " + call.toString() + "...");
                }
            }
        };

        model.addObserver(press);
        model.addObserver(dial);
    }
}
