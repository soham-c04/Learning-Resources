package Assignment_8.Drone_Delivery_System;

import java.util.*;

class Package {
    private String id;
    private String address;
    private double weight;

    public Package(String id, String address, double weight) {
        this.id = id;
        this.address = address;
        this.weight = weight;
    }

    public void display() {
        System.out.println(id + " - " + address + " - " + weight);
    }
}