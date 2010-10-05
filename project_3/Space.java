import java.util.*;

public class Space
{
  private int x;
  private int y;
  private List<Object> objects;
  
  public Space(int x, int y)
  {
    this.x = x;
    this.y = y;
    objects = new ArrayList<Object>();
  }
  
  public void add(Object obj)
  {
    objects.add(obj);
  }
  
  public int getX()
  {
    return x;
  }
  
  public int getY()
  {
    return y;
  }
  
  public String toString()
  {
    String str = "";
    for(Object o : objects)
      str += o.toString();
    return str + "";
  }
  
}