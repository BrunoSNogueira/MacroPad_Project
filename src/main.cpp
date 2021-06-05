/*
@project MacroPad
@file main.cpp
@author Pedro Zamora

2021-06-04 - Created, based on tinyboatproductions instructables (Macropad V2) and Mark Stanley Multikey.ino
  -- Hardware Modifications: Leds Removed;
  -- Code major modifications: Adjustments on debouncing, multikey features, allows change of music to previous, enables '*' key to also be used.
  -- Code minor mods: removed Layouts, for now (but as a choice based on my usage). And a lot of code-structure remixes. 

*/

#include <Arduino.h>
#include <Keypad.h>

#include <Bounce2.h>
#include <HID-Project.h> // PlatformIO Users: also include 'HID @ ^1.0' in platformIO.ini lib_deps.
#include <Encoder.h>

// Setting Keypad buttons...
const int R1 = 6;  // top line
const int R2 = 7;  // second line
const int R3 = 8;  // third line
const int R4 = 9;  // botton line
const int C1 = 15; // left collumn
const int C2 = 14; // middle collumn
const int C3 = 16; // right collumn

const byte ROWS = 4; // Total Rows
const byte COLS = 3; // Total Cols

char keys[ROWS][COLS] = {
{'X','0','*'}, // In my project, 'X' is unused. Encoder is in that physical place.
{'7','8','9'},
{'4','5','6'},
{'1','2','3'}
};

int changeButton;

byte rowPins[ROWS] = {R1, R2, R3, R4};
byte colPins[COLS] = {C1, C2, C3};

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// Keypad Setup: Ok!


// Setting encoder...
int DT = 2;
int CLK = 3;
Encoder volumeKnob(DT,CLK);

int SW = 4; // "Clicky Button" (switch) of the encoder
Bounce encoderButton = Bounce(SW, 40);

int timeLimit = 150;
long oldPosition;
long newPosition;
int rollDelta = 2; //Well, sometimes we roll it without wanting to. So, let's give us some space.
// Encoder Setup: Ok!

// Layouts Setup...
const int numLayouts = 2; //If you add Layouts, remember to increase this value!
int currentLayout;
// Layout Setup: Ok!

// Other... 
String msg;
//Others: Ok!


void setup() {

    Serial.begin(9600);

    msg = "";

    pinMode(CLK, INPUT_PULLUP);
    oldPosition = volumeKnob.read();
    newPosition = oldPosition;

    changeButton = 0;
    currentLayout = 0;

}

void changeLayout(){
  
  currentLayout++;

  if (currentLayout == numLayouts){
    currentLayout = 0;
  }
  
  Serial.print("Layout Changed. Current Layout: "); 
  Serial.println(currentLayout);
  delay(100);
  return;
}

void Layout1(char button){
  // Numpad Layout

  switch(button){
    case '1':
      Keyboard.print('1');
      break;
    case '2':
      Keyboard.print('2');
      break;
    case '3':
      Keyboard.print('3');
      break;
    case '4':
      Keyboard.print('4');
      break;
    case '5':
      Keyboard.print('5');
      break;
    case '6':
      Keyboard.print('6');
      break;
    case '7':
      Keyboard.print('7');
      break;
    case '8':
      Keyboard.print('8');
      break;
    case '9':
      Keyboard.print('9');
      break;
    case '0':
      Keyboard.print('0');
      break;
    case '*':
      // Tip: I used F13 to mute my discord Mic. As so, all of my '*' in the Layouts will have this key fixed. 
      Keyboard.press(KEY_F13);
      Keyboard.releaseAll();
      break;
  };
}

void Layout2(char button){
  // FullMacro Layout

  switch(button){
    case '1':
      Keyboard.press(KEY_F14);
      Keyboard.releaseAll();
      break;
    case '2':
      Keyboard.press(KEY_F15);
      Keyboard.releaseAll();
      break;
    case '3':
      Keyboard.press(KEY_F16);
      Keyboard.releaseAll();
      break;
    case '4':
      Keyboard.press(KEY_F17);
      Keyboard.releaseAll();
      break;
    case '5':
      Keyboard.press(KEY_F18);
      Keyboard.releaseAll();
      break;
    case '6':
      Keyboard.press(KEY_F19);
      Keyboard.releaseAll();
      break;
    case '7':
      Keyboard.press(KEY_F20);
      Keyboard.releaseAll();
      break;
    case '8':
      Keyboard.press(KEY_F21);
      Keyboard.releaseAll();
      break;
    case '9':
      Keyboard.press(KEY_F22);
      Keyboard.releaseAll();
      break;
    case '0':
      Keyboard.press(KEY_F23);
      Keyboard.releaseAll();
      break;
    case '*':
      // Tip: I used F13 to mute my discord Mic. As so, all of my '*' in the Layouts will have this key fixed.
      Keyboard.press(KEY_F13);
      Keyboard.releaseAll();
      break;
  };
}


void musicalChoice(){
  // Enables you to pause/start or change music. Single-press to start/stop, press-and-turn encoder to change the track.
  // Turning clockwise takes you to next song. Otherwise, you get the previous one.

  if(encoderButton.update()) {
        if(encoderButton.fell()) {
          
          while(!encoderButton.update()){}
          if(encoderButton.rose()){

            newPosition = volumeKnob.read();

              if(newPosition - oldPosition < (-1*rollDelta)){
                Consumer.write(MEDIA_PREV);
                Serial.print("Previous");
              }
              else if (newPosition - oldPosition > rollDelta){
                Consumer.write(MEDIA_NEXT);
                Serial.print("Next");
              }
              else { 
                //Range between -delta and +delta, including 0
                Consumer.write(MEDIA_PLAY_PAUSE);
                Serial.print("Play/Pause");
              }

              oldPosition = newPosition;
          }
          Keyboard.releaseAll();
        }
      }
}

void volumeAlter(){
  // Allows you to tune your volume, while the encoder is not pressed.
    
    newPosition = volumeKnob.read();

    if(newPosition != oldPosition){
      delay(timeLimit);
      newPosition = volumeKnob.read();

      if((newPosition - oldPosition) > (rollDelta)) {
        Serial.println("volume up");
        Consumer.write(MEDIA_VOLUME_UP);
      } 
      else if ((newPosition - oldPosition) < (-1 * rollDelta)){
        Serial.println("volume down");
        Consumer.write(MEDIA_VOLUME_DOWN);                               
      }
      Keyboard.releaseAll();
      oldPosition = volumeKnob.read();    
    }    
    
}

void sendKey(char key){
    
    switch(currentLayout){
    case 0:
      Layout1(key);
      break;
    case 1:
      Layout2(key);
      break;
    default: 
      // Shoudn't happen. But, in any case, better safe than sorry.
      Layout1(key);
      break;
    }
}

void loop() {

    // Fills kpd.key[ ] array with up-to 10 active keys. Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    msg = " PRESSED.";
                    if (kpd.key[i].kchar != '*'){
                      sendKey(kpd.key[i].kchar);
                    }
                break;
                    case HOLD:
                    msg = " HOLD.";
                    if (kpd.key[i].kchar == '*'){
                      changeLayout();
                      changeButton = 1;
                    }
                break;
                    case RELEASED:
                    msg = " RELEASED.";
                    if ((kpd.key[i].kchar == '*') & (changeButton == 0)){
                      sendKey(kpd.key[i].kchar);
                    }
                break;
                    case IDLE:
                    msg = " IDLE.";
                    if (kpd.key[i].kchar == '*'){
                      changeButton = 0;
                    }
                }

                Serial.print("Key ");
                Serial.print(kpd.key[i].kchar);
                Serial.println(msg);
            }
        }
    }
    else
    {
      musicalChoice();
      volumeAlter();
    }

}