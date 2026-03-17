package Assignment_7.telephone;

import java.util.List;

interface Observer {    // Interface implemented in Screen
    void update(List<Integer> phoneNumber);
}

public class Main {
    public static void main(String[] args) {
        final int NUM_DIGITS = 10;

        // Build the object graph
        PhoneModel model = new PhoneModel();
        Screen screen = new Screen(model);
        KeyPad keyPad = new KeyPad(model);

        // Run the program
        keyPad.simulateKeyPresses(NUM_DIGITS);
    }
}
