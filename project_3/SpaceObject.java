import java.util.*;

public class SpaceObject extends Thread
{
  protected Space space;
  public int x;
  public int y;
  
  public SpaceObject(Space space)
  {
    this.space = space;
  }
  
  public void move()
  {}
  
  public void applyRules()
  {}
  
  public int[] randomSpace()
  {
    Random random = new Random();
    int[] space = {random.nextInt(this.space.width), random.nextInt(this.space.height)};

    return space;
  }
  
  public void run()
  {}
}