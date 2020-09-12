import java.io.IOException;
 
static class TextToSpeech extends Object {
 
  // Store the voices, makes for nice auto-complete in Eclipse
 
  // male voices
  static final String ALEX = "Alex";
  static final String BRUCE = "Bruce";
  static final String FRED = "Fred";
  static final String JUNIOR = "Junior";
  static final String RALPH = "Ralph";
 
  // female voices
  static final String AGNES = "Agnes";
  static final String KATHY = "Kathy";
  static final String PRINCESS = "Princess";
  static final String VICKI = "Vicki";
  static final String VICTORIA = "Victoria";
 
  // novelty voices
  static final String ALBERT = "Albert";
  static final String BAD_NEWS = "Bad News";
  static final String BAHH = "Bahh";
  static final String BELLS = "Bells";
  static final String BOING = "Boing";
  static final String BUBBLES = "Bubbles";
  static final String CELLOS = "Cellos";
  static final String DERANGED = "Deranged";
  static final String GOOD_NEWS = "Good News";
  static final String HYSTERICAL = "Hysterical";
  static final String PIPE_ORGAN = "Pipe Organ";
  static final String TRINOIDS = "Trinoids";
  static final String WHISPER = "Whisper";
  static final String ZARVOX = "Zarvox";
 
  // throw them in an array so we can iterate over them / pick at random
  static String[] voices = {
    ALEX, BRUCE, FRED, JUNIOR, RALPH, AGNES, KATHY,
    PRINCESS, VICKI, VICTORIA, ALBERT, BAD_NEWS, BAHH,
    BELLS, BOING, BUBBLES, CELLOS, DERANGED, GOOD_NEWS,
    HYSTERICAL, PIPE_ORGAN, TRINOIDS, WHISPER, ZARVOX
  };
 
  // this sends the "say" command to the terminal with the appropriate args
  static void say(String script, String voice, int speed) {
    try {
      Runtime.getRuntime().exec(new String[] {"say", "-v", voice, "[[rate " + speed + "]]" + script});
    }
    catch (IOException e) {
      System.err.println("IOException");
    }
  }
 
  // Overload the say method so we can call it with fewer arguments and basic defaults
  static void say(String script) {
    // 200 seems like a resonable default speed
    say(script, ALEX, 200);
  }
 
}

  int offsetX   = 250;
  int offsetY   = 110;
  float rotation = 0.5;
  boolean waveup = true;
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
  pushMatrix();
  translate(100, offsetY);
  noStroke();
  fill(255, 0, 0);
  rect(20, 0, 38, 30); // head
  rect(14, 32, 50, 65); // body

  fill(255, 0, 0);
  rect(0, 32, 12, 26); // left upper arm
  rect(0, 60, 12, 26); // left lower arm
  fill(243, 85, 85);
  ellipse(6, 98, 12, 20); // left hand

  fill(255, 0, 0);
  rect(66, 32, 12, 26); // right upper arm  
  rect(66, 60, 12, 26); // right lower arm
  fill(243, 85, 85);
  ellipse(72, 98, 12, 20); // right hand
  
  fill(255, 0, 0);
  rect(20, 100, 16, 40); // left upper leg
  rect(20, 142, 16, 40); // left lower leg
  fill(243, 85, 85);
  ellipse(28, 184, 26, 12); // left foot
  
  fill(255, 0, 0);
  rect(42, 100, 16, 40); // right upper leg
  rect(42, 142, 16, 40); // right lower leg
  fill(243, 85, 85);
  ellipse(50, 184, 26, 12); // right foot
  
  fill(60, 60, 60);
  ellipse(30, 12, 12, 12); // left eye
  ellipse(47, 12, 12, 12); // right eye
  popMatrix();
  
  fill(0,0,0);
  if(rotation > 1 && !waveup){
   text("QUIT HITTING YOURSELF",80,75);
   stroke(10);
   line(150,80,150,100);
  }
  if(rotation > 2){
    waveup = false;
    TextToSpeech.say("QUIT HITTING YOURSELF", TextToSpeech.voices[11], 200);
    rotation = 2;
  } else if (rotation < 0){
    waveup = true;
  }
  if(waveup){
    rotation += 0.07;
  }else{
    rotation -= 0.02;
  }
  
  pushMatrix();
  translate(offsetX,offsetY);
  noStroke();
  fill(38, 38, 200);
  
  pushMatrix();
  translate(39, 30);
  rotate(-rotation*0.2);
  translate(-39,-30);
  rect(20, 0, 38, 30); // head
  fill(222, 222, 249);
  ellipse(30, 12, 12, 12); // left eye
  ellipse(47, 12, 12, 12); // right eye
  popMatrix();
  
  fill(38, 38, 200);
  rect(14, 32, 50, 65); // body
  fill(38, 38, 200);
  pushMatrix();
  translate(6,32);
  rotate(rotation);
  translate(-6, -32);
  rect(0, 32, 12, 26); // left upper arm
  pushMatrix();
  translate(6, 60);
  rotate(rotation + 0.3);
  translate(-6, -60);
  rect(0, 60, 12, 26); // left lower arm
  fill(122, 122, 249);
  pushMatrix();
  translate(6,92);
  rotate(rotation*0.5);
  translate(-6, -92);
  ellipse(6, 98, 12, 20); // left hand
  popMatrix();
  popMatrix();
  popMatrix();
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
  
  popMatrix();
}

void drawFloor()
{
  noStroke();
  fill(100, 255, 100);
  rect(0, 300, 600, 10); // head   
}
