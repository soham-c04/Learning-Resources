package Assignment_8.Parking_Garage_System;

class PaymentSystem {
    private long entryTime;

    public PaymentSystem(Vehicle v){
        entryTime = v.getEntryTime();
    }

    public double calculateFee() {
        long duration = System.currentTimeMillis() - entryTime;
        long minutes = duration / (1000 * 60);
        return minutes * 2;
    }
}