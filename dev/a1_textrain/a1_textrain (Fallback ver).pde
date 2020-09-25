/*
 * CSci-4611 Assignment #1 Text Rain
 */


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
// Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;


void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
}


void draw() {
  
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }

  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.
  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
//  if ((cam != null) && (cam.available())) {
//    cam.read();
//    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
//  }
   if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }

  // This code draws the current camera/movie image to the screen
  set(0, 0, inputImage);

  // TODO: Fill in your code to implement the rest of TextRain here..

}



void keyPressed() {
  if (!inputMethodSelected) {
    
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
    //  else if ((input >= 1) && (input <= 9)) {
    //    println("Camera " + input + " selected.");
        
        // The camera can be initialized directly using an element from the array returned by list():
    //    cam = new Capture(this, cameras[input-1]);
    //    cam.start();
    //    inputMethodSelected = true;
    //  }
    }
    return;
  }

  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // TODO: Fill in your code to handle keypresses here..
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
    }
  }
  else if (key == ' ') {
    // spacebar pressed
  } 
}
