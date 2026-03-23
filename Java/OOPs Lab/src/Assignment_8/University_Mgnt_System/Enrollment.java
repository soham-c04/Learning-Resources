package Assignment_8.University_Mgnt_System;

class Enrollment {
    private Course course;
    private double grade;

    public Enrollment(Course course, double grade) {
        this.course = course;
        this.grade = grade;
    }

    public Course getCourse() {
        return course;
    }

    public double getGrade() {
        return grade;
    }
}