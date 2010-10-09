import java.util.*;

public class SpaceObject
{
  protected Space space;
  public int x;
  public int y;
  
  public SpaceObject(Space space)
  {
    this.space = space;
  }
  
  public void move()
  {
    System.out.println("lol interface");
  }
  
  public void applyRules()
  {
    System.out.println("lol interface");
  }
  
  public int[] randomSpace()
  {
    Random random = new Random();
    int[] space = {random.nextInt(this.space.width), random.nextInt(this.space.height)};

    return space;
  }
}