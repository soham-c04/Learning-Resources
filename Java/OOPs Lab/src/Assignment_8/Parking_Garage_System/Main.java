package Assignment_8.Parking_Garage_System;

import java.util.*;

public class Main {

    public static void main(String[] args) {

        Controller controller = new Controller();
        controller.add(1);
        controller.add(2);

        Vehicle v = new Vehicle("KA01AB1234", "Car");

        controller.park(v);
        controller.displayAvailable();

        PaymentSystem payment = new PaymentSystem(v);   // 1 System for each vehicle
        double fee = payment.calculateFee();
        System.out.println("Parking fee: " + fee);
    }
}