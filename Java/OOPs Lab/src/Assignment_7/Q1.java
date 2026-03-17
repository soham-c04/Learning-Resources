package Assignment_7;

import java.util.ArrayList;

abstract class AcademicComponent{
    private String name;
    protected AcademicComponent(String name){
        this.name = name;
    }
    protected String getName(){
        return name;
    }
    public void add(AcademicComponent component) {
        throw new UnsupportedOperationException();
    }
    public abstract void display();
}

class Course extends AcademicComponent{
    public Course(String name){
        super(name);
    }
    @Override
    public void display() {
        System.out.println("    Course: " + getName());
    }
}

class CourseModule extends AcademicComponent{
    private ArrayList<AcademicComponent> components = new ArrayList<>();
    public CourseModule(String name){
        super(name);
    }
    @Override
    public void add(AcademicComponent component) {
        components.add(component);
    }

    @Override
    public void display() {
        System.out.println("Module: " + getName());
        for (AcademicComponent ac : components) {
            ac.display();
        }
    }
}



public class Q1 {
    public static void main(String[] args) {
        // courses
        AcademicComponent introProg = new Course("Introduction to Programming");
        AcademicComponent dataStructures = new Course("Data Structures");
        AcademicComponent machineLearning = new Course("Machine Learning");
        AcademicComponent nlp = new Course("Natural Language Processing");
        AcademicComponent finalProject = new Course("Final Year Project");
        // Modules
        AcademicComponent programmingModule = new CourseModule("Programming Module");
        programmingModule.add(introProg);
        programmingModule.add(dataStructures);
        AcademicComponent aiModule = new CourseModule("AI Module");
        aiModule.add(machineLearning);
        aiModule.add(nlp);
        // Program root
        AcademicComponent csProgram = new CourseModule("Computer Science Program");
        csProgram.add(programmingModule);
        csProgram.add(aiModule);
        csProgram.add(finalProject);
        // Display program structure
        csProgram.display();
    }
}
