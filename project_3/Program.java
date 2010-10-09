import java.util.*;

public class Program
{ 
  public static void main(String[] args)
  {
    Program program = new Program();
    
    //program.run(8);
    //program.run(16);
    program.run(32);
  }
  
  public Program()
  {
  }
  
  public void run(int numbirds)
  {
    System.out.println("Running with " + numbirds + " birds.");
    Space space = new Space(10, 10);
    
    for(int i = 0; i < numbirds; i++)
    {
      new Bird(space);
    }
    
    Aircraft aircraft = new Aircraft(space);
    aircraft.setStart(0, 0);
    aircraft.setDest(9, 9);
    
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