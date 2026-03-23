package Assignment_8;

// State Pattern
interface FlightMode{
    void handle(FlightControl control);
}

class Idle implements FlightMode{
    @Override
    public void handle(FlightControl control){
        System.out.println("Mode - Idle");
        control.changeMode(new Takeoff());
    }
}

class Takeoff implements FlightMode{
    @Override
    public void handle(FlightControl control){
        System.out.println("Mode - Takeoff");
        control.changeMode(new Cruise());
    }
}

class Cruise implements FlightMode{
    @Override
    public void handle(FlightControl control){
        System.out.println("Mode - Cruise");
        control.changeMode(new Landing());
    }
}

class Landing implements FlightMode{
    @Override
    public void handle(FlightControl control){
        System.out.println("Mode - Landing");
        control.changeMode(new Idle());
    }
}

class FlightControl{
    private FlightMode mode;

    FlightControl(){ mode = new Idle(); }

    public void request(){
        mode.handle(this);
    }
    public void changeMode(FlightMode nextMode){
        mode = nextMode;
    }
}

public class Q2 {
    public static void main(String[] args){
        FlightControl control = new FlightControl();
        control.request();
        control.request();
        control.request();
        control.request();
    }
}
