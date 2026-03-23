package Assignment_8.Parking_Garage_System;

import java.util.ArrayList;
import java.util.List;

public class Controller {
    private List<ParkingSlot> slots;

    public Controller() {
        slots = new ArrayList<>();
    }

    public void add(int slotNumber){
        slots.add(new ParkingSlot(slotNumber));
    }

    public void park(Vehicle v){
        for (ParkingSlot slot : slots) {
            if (slot.isAvailable()) {
                slot.assignSlot();
                System.out.println("Vehicle parked in slot " + slot.getSlotNumber());
                break;
            }
        }
    }

    public void displayAvailable(){
        // Display available slots
        System.out.print("Available slots: ");
        for (ParkingSlot slot : slots) {
            if (slot.isAvailable()) {
                System.out.print(slot.getSlotNumber() + " ");
            }
        }
        System.out.println();
    }
}
