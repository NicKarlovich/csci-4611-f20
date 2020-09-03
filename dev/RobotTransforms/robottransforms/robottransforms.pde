void setup()
{
  size(600, 310);
  smooth();
}

void draw()
{
  background(255);
  drawRobot();
  drawFloor();
}

void drawRobot()
{
  noStroke();
  fill(38, 38, 200);
  rect(20, 0, 38, 30); // head
  rect(14, 32, 50, 65); // body

  fill(38, 38, 200);
  rect(0, 32, 12, 26); // left upper arm
  rect(0, 60, 12, 26); // left lower arm
  fill(122, 122, 249);
  ellipse(6, 98, 12, 20); // left hand

  fill(38, 38, 200);
  rect(66, 32, 12, 26); // right upper arm  
  rect(66, 60, 12, 26); // right lower arm
  fill(122, 122, 249);
  ellipse(72, 98, 12, 20); // right hand
  
  fill(38, 38, 200);  
  rect(20, 100, 16, 40); // left upper leg
  rect(20, 142, 16, 40); // left lower leg
  fill(122, 122, 249);
  ellipse(28, 184, 26, 12); // left foot
  
  fill(38, 38, 200);  
  rect(42, 100, 16, 40); // right upper leg
  rect(42, 142, 16, 40); // right lower leg
  fill(122, 122, 249);
  ellipse(50, 184, 26, 12); // right foot
  
  fill(222, 222, 249);
  ellipse(30, 12, 12, 12); // left eye
  ellipse(47, 12, 12, 12); // right eye
}

void drawFloor()
{
  noStroke();
  fill(100, 255, 100);
  rect(0, 300, 600, 10); // head   
}
