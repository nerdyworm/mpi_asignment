import java.util.*;

public class Bird extends SpaceObject
{
  public boolean alive = true;
  public boolean airplane_flag = false;//used to make sure airplane knows it hit the bird
  
  private Random random;
  public Bird(Space space)
  {
    super(space);
    
    int[] randomSpace = randomSpace();
    this.x = randomSpace[0];
    this.y = randomSpace[1];
    
    space.add(this);
    
    random = new Random();
  }
  
  public String toString()
  {
    if(alive)
      return "b";
    else
      return " ";
  }
  
  
  public void move()
  { 
    if(alive)
    {
      moveTo(nextMove());
    }
  }
  
  
  public int[] nextMove()
  {
    int[] move = {0, 0};
    
    int[] possible = {-1, 0, 1};
    
    
    while(true)
    {
      move[0] = this.x + possible[random.nextInt(possible.length)];
      move[1] = this.y + possible[random.nextInt(possible.length)];
      
      if(validMove(move[0], move[1])) break;
    }
    
    return move;
  }
  
  public boolean validMove(int x, int y)
  {
    if(x < 0 || x > this.space.width - 1)
      return false;
      
    if(y < 0 || y > this.space.height - 1)
      return false;
      
    return true;
  }
  
  public void moveTo(int[] move)
  {
    moveTo(move[0], move[1]);
  }
  
  public void moveTo(int x, int y)
  {
    //System.out.println("Bird from: " + this.x + ", " + this.y + " to:" + x + ", " + y);
    this.x = x;
    this.y = y;
  }
  
  public void applyRules()
  {
    for(SpaceObject o: space.objectsIn(x, y))
    {
      if(o instanceof Aircraft)
      {
        alive = false;
        System.out.println("OMG PANIC WE HIT A AIRPLANE :( ");
      }
    }
  }
  
}