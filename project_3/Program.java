import java.util.*;

public class Program
{
  static int NUM_BIRDS = 8;

  private List<Bird> birds;
  private List<Aircraft> aircrafts;
  static Space space;
  
  public static void main(String[] args)
  {
    Program program = new Program();
    
    //program.printSpaces();
    
    Aircraft aircraft = new Aircraft(space);
    
   
    for(int i = 0; i < NUM_BIRDS; i++)
    {
      new Bird(space);
    }
    
    aircraft.setStart(0, 0);
    aircraft.setDest(9, 9);
  program.printSpaces();
    while(aircraft.flying && !aircraft.arrived())
    {
      for(SpaceObject o : space.objects())
      {
        o.applyRules();
      }
      program.printSpaces();
    }
  }
  
  public Program()
  {
    space = new Space(10, 10);
  
    //spaces = new Space[10][10];
    //for(int i = 0; i < spaces.length; i++) 
    //  for(int j = 0; j < spaces[i].length; j++)
    //    spaces[i][j] = new Space(i, j);

    birds = new ArrayList<Bird>();
    aircrafts = new ArrayList<Aircraft>();
  }
  
  public void printSpaces() 
  {
    for(int i = 0; i < space.height; i++) 
    {
      for(int j = 0; j < space.width; j++) 
      {
        for(SpaceObject o: space.objectsIn(j, i))
        { 
            System.out.print(o);
        }
        System.out.print(" \t|\t ");
      } 
      
      System.out.println("\n_____________________________________________________________________________________________________________________________________________________________________________________________________");
    }
  }
}