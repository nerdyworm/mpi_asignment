import java.util.*;

public class Space
{
  public int width;
  public int height;
  private List<SpaceObject> objects;
  
  public Space(int width, int height)
  {
    this.width = width;
    this.height = height;
    objects = new ArrayList<SpaceObject>();
  }
  
  public void add(SpaceObject obj)
  {
    objects.add(obj);
  }
  
  public List<SpaceObject> objectsIn(int x, int  y)
  {
    List<SpaceObject> inSpace = new ArrayList<SpaceObject>();
    for(SpaceObject o : objects)
    {
      if(o.x == x && o.y == y)
        inSpace.add(o);
    }
    
    return inSpace;
  }

  public String toString()
  {
    String str = "";
    for(Object o : objects)
      str += o.toString();
    return str + "";
  }
  
  public List<SpaceObject> objects()
  {
    return objects;
  }
  
}