package Assignment_7;

class WaterController{      // Singleton Class [Design Pattern]
    private static final WaterController water = new WaterController();
    private WaterController(){}
    public static WaterController getInstance(){ return water; }
    public static void openValve(String who){
        System.out.println("Valve opened by " + who);
    }
    public static void closeValve(String who){
        System.out.println("Value closed by " + who);
    }
}

abstract class AgricultureModule{
    protected String type;
    public void requestWater(){
        WaterController controller = WaterController.getInstance();
        controller.openValve(type);
        controller.closeValve(type);
    }
}

class MoistureSensor extends AgricultureModule {
    public MoistureSensor(){ type = "MoistureSensor"; }
}

class WeatherMonitor extends AgricultureModule {
    public WeatherMonitor(){ type = "WeatherMonitor"; }
}

class IrrigationScheduler extends AgricultureModule {
    public IrrigationScheduler(){ type = "IrrigationScheduler"; }
}

public class Q2 {
    public static void main(String[] args) {
        MoistureSensor sensor = new MoistureSensor();
        WeatherMonitor weather = new WeatherMonitor();
        IrrigationScheduler scheduler = new IrrigationScheduler();
        System.out.println("MoistureSensor requesting water supply");
        sensor.requestWater();
        System.out.println("\nWeatherMonitor requesting water supply");
        weather.requestWater();
        System.out.println("\nIrrigationScheduler requesting water supply");
        scheduler.requestWater();
    }
}
