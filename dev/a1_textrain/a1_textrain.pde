/*
 * CSci-4611 Assignment #1 Text Rain
 */

import processing.video.*;
import java.util.ArrayList;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;
int count = 0;
String poem = "I don't want to be a part of an us that makes a them of the worlds most vulnerable people - John Green";
char[] poemArr = poem.toCharArray();
ArrayList<String> words = new ArrayList<String>(0);
ArrayList<Integer> wordIdx = new ArrayList<Integer>(0);
ArrayList<Float> rainVelocity = new ArrayList<Float>(0);
ArrayList<Float> rainYPosition = new ArrayList<Float>(0);
ArrayList<Integer> rainXPosition = new ArrayList<Integer>(0);
ArrayList<Character> rainCharacter = new ArrayList<Character>(0);
int THRESHOLD = 128;
boolean DEBUGMODE = false;
double CHANCE_TO_GENERATE = 0.35;
int CHAR_X_OFFSET = 12;

void setup() {
  size(1280, 720);
  inputImage = createImage(width, height, RGB);

  // splitting the original poem into words based on a space delimiter, then also in a second array making note
  // of the index that said word appears in, in the original poem in order to calculate where it should appear
  // along the x-coordiante.
  int idx = 0;
  int temp = 0;
  String l_poem = poem; //local poem variable to edit
  while(temp != -1) {
    temp = l_poem.indexOf(" ");
    /*if(temp < 3) {
      idx = idx + temp + 1;
      l_poem = l_poem.substring(temp + 1);
    } else */if(temp < 0) { //getting the last word in the list
      words.add(l_poem);
      wordIdx.add(idx);
    } else {
      words.add(l_poem.substring(0,temp));
      wordIdx.add(idx);
      idx = idx + temp + 1;
      l_poem = l_poem.substring(temp + 1);
    }
  }
}

// Function to create a random raindrop somewhere along the top of the screen.
void createRaindrop() {
  rainVelocity.add(generateVelocity());
  rainYPosition.add(0.0);
  int index = (int)(Math.random() * poem.length());
  rainXPosition.add(index * CHAR_X_OFFSET);
  rainCharacter.add(poemArr[index]);
}

// Function to create part of a random word, similar to createRaindrop() but is more likely to drop letters in the same word.
void createWordRaindrop() {
  int idx = (int) Math.floor(Math.random() * words.size()); // determining which word we want, (via index)
  double[] probabilities = {0.25d, 0.5d, 0.75d, 1.0d};
  double amountOfWord = probabilities[((int) Math.floor(Math.random() * probabilities.length))];
  String word = words.get(idx); // gets "raindrop word" from master words list.
  if(word.length() < 3) { //if the word is less then 3 letters, just generate the whole word.
    for(int i = 0; i < word.length(); i++) {
      rainVelocity.add(generateVelocity());
      rainCharacter.add(word.charAt(i));
      rainYPosition.add(0.0);
      rainXPosition.add((wordIdx.get(idx) + i) * CHAR_X_OFFSET);
    }
  } else {
    double percentOfWord = 1.0 / word.length();
    boolean[] letterTaken = new boolean[word.length()];
    int count = 0;
    boolean doubleLetter;
    int randCharIdx = -1; //default values for later to satisfy compiler
    int randCharGlobalIdx = -1;
    char randChar = ' ';
    while(count * percentOfWord < amountOfWord) { // will add a random character from the word that we're generating.
      doubleLetter = false;
      //randChar = '-';
      while(!doubleLetter) { //while loop to make sure we don't add a character we've already added
        randCharIdx = (int) Math.floor(Math.random() * word.length()); // generate which letter in the word to add
        randCharGlobalIdx = wordIdx.get(idx) + randCharIdx; // where said letter exists in the entire poem 
        randChar = word.charAt(randCharIdx);
        if(!letterTaken[randCharIdx]) { //if the letter has NOT been taken
          doubleLetter = true;
          letterTaken[randCharIdx] = true;
        }
      }
      rainVelocity.add(generateVelocity());
      rainYPosition.add(0.0);
      rainXPosition.add(randCharGlobalIdx * CHAR_X_OFFSET);
      rainCharacter.add(randChar);
      count++;
    }
  }
}


// Function to generate a random velocity
float generateVelocity() {
  double random = Math.random() / 3;
  double random2 = Math.random();
  if(random2 < 0.5) {
    return 3.0 - (float)random;
  } else {
    return 3.0 + (float)random;
  }
}


void draw() {

  // optimization to remove letters as they fall off the screen.
  for(int i = 0; i < rainYPosition.size(); i++) {
    if(rainYPosition.get(i) > 711) {
      rainYPosition.remove(i);
      rainXPosition.remove(i);
      rainCharacter.remove(i);
      rainVelocity.remove(i);
    }
  }
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
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }

  PImage bw = inputImage.copy();
  // left to right, scan of image and making black version
  for(int i = 0; i < inputImage.height; i++) { // row
    for(int j = 0; j < inputImage.width; j++) { // column
      bw.pixels[inputImage.width * i + j] = thresholdPixel(bw.pixels[inputImage.width * i + j]);
    }
  }

  // This code draws the current camera/movie image to the screen
  // Flipping image pulled from this link:
  // https://processing.org/discourse/beta/num_1203607253.html
  if(DEBUGMODE) {
    pushMatrix();
    set(0, 0, bw);
    scale(-1.0, 1.0);
    image(bw,-bw.width,0);
    popMatrix();
  } else {
    pushMatrix();
    set(0, 0, inputImage);
    scale(-1.0, 1.0);
    image(inputImage,-inputImage.width,0);
    popMatrix();
  }

  // load font
  PFont font;
  font = createFont("EBGaramond-Regular.ttf", 16);
  textFont(font);
  double random = Math.random();

  //try to generate a raindrop, an even smaller chance to generate a word as well.
  if(random < CHANCE_TO_GENERATE) {
    createRaindrop();
    if(random < CHANCE_TO_GENERATE / 6) {
      createWordRaindrop();
    }
  }

    //movement calculation and rendering of text
  for(int i = 0; i < rainYPosition.size(); i++) {
    calculatePositionChange(i, bw);
  }

  for(int i = 0; i < rainYPosition.size(); i++) {
    fill(0,255,0); //set text color to green.
    text(rainCharacter.get(i),rainXPosition.get(i),rainYPosition.get(i));  //render text.
  }
}

void calculatePositionChange(int index, PImage collisionImage) {
  int y = (int) Math.floor(rainYPosition.get(index));
  int x = rainXPosition.get(index);
  if(y < 9) { //can't be pushed off screen, any y less than 9 will push it off screen.
    rainYPosition.set(index,y + rainVelocity.get(index));
  } else {
    boolean collision = false;
    int count = 8;
    color white = #FFFFFF;
    while(!collision && count > -1) {
      PImage collisionRect = collisionImage.get(1272 - x, y + 1 - count, 7, 1); //1x7 horizontal rectangle (width of letter)
      for(int i = 0; i < collisionRect.pixels.length && !collision; i++) { // checking all 1x7 rectangle's which make up the "hitbox" of the number
        if(collisionRect.pixels[i] != white) {
          collision = true;
        }
      }
      // if any of the hitboxes hit a black square we stop and calculate there
      // otherwise we keep moving our hitbox down until we get to the bottom of the number
      if(!collision) {
        count--;
      }
    }
    if(collision) {

      //we collided on the upper edge of the character, we need to shoot upwards if there are more black pixels above our character on those lines.
      if(count > 7) {
        int temp = 0;
        while(collision && (y + 1 - count > 0)) { //if we go less than 0 then we are out of bounds.
          count++;
          PImage collisionRect = collisionImage.get(1272 - x, y + 1 - count, 7, 1); //1x7 horizontal rectangle (width of letter)
          temp = 0;
          for(int i = 0; i < collisionRect.pixels.length; i++) { // checking all
            if(collisionRect.pixels[i] != white) {
              temp++;
            }
          }
          if(temp == 0) { //none of the scanned pixels were black then we have moved character up high enough and we can stop moving it upwards.
            collision = false;
          }
        }
      }

      rainYPosition.set(index,(float)y - count);
    } else {
      //if no collision anywhere we move down 1 velocity unit
      rainYPosition.set(index,rainYPosition.get(index) + rainVelocity.get(index));
    }
  }
}

color thresholdPixel(color inputPixel) {
  if(brightness(inputPixel) >= THRESHOLD) {
    return color(255);
  } else {
    return color(0);
  }
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
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");

        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }

  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // TODO: Fill in your code to handle keypresses here..
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      if(THRESHOLD < 255) {
        THRESHOLD++;
        println("threshold up to:" + THRESHOLD);
      }
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      if(THRESHOLD > 0) {
        THRESHOLD--;
        println("threshold down to:" + THRESHOLD);
      }
    }
  }
  else if (key == ' ') {
    // spacebar pressed
    DEBUGMODE = !DEBUGMODE;
  }
}
