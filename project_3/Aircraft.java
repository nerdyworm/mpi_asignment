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
    return "-|-";
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
    System.out.println("Plane: from: " + this.x + ", " + this.y + " to:" + x + ", " + y);
    
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
  
    if(engines > 0 && flying)
    {
      moveTo(nextMove());
    }
    
    
    for(SpaceObject o: space.objectsIn(x, y))
    {
      if(o instanceof Bird)
      {
        engines--;
        System.out.println("OMG PANIC WE HIT A BIRD");
        System.out.println("We have: " + engines + " engines left.");
      }
    }
    
    if(engines <= 0)
    {
      System.out.println("We crashed....");
      flying = false;
    }  
  }
}