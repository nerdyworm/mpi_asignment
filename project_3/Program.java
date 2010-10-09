import java.util.*;

public class Program
{ 
  public static void main(String[] args) throws Exception
  {
    Program program = new Program();
    
    program.runSingleThread(8);
    program.runSingleThread(16);
    program.runSingleThread(32);
    
    program.runMultiThread(8);
    program.runMultiThread(16);
    program.runMultiThread(32);
  }
  
  public void runMultiThread(int numbirds)  throws Exception
  {
    System.out.println("Running multithreaded with " + numbirds + " birds.");
    
    Space space = new Space(10, 10);
    
    Aircraft aircraft = new Aircraft(space);
    aircraft.setStart(0, 0);
    aircraft.setDest(9, 9);
    
    List<Bird> birds = new ArrayList<Bird>();
    for(int i = 0; i < numbirds; i++)
    {
      Bird b = new Bird(space);
      birds.add(b);
    }
    
    for(Bird b: birds) 
      b.start();
      
    aircraft.start();
    
    while(aircraft.flying && !aircraft.arrived())
    {
      printSpaces(space);
      Thread.sleep(100);
    }
    
    for(Bird b: birds) 
      b.endThread();
    
    printSpaces(space);
    
    if(aircraft.arrived())
    {
      System.out.println("woooot we made it!");
    }
  }
  
  public void runSingleThread(int numbirds)
  {
    System.out.println("Running with " + numbirds + " birds.");
    
    Space space = new Space(10, 10);
    
    Aircraft aircraft = new Aircraft(space);
    aircraft.setStart(0, 0);
    aircraft.setDest(9, 9);
    
    for(int i = 0; i < numbirds; i++)
    {
      new Bird(space);
    }
    
    printSpaces(space);
    
    while(aircraft.flying && !aircraft.arrived())
    {
      for(SpaceObject o : space.objects())
        o.applyRules();
        
      for(SpaceObject o : space.objects())
        o.move();
      
      printSpaces(space);
    }
    
    if(aircraft.arrived())
    {
      System.out.println("We made it!!!");
    }
  
  }
  
  public void printSpaces(Space space) 
  {
    System.out.println("\n-------------------------------------------------------------------------");
    System.out.print("  |");
    for(int j = 0; j < space.width; j++)  
    {
      System.out.print("   " + j + "  |");
    }
    System.out.println("\n-------------------------------------------------------------------------");
    
    for(int i = 0; i < space.height; i++) 
    {
      System.out.print(i + " |");
      for(int j = 0; j < space.width; j++) 
      {
        int width = 5;
        for(SpaceObject o: space.objectsIn(j, i))
        { 
            System.out.print(o);
            if(o instanceof Aircraft)
              width -= 3;
            else
              width--;
        }
        
        for(int w = 0; w < width; w++)
          System.out.print(" ");
          
        System.out.print(" |");
      } 
      System.out.println("\n------------------------------------------------------------------------");
    }
    System.out.println("");
  }
}