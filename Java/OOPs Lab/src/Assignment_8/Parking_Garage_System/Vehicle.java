package Assignment_8.Parking_Garage_System;

class Vehicle {
    private String number;
    private String type;
    private long entryTime;

    public Vehicle(String number, String type) {
        this.number = number;
        this.type = type;
        this.entryTime = System.currentTimeMillis();
    }

    public String getNumber() {
        return number;
    }

    public long getEntryTime() {
        return entryTime;
    }
}