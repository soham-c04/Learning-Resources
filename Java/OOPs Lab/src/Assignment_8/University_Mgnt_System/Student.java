package Assignment_8.University_Mgnt_System;

import java.util.*;

class Student {
    private String name;
    private List<Enrollment> enrollments = new ArrayList<>();

    public Student(String name) {
        this.name = name;
    }

    public void addEnrollment(Enrollment e) {
        enrollments.add(e);
    }
    public List<Enrollment> getEnrollments() {
        return enrollments;
    }

    public double calculateGPA(){   // Student is the Expert Class for calculating GPA. No need for GPAUtil.
        double total = 0;
        int credits = 0;

        for (Enrollment e : enrollments) {
            total += e.getGrade() * e.getCourse().getCredits();
            credits += e.getCourse().getCredits();
        }

        return total / credits;
    }
}