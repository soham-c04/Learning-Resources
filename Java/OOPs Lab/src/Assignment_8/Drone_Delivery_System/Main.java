package Assignment_8.Drone_Delivery_System;

public class Main{
	 public static void main(String[] args) {

        Mission mission = new Mission();

        mission.addPackage("P1", "City A", 5.0);
        mission.addPackage("P2", "City B", 3.5);

        for (Package p : mission.getPackages()) {
            p.display();
        }
    }
}