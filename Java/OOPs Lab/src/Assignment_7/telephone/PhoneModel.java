package Assignment_7.telephone;

import java.util.ArrayList;
import java.util.List;

/**
 * Store a phone number, digit-by-digit
 */
public class PhoneModel {      // Subject
    private List<Integer> digits = new ArrayList<>();
    private List<Observer> observers = new ArrayList<>();

    public void addObserver(Observer o) {
        observers.add(o);
    }
    public void addDigit(int newDigit) {
        digits.add(newDigit);
        notifyObservers(newDigit);
    }

    private void notifyObservers(int newDigit) {
        for(Observer o : observers) o.update(digits);
    }
}
