//Written by Bob Clagett - I Like To Make Stuff

//This code is for the Optimus Prime Ion Cannon project at 
//https://www.iliketomakestuff.com/making-optimus-primes-blaster/
//See the link above for a list of the associated hardware and links to everything you'll need.

//This code is provided AS IS. 
//Not support is provided, you're on your own, but you can do it.
//I believe in you!

#include <Adafruit_NeoPixel.h>

#define PIXEL_PIN    2    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 26

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

bool blastNum= HIGH;
bool sfxNum = HIGH;

//pushbutton pins
const int buttonPin = 4;
const int buttonPin1 = 6;

//audio trigger pins
const int audioPin1 = A0; // blaster
const int audioPin2 = A1; //blaster flash
const int audioPin3 = A2; // music
const int audioPin4 = A3; //transform

const int ledPin = 2;
const int ledPin1 = 3;
int ledPinState = HIGH;         // the current state of the output pin
int ledPin1State = HIGH;
int buttonPinState;             // the current reading from the input pin
int buttonPin1State;
int lastButtonPinState = LOW;   // the previous reading from the input pin
int lastButtonPin1State = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

int colorNum = 0;
                  
void setup() {
  pinMode(audioPin1,  OUTPUT);
  pinMode(audioPin2, OUTPUT);
  pinMode(audioPin3,  OUTPUT);
  pinMode(audioPin4, OUTPUT);
  pinMode(buttonPin,  INPUT);
  pinMode(buttonPin1, INPUT);
  pinMode(ledPin,    OUTPUT);
  pinMode(ledPin1,   OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  
    
  // set initial LED state
digitalWrite(audioPin1,HIGH);
digitalWrite(audioPin2,HIGH);
digitalWrite(audioPin3,HIGH);
digitalWrite(audioPin4,HIGH);
  digitalWrite(ledPin,   ledPinState);
  digitalWrite(ledPin1, ledPin1State);
  Serial.begin(9600);
}

void loop() {
  {  
    int reading = digitalRead(buttonPin);
    if (reading != lastButtonPinState) {
      lastDebounceTime = millis();
    } 
 
    if ((millis() - lastDebounceTime) > debounceDelay && reading != buttonPinState ) {
      {
        buttonPinState = reading;
        if (buttonPinState == HIGH) {
          ledPinState = !ledPinState;
          Serial.println("press1");
          colorNum++;
          int colors[] = {
           strip.Color(50, 255, 50),
           strip.Color(50, 50, 255),
           strip.Color(255, 255, 255)
          };
          if(colorNum>3){
            colorNum = 0;
          }
          Serial.println(colorNum);
          colorBlast(colors[colorNum], 50);
        }
      } 
    }
    lastButtonPinState = reading;
  
  
    reading = digitalRead(buttonPin1);
    if(reading!=lastButtonPin1State){
      lastDebounceTime=millis();
    }

    if ((millis() - lastDebounceTime) > debounceDelay && reading != buttonPin1State ) {
      {
        buttonPin1State=reading;
        if(buttonPin1State==HIGH){
          ledPinState = !ledPin1State;
          //trigger sound only
          int audio;
            sfxNum = !sfxNum;
            if (sfxNum==HIGH){
              audio = audioPin3;
            }else{
              audio = audioPin4;
            }
          digitalWrite(audio,LOW);
          delay(500);
          digitalWrite(audio,HIGH);
        }
      }
    }
    lastButtonPin1State=reading;
  }

  // set the LED:
  digitalWrite(ledPin, ledPinState);
  digitalWrite(ledPin1, ledPin1State);

}

// Fill the dots one after the other with a color
void colorBlast(uint32_t c, uint8_t wait) {
  
  uint16_t flashes;
  int cutOff, audio, flashDelay, startDelay;
  blastNum = !blastNum;
  if (blastNum==HIGH){
    audio = audioPin1;
    flashes = 1;
    flashDelay = 500;
    startDelay = 0;
  }else{
    audio = audioPin2;
    flashes = 5;
    flashDelay = 50;
    startDelay = 50;
  }
  
  digitalWrite(audio,LOW);
  delay(startDelay);
  for(uint16_t k=0; k<flashes; k++) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
    }
    delay(flashDelay);
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    delay(flashDelay);
    //after correct sound delay, LOW audio pin
    if(flashDelay*flashes>200){
      digitalWrite(audio,HIGH);
    }
  }
}

