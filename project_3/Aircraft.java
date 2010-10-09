import java.util.*;

public class Aircraft extends SpaceObject
{
  private int dest_x;
  private int dest_y;
  
  private int engines = 2;
  
  public boolean flying = true;
  
  public Aircraft(Space space)
  {
    super(space);
    space.add(this);
  }
  
  public String toString()
  {
    if (engines == 2)
      return "-a-";
    else if (engines == 1)
      return " a-";
    else
      return " a ";
  }
  
  public void move()
  {
    if(engines > 0 && flying)
    {
      moveTo(nextMove());
    }
  }
  
  public int[] nextMove()
  {
    int[] ret = {-1, -1};
    
    
    if(dest_x > x)
      ret[0] = x + 1;
      
    if(dest_y > y)
      ret[1] = y + 1;
    
    return ret;
  }
  
  public void moveTo(int[] move)
  {
    moveTo(move[0], move[1]);
  }
  
  public void moveTo(int x, int y)
  {
    if(x < 0) 
    {
      System.out.println("invalid x move");
    } else
      this.x = x;
    
    if(y < 0) 
    {
      System.out.println("invalid y move");
    } else
      this.y = y;
      
  }
  
  public void setStart(int x, int y)
  {
    this.x = x;
    this.y = y;
  }
  
  public void setDest(int x, int y)
  {
    this.dest_x = x;
    this.dest_y = y;
  }
  
  public boolean arrived()
  {
    return x == dest_x && y == dest_y;
  }
  
  public void applyRules()
  {
    for(SpaceObject o: space.objectsIn(x, y))
    {
      if(o instanceof Bird)
      {
        Bird b = (Bird) o;
        
        if(!b.airplane_flag)
        {
          b.airplane_flag = true;
          engines--;
          //System.out.println("\nSplat! We hit a bird. We have: " + engines + " engines left.");
        }
      }
    }
    
    if(engines <= 0)
    {
      System.out.println("We crashed.... :-(");
      flying = false;
    }  
  }
  
  public void run()
  {
    while(flying && !arrived())
    {
      applyRules();
      move();
      try{
        Thread.sleep(1000);
      } catch (Exception e) { }
    }
  }
}