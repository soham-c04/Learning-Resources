package Assignment_7.telephone;

import java.util.Random;
import java.util.Scanner;

/**
 * Mimic the data input ability of a physical phone's keypad;
 * however, here we're just sending it fake digits.
 */
public class KeyPad {       // Input Soruce
    private final PhoneModel model;

    public KeyPad(PhoneModel model) {
        this.model = model;
    }

    public void simulateKeyPresses(int numKeyPresses) {
        Scanner sc = new Scanner(System.in);
        for (int i = 0; i < numKeyPresses; i++) {
            int newDigit = sc.nextInt(); sc.nextLine();
            model.addDigit(newDigit);
        }
    }

}
