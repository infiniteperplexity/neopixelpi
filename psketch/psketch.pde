import processing.serial.*;
Serial arduino;
//String portName = "/dev/ttyACM0";
String portName;
String path = "C:/Users/infin/OneDrive/Documents/GitHub/neopixelpi/";
//string path = "/home/pi/Documents/GitHub/neopixelpi/";
String[] months = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
PFont font;
PFont lcdTiny;
PFont lcdFont;
PFont lcdSmall;
int index = 0;
int wait = 50;
long last = 0;
PImage img;
PShape shape;
PShape button;
ArrayList<Reef> reefs = new ArrayList<Reef>();
int tick;
float fade = 10;
int selected = 0;
boolean bookmarks = true;
//https://pae-paha.pacioos.hawaii.edu/erddap/griddap/dhw_5km.html
//https://en.wikipedia.org/wiki/List_of_reefs
//https://pae-paha.pacioos.hawaii.edu/erddap/griddap/dhw_5km.csv?CRW_DHW[(2020-01-01T12:00:00Z):1:(2020-05-17T12:00:00Z)][(17.175):1:(17.175)][(-87.325):1:(-87.325)],CRW_SST[(2020-01-01T12:00:00Z):1:(2020-05-17T12:00:00Z)][(17.175):1:(17.175)][(-87.325):1:(-87.325)]

void setup(){
  reefs.add(new Reef(2055, 1160, "Great Barrier Reef", "great"));
  //reefs.add(new Reef(1289, 852, "New Caledonia Barrier Reef", "newcaledonia"));
  reefs.add(new Reef(1885, 1000, "Tubbataha Reef", "tubbataha"));
  reefs.add(new Reef(1320, 920, "Red Sea Reef System", "redsea"));
  //reefs.add(new Reef(515, 900, "Florida Keys Reef Tract", "florida"));
  reefs.add(new Reef(515, 895, "Florida Keys Reef Tract", "florida"));
  //reefs.add(new Reef(467, 942, "Belize Barrier Reef", "belize"));
  reefs.add(new Reef(455, 955, "Belize Barrier Reef", "belize"));
  shape = loadShape("Continents.svg");
  button = loadShape("button.svg");
  fullScreen();
  
  font = loadFont("ArialMT-48.vlw");
  //DS-Digital by Dusit Supasawat
  lcdTiny = createFont("DS-DIGI.TTF", 36);
  lcdSmall = createFont("DS-DIGI.TTF", 72);
  lcdFont = createFont("DS-DIGI.TTF", 144);
  img = loadImage("button.png");
  tick = 0;
  try
  {
    portName = Serial.list()[0];
    print(portName);
    arduino = new Serial(this, portName, 9600);
  }
  catch(Exception e)
  {
    System.err.println("Error initializing Serial port!\nPlease check connections and port settings");
    e.printStackTrace();
  }
}
void draw(){
  long now = millis();
  if (now - last >= wait)
  {
    // this is how many "steps" the time advances
    int bump = int(now - last) / wait;
    Table t = reefs.get(selected).Data;
    // this could happen if the number of rows is off by one or two when you switch reefs, due to downloading on different dates
    if (index > t.getRowCount())
    {
      index = 0;
    }
    TableRow tr = t.getRow(index);
    String name = reefs.get(selected).Name;
    float dhw = tr.getFloat(3);
    float temp = tr.getFloat(4);
    String date = tr.getString(0).substring(0,7);
    int year = Integer.valueOf(date.substring(0,4));
    String month = months[Integer.valueOf(date.substring(5,7))-1];
    background(0, 0, 0);
    
    //// World Map
    shape.disableStyle();
    fill(0,255,00);
    shape(shape,-125,550, 2500, 1000);
    fill(0, 255, 0);
    
    //// Text
    textFont(lcdFont);
    // Name
    text(name, 150, 150+50);
    // Date
    text(year + " " + month, 150, 300+50);
    // Stress (DHW)
    text("stress: " + nf(dhw,1,2), 150, 450+50);
    // Celsius
    text(nf(temp,2,2), 940, 450+50);
    text(" C", 1265, 450+50);
    circle(1285, 370+50, 25);
    fill(0,0,0);
    circle(1285, 370+50, 12);
    // Farenheit
    fill(0, 255, 0);    
    circle(1795, 370+50, 25);
    fill(0,0,0);
    circle(1795, 370+50, 12);
    fill(0, 255, 0);
    text(nf(1.8*temp+32,2,2), 1450, 450+50);
    text(" F", 1775, 450+50);
    if (bookmarks == true)
    {
      textFont(lcdTiny);
      text("1987", 2100, 100);
      text("2015", 2100, 200);
    }
    tick += 1;
    for (int i = 0; i < reefs.size(); i++)
    {
      Reef r = reefs.get(i);
      if (selected == i)
      {
        fill(0,255,0);
      }
      else
      {
        fill(0,255,0);
      }
      int radius = 30;
      strokeWeight(3);
      if (selected == i)
      {
        radius = 50;
        strokeWeight(5);
      }
      circle(r.X, r.Y, 2*radius);
      pushMatrix();
      translate(r.X,r.Y);
      rotate(4*radians(tick));
      shape.disableStyle();
      fill(0,0,0);
      shape(button,-radius,-radius,2*radius,2*radius);
      popMatrix();

    }
    
    colorMode(RGB);
    try
    {
      arduino.write(Math.round(255*(1.0-bleach(dhw))));
    }
    catch(Exception e)
    {
      ;
    }
    index = (index + bump) % t.getRowCount();
    last = millis();
    
  }
  //delay(wait);
}

float bleach(float dhw)
{
  if (dhw <= 1)
  {
    return 0.2*dhw;
  }
  else if (dhw <= 4)
  {
    return 0.2 + 0.2*(dhw-1);
  }
  else if (dhw <= 8)
  {
    return 0.8 + (dhw-4)*0.05;
  }
  return 1;
}

class Reef
{
  int X;
  int Y;
  String Name;
  Table Data;
  Reef(int x, int y, String name, String fname)
  {
    X = x;
    Y = y;
    Name = name;
    Data = loadTable(fname +".csv", "header");
  }
}

void keyPressed()
{
  exit();
}

void mousePressed() {
  for (int i = 0; i < reefs.size(); i++)
  {
    Reef r = reefs.get(i);
    if (Math.sqrt((mouseX - r.X)*(mouseX - r.X) + (mouseY - r.Y)*(mouseY - r.Y))<30)
    {
      selected = i;
    }
  }
  if (bookmarks == true)
  {
    if (mouseX >= 2000)
    {
      if (mouseY >= 75 && mouseY <= 125)
      {
        selected = 0;
        index = 647;
      }
      else if (mouseY >= 175 && mouseY < 225)
      {
        selected = 0;
        index = 10874;
        //7217 would be 2005
        
      }
    }
  }
}
