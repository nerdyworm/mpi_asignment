import java.util.*;

public class Program
{
  private List<Bird> birds;
  private List<Aircraft> aircrafts;
  private Space[][] spaces;
  
  public static void main(String[] args)
  {
    Program program = new Program();
    
    program.printSpaces();
  }
  
  public Program()
  {
    spaces = new Space[10][10];
    for(int i = 0; i < spaces.length; i++) 
      for(int j = 0; j < spaces[i].length; j++)
        spaces[i][j] = new Space(i, j);

    birds = new ArrayList<Bird>();
    aircrafts = new ArrayList<Aircraft>();
  }
  
  public void printSpaces() 
  {
    for(int i = 0; i < spaces.length; i++) 
    {
      for(int j = 0; j < spaces[i].length; j++) 
      {
         System.out.print(spaces[i][j] + " | ");
      } 
      
      System.out.println("");
    }
 
  }
}