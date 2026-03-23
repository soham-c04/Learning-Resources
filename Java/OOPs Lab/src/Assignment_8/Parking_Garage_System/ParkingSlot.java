package Assignment_8.Parking_Garage_System;

class ParkingSlot {
    private int slotNumber;
    private boolean isAvailable = true;

    public ParkingSlot(int slotNumber) {
        this.slotNumber = slotNumber;
    }

    public boolean isAvailable() {
        return isAvailable;
    }

    public void assignSlot() {
        isAvailable = false;
    }

    public void releaseSlot() {
        isAvailable = true;
    }

    public int getSlotNumber() {
        return slotNumber;
    }
}