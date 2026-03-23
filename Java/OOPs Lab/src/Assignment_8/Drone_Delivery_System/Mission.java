package Assignment_8.Drone_Delivery_System;

import java.util.*;

class Mission {
    private List<Package> packages = new ArrayList<>();

    public void addPackage(String id, String address, double weight) {
        packages.add(new Package(id, address, weight));
    }

    public List<Package> getPackages() {
        return packages;
    }
}