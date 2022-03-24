#include <Mouse.h>

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(50);
  Mouse.begin();
}

void loop() {
  if(Serial.available())
  {
    String data = Serial.readString();
    int pos = data.indexOf("*");
    if(pos != -1)
    {
      int relX = (data.substring(0, pos)).toInt();
      int relY = (data.substring(pos+1)).toInt();
      MouseMove(relX, relY);
    }
    else
    {
      String code = data.substring(0, 4);
      if(code == "DLEF")
      {
        MouseMove(-60, 60);
      }
      else if(code == "DOWN")
      {
        MouseMove(0, 60);
      }
      else if(code == "DRIG")
      {
        MouseMove(60, 60);
      }
      else if(code == "LEFT")
      {
        MouseMove(-60, 0);
      }
      else if(code == "CLIC")
      {
        Mouse.click();
      }
      else if(code == "RIGH")
      {
        MouseMove(60, 0);
      }
      else if(code == "ULEF")
      {
        MouseMove(-60, -60);
      }
      else if(code == "UPPP")
      {
        MouseMove(0, -60);
      }
      else if(code == "URIG")
      {
        MouseMove(60, -60);
      }
    }
  }

}

void MouseMove(int x, int y) 
{   for(int i=0; (x>0)?(i<x/100):(i>x/100); (x>0)?(i++):(i--))      Mouse.move((x>0)?(100):(-100), 0);   for(int j=0; (y>0)?(j<y/100):(j>y/100); (y>0)?(j++):(j--))      Mouse.move(0, (y>0)?(100):(-100));   Mouse.move(x%100, y%100); }
