package Assignment_8.University_Mgnt_System;

public class Main {

    public static void main(String[] args) {

        Course c1 = new Course("Mathematics", 3);
        Course c2 = new Course("Physics", 4);

        Student student = new Student("John");

        student.addEnrollment(new Enrollment(c1, 8.0));
        student.addEnrollment(new Enrollment(c2, 7.0));

        double gpa = student.calculateGPA();

        System.out.println("GPA: " + gpa);
    }
}
