#include<CountUpDownTimer.h>
#include <Adafruit_NeoPixel.h>

#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5
#define PIN5 6
#define NUM_LEDS 168
//Segment Length is 24 Pixels
//   A
// F   B
//   G
// E   C
//   D
#define SEGA 0
#define SEGB 24
#define SEGC 48
#define SEGD 72
#define SEGE 96
#define SEGF 120
#define SEGG 144
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUM_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUM_LEDS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(NUM_LEDS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(NUM_LEDS, PIN4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(NUM_LEDS, PIN5, NEO_GRB + NEO_KHZ800);

CountUpDownTimer T(DOWN, HIGH); // Default precision is HIGH, but you can change it to also be LOW

int digitMap[10][NUM_LEDS];
uint16_t Index; // current step within the pattern

void setup()
{
  Serial.begin(115200);
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
  strip3.show(); // Initialize all pixels to 'off'
  strip4.show(); // Initialize all pixels to 'off'
  strip5.show(); // Initialize all pixels to 'off'
  calculateDigitMap();
  randomSeed(analogRead(0));
  Index = 0;
  T.SetTimer(4, 16, 52);   //start at 4 hours (USE FOR: DOWN ONLY)
  T.SetStopTime(0, 0, 0); // stop at 0 seconds (USE FOR: UP/DOWN)
  T.StartTimer();
}

void loop()
{
  T.Timer(); // run the timer

  if (T.TimeHasChanged() ) // this prevents the time from being constantly shown.
  {
    Serial.print(T.ShowHours());
    Serial.print(":");
    Serial.print(T.ShowMinutes());
    Serial.print(":");
    Serial.println(T.ShowSeconds());
    // This DOES NOT format the time to 0:0x when seconds is less than 10.
    // if you need to format the time to standard format, use the sprintf() function.
    drawAllSegments();
  }
}

void showStrip() {
  T.Timer(); // run the timer
  drawDigits(); //Turn unwanted pixels black
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip5.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip1.setPixelColor(Pixel, strip1.Color(red, green, blue));
  strip2.setPixelColor(Pixel, strip2.Color(red, green, blue));
  strip3.setPixelColor(Pixel, strip3.Color(red, green, blue));
  strip4.setPixelColor(Pixel, strip4.Color(red, green, blue));
  strip5.setPixelColor(Pixel, strip5.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void setAllPosition(byte red, byte green, byte blue, int pos){
  switch(pos){
    case 1:
      for (int i = 0; i < NUM_LEDS; i++ ) {
        strip1.setPixelColor(i, strip1.Color(red, green, blue));
      }
      break;
    case 2:
      for (int i = 0; i < NUM_LEDS; i++ ) {
        strip2.setPixelColor(i, strip2.Color(red, green, blue));
      }
      break;
    case 3:
      for (int i = 0; i < NUM_LEDS; i++ ) {
        strip3.setPixelColor(i, strip3.Color(red, green, blue));
      }
      break;
    case 4:
      for (int i = 0; i < NUM_LEDS; i++ ) {
        strip4.setPixelColor(i, strip4.Color(red, green, blue));
      }
      break;  
    case 5:
      for (int i = 0; i < NUM_LEDS; i++ ) {
        strip5.setPixelColor(i, strip5.Color(red, green, blue));
      }
      break;      
  }
  showStrip();
}

void RGBNoise(){
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, random(255), random(255), random(255));
  }
  showStrip();
}

void RGBSparkle(){
  for (int i = 0; i < NUM_LEDS; i++) {
    setPixel(i, random(255), random(255), random(255));
  }
  for (int i = 0; i < NUM_LEDS/2; i++) {
    setPixel(random(NUM_LEDS), 0, 0, 0);
  }
  showStrip();
}

void rainbowCycleMod(){
  for(int i=0; i< NUM_LEDS; i++)
  {
    strip1.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + Index) & 255));
    strip2.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + Index) & 255));
    strip3.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + Index) & 255));
    strip4.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + Index) & 255));
    strip5.setPixelColor(i, Wheel(((i * 256 / NUM_LEDS) + Index) & 255));
    if (Index < NUM_LEDS){
      Index++;
    }
    else { 
      Index = 0;
    }
  }
  showStrip();
}

void rainbowCycle(int SpeedDelay) {
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c=Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      setPixel(i, *c, *(c+1), *(c+2));
    }
    showStrip();
    delay(SpeedDelay);
  }
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}

void drawAllSegments() {
    T.Timer(); // run the timer
    if (T.ShowHours() == 4){
    //Set All
    setAll(0xff,0,0);
    }
    else if (T.ShowHours() == 3){
      if (T.ShowMinutes() >= 30){
        setAll(0,0xff,0);
        }
      else {
        setAll(0xff,0,0);
      }
    }
    else if (T.ShowHours() == 2){
      if (T.ShowMinutes() >= 30){
        setAll(0,0,0xff);
      }
      else {
        setAll(0xff,0,0xff);
      }
    }
    else if (T.ShowHours() == 1){
      int r,g,b;
      r = map(T.ShowMinutes(),30,59,0,255);
      g = map(T.ShowSeconds(),0,59,0,255);
      b = map(T.ShowMilliSeconds(),0,1000,0,255);
      setAll(r,g,b);
    }
    else if (T.ShowHours() == 0){
      if (T.ShowMinutes() >= 10){
        RGBNoise();
      }
      else if (T.ShowMinutes() < 10 && T.ShowMinutes() > 0){
        RGBSparkle();
      }
      else if (T.ShowMinutes() == 0){
        if (T.ShowSeconds() == 0){
          rainbowCycle(0);
        }
        else{
          RGBSparkle();
        }
      }
    }
}

void drawDigits() {
  //Blackout unused segments
  for (int j = 0; j < NUM_LEDS; j++) {
    if (digitMap[T.ShowHours()][j] == 0) {
      strip1.setPixelColor(j, 0, 0, 0);
    }
  }
  for (int j = 0; j < NUM_LEDS; j++) {
    int firstDigit = (int)(T.ShowMinutes() / 10) % 10;
    if (digitMap[firstDigit][j] == 0) {
      strip2.setPixelColor(j, 0, 0, 0);
    }
  }
  for (int j = 0; j < NUM_LEDS; j++) {
    int lastDigit = T.ShowMinutes() % 10;
    if (digitMap[lastDigit][j] == 0) {
      strip3.setPixelColor(j, 0, 0, 0);
    }
  }
  for (int j = 0; j < NUM_LEDS; j++) {
    int firstDigit = (int)(T.ShowSeconds() / 10) % 10;
    if (digitMap[firstDigit][j] == 0) {
      
      strip4.setPixelColor(j, 0, 0, 0);
    }
    Serial.print(T.ShowSeconds());
    Serial.print(" First: ");
    Serial.println(firstDigit);
  }
  for (int j = 0; j < NUM_LEDS; j++) {
    int lastDigit = T.ShowSeconds() % 10;
    if (digitMap[lastDigit][j] == 0) {
      strip5.setPixelColor(j, 0, 0, 0);
    }
    Serial.print(T.ShowSeconds());
    Serial.print(" Last: ");
    Serial.println(lastDigit);
  }
}

void calculateDigitMap() {
  //[digit][pixel index]
  for (int i = 0; i < 10; i++) { //Digit Number
    //Serial.print(i);
    for (int j = 0; j < NUM_LEDS; j++) { //Pixel Number
      digitMap[i][j] = 1;
      switch (i) {
        case 0 :
          if (j >= SEGG) {
            digitMap[i][j] = 0;
          }
          break;
        case 1 :
          if (j < SEGB) {
            digitMap[i][j] = 0;
          }
          if (j >= SEGD) {
            digitMap[i][j] = 0;
          }
          break;
        case 2 :
          if (j >= SEGC && j < SEGD) {
            digitMap[i][j] = 0;
          }
          if (j >= SEGF && j < SEGG) {
            digitMap[i][j] = 0;
          }
          break;
        case 3 :
          if (j >= SEGE && j < SEGG) {
            digitMap[i][j] = 0;
          }
          break;
        case 4 :
          if (j < SEGB) {
            digitMap[i][j] = 0;
          }
          if (j >= SEGD && j < SEGF) {
            digitMap[i][j] = 0;
          }
          break;
        case 5 :
          if (j >= SEGB && j < SEGC) {
            digitMap[i][j] = 0;
          }
          if (j >= SEGE && j < SEGF) {
            digitMap[i][j] = 0;
          }
          break;
        case 6 :
          if (j >= SEGB && j < SEGC) {
            digitMap[i][j] = 0;
          }
          break;
        case 7 :
          if (j >= SEGD) {
            digitMap[i][j] = 0;
          }
          break;
        case 8 :
          break;
        case 9 :
          if (j >= SEGD && j < SEGF) {
            digitMap[i][j] = 0;
          }
          break;
      }
      //Serial.print( digitMap[i][j]);
    }
    //Serial.println();
  }
}

