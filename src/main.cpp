/*
@project MacroPad
@file main.cpp
@author Bruno Nogueira

2021-06-04 - Created, based on tinyboatproductions instructables (Macropad V2) and Mark Stanley Multikey.ino
  -- Hardware Modifications: Leds Removed;
  -- Code major modifications: Adjustments on debouncing, multikey features, allows change of music to previous, enables '*' key to also be used.
  -- Code minor mods: removed Layouts, for now (but as a choice based on my usage). And a lot of code-structure remixes. 

2021-09-12 - Changed some functionalities from the previous version.
  -- Code major modifications: Altered profile switching to the encoder rotation and the keys in each profile;
  -- Code minor mods: Orginized and remixed the code again;
*/

// Setting Keypad buttons...
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

void setup() {
    Serial.begin(9600);

    pinMode(CLK, INPUT_PULLUP);
    oldPosition = volumeKnob.read();
    newPosition = oldPosition;

    //changeButton = 0;
    currentLayout = 0;
}

void Profile1(char button){
  switch(button){
    case '1':
      Keyboard.press(KEY_F13);
      Keyboard.releaseAll();
      break;
    case '2':
      Keyboard.press(KEY_F14);
      Keyboard.releaseAll();
      break;
    case '3':
      Keyboard.press(KEY_F15);
      Keyboard.releaseAll();
      break;
    case '4':
      Keyboard.press(KEY_F16);
      Keyboard.releaseAll();
      break;
    case '5':
      Keyboard.press(KEY_F17);
      Keyboard.releaseAll();
      break;
    case '6':
      Keyboard.press(KEY_F18);
      Keyboard.releaseAll();
      break;
    case '7':
      Keyboard.press(KEY_F19);
      Keyboard.releaseAll();
      break;
    case '8':
      Keyboard.press(KEY_F20);
      Keyboard.releaseAll();
      break;
    case '9':
      Keyboard.press(KEY_F21);
      Keyboard.releaseAll();
      break;
    case '0':
      Keyboard.press(KEY_F22);
      Keyboard.releaseAll();
      break;
    case '*':
      Keyboard.press(KEY_F23);
      Keyboard.releaseAll();
      break;
  };
}

void Profile2(char button){
  switch(button){
    case '1':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F13);
      Keyboard.releaseAll();
      break;
    case '2':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F14);
      Keyboard.releaseAll();
      break;
    case '3':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F15);
      Keyboard.releaseAll();
      break;
    case '4':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F16);
      Keyboard.releaseAll();
      break;
    case '5':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F17);
      Keyboard.releaseAll();
      break;
    case '6':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F18);
      Keyboard.releaseAll();
      break;
    case '7':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F19);
      Keyboard.releaseAll();
      break;
    case '8':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F20);
      Keyboard.releaseAll();
      break;
    case '9':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F21);
      Keyboard.releaseAll();
      break;
    case '0':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F22);
      Keyboard.releaseAll();
      break;
    case '*':
      Keyboard.press(HID_KEYBOARD_LEFT_ALT);
      Keyboard.press(KEY_F23);
      Keyboard.releaseAll();
      break;
  };
}

void selectProfile(char key){
    switch(currentLayout){
    case 0:
      Profile1(key);
      break;
    case 1:
      Profile2(key);
      break;
    default: 
      // Shoudn't happen, but it's better to be safe than sorry.
      Profile1(key);
      break;
    }
}

// Verifies if any of the keys are being pressed
void keyPress(){
    // Fills kpd.key[ ] array with up-to 10 active keys. Returns true if there are ANY active keys.
    if (kpd.getKeys()){
        for (int i=0; i<LIST_MAX; i++){   // Scan the whole key list.
            if ( kpd.key[i].stateChanged ){   // Only find keys that have changed state.
                if(kpd.key[i].kstate == PRESSED || HOLD)
                    selectProfile(kpd.key[i].kchar);
            }
        }
    }
}

void changeProfile(int rotacao){
    currentLayout += rotacao;

    if (currentLayout == numLayouts){
        currentLayout = 0;
    } else if (currentLayout < 0){
        currentLayout = numLayouts - 1;
    }
  
    Serial.print("Layout Changed. Current Layout: "/*, currentLayout*/);
    delay(100);
    return;
}

void encoderRotate(){
    newPosition = volumeKnob.read();
    
    if(newPosition != oldPosition){
          delay(timeLimit);
          newPosition = volumeKnob.read();

      if((newPosition - oldPosition) > (rollDelta)) {
          changeProfile(1);
      } 
      else if ((newPosition - oldPosition) < (-1 * rollDelta)){
          changeProfile(-1);                               
      }
      Keyboard.releaseAll();
      oldPosition = volumeKnob.read();    
    }    
}

void encoderPress(){
  if(encoderButton.update()){
    if(encoderButton.fell())
        Keyboard.press(KEY_F24);  
    else if(encoderButton.rose())
        Keyboard.releaseAll(); 
  }
}

void loop() {
    keyPress();
    encoderRotate();
    encoderPress();
}
