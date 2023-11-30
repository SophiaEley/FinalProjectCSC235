#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip(25, 3);

// my state var
char state = 0;

// button vars
int buttonPin = 5;
boolean currentState = false;
boolean prevState = false;


const int MOTION_SENSOR_PIN = 7;   
const int lHLed           = 11;   
const int rHLed           = 10;
const int lRLed           = 9;
const int rRLed           = 8;
int motionStateCurrent      = LOW; 
int motionStatePrevious     = LOW; 

void setup() {
  Serial.begin(9600);                
  pinMode(MOTION_SENSOR_PIN, INPUT); 
  pinMode(lHLed, OUTPUT);          
  pinMode(rHLed, OUTPUT);
  pinMode(lRLed, OUTPUT);
  pinMode(rRLed, OUTPUT);
  pinMode(buttonPin, INPUT);
  strip.begin();
  strip.clear();
  strip.show();
}

void loop() {
  motionStatePrevious = motionStateCurrent;             
  motionStateCurrent  = digitalRead(MOTION_SENSOR_PIN); 

  if (motionStatePrevious == LOW && motionStateCurrent == HIGH) { 
    Serial.println("Motion detected!");
    digitalWrite(lHLed, HIGH); 
    digitalWrite(rHLed, HIGH);
    digitalWrite(lRLed, HIGH);
    digitalWrite(rRLed, HIGH);
    delay (2000);
  }
  else
  if (motionStatePrevious == HIGH && motionStateCurrent == LOW) { 
    Serial.println("Motion stopped!");
    digitalWrite(lHLed, LOW);  
    digitalWrite(rHLed, LOW);
    digitalWrite(lRLed, LOW);
    digitalWrite(rRLed, LOW);
    delay(2000);
  }
  prevState = currentState;
  currentState = debounce(buttonPin, prevState);

  //if you push the button this frame
  if (currentState == true && prevState == false){
    state++;
    Serial.println("press");
  }

  switch (state){
    case 0:
     bulletTrain(strip.Color(0,255,255), 750 );
    break;
    case 1:
      threeColorSweep(strip.Color(255,0,0), strip.Color(0,255,0), strip.Color(0,0,255), 100);
    break;
    case 2:
      bouncingBrightness(strip.Color(0, 255, 255), 25);
    break;
    default:
      strip.setPixelColor(0,255,0,0);
    break;
  }

  // if state is too big, start over
  if (state > 2){
    state = 0;
  }
}

void bulletTrain(uint32_t aColor, int interval) {
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  static int index = 0;

  if (currentTime - startTime >= interval) {
    strip.setPixelColor(index, aColor);
    strip.setPixelColor(index - 1, 0, 0, 0);
    index++;
    strip.show();
    strip.clear();

    //reset timer, so start time = millis
    startTime = millis();

    //reset moving color when it reaches the end
    if (index >= strip.numPixels()) {
      index = 0;
      strip.clear();
    }
  }
}

void bouncingLight(uint32_t aColor, int interval) {
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  static int index = 0;
  static int direction = 1;

  if (currentTime - startTime >= interval) {
    strip.setPixelColor(index, aColor);
    strip.setPixelColor(index - 1, 0, 0, 0);
    index = index + direction;
    strip.show();
    strip.clear();

    //reset timer, so start time = millis
    startTime = millis();

    //reset moving color when it reaches the end
    if (index >= strip.numPixels()) {
      direction = -1;
      strip.clear();
    }
    if (index < 0) {
      direction = 1;
    }
  }
}

void bouncingBrightness(uint32_t aColor, int interval) {
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  static int index = 0;
  static int direction = 1;

  if (currentTime - startTime >= interval) {
    strip.setBrightness(index);
    strip.fill(strip.Color(0, 255, 255));
    index = index + direction;
    strip.show();
    strip.clear();
    Serial.println(index);


    //reset timer, so start time = millis
    startTime = millis();

    //reset moving color when it reaches the end
    if (index >= 100) {
      direction = -1;
      strip.clear();
    }
    if (index <= 0) {
      direction = 1;
    }
  }
}
void threeColorSweep(uint32_t c1, uint32_t c2, uint32_t c3, int interval) {
  static unsigned long startTime = millis();
  unsigned long currentTime = millis();

  // static int index = 0;
  // static int direction = 1;
  static int state = 0;

  if (currentTime - startTime >= interval) {

    state++;

    //reset timer, so start time = millis
    startTime = millis();
  }
  if(state == 0){
    for (int i = 0; i < strip.numPixels(); i++){
   if (i%3 == 0){
      strip.setPixelColor(i, c1);
    }
    else if (i%3 == 1){
      strip.setPixelColor(i, c2);
    } 
    else if (i%3 == 2){
      strip.setPixelColor(i, c3);
    }
  }
  strip.show();
  }
  if(state == 1){
    for (int i = 0; i < strip.numPixels(); i++){
   if (i%3 == 0){
      strip.setPixelColor(i, c2);
    }
    else if (i%3 == 1){
      strip.setPixelColor(i, c3);
    } 
    else if (i%3 == 2){
      strip.setPixelColor(i, c1);
    }
  }
  strip.show();
  }
  if(state == 2){
    for (int i = 0; i < strip.numPixels(); i++){
   if (i%3 == 0){
      strip.setPixelColor(i, c3);
    }
    else if (i%3 == 1){
      strip.setPixelColor(i, c1);
    } 
    else if (i%3 == 2){
      strip.setPixelColor(i, c2);
    }
  }
  strip.show();
  state = 0;
  }

}

boolean debounce(int aButton, boolean aprevState) {
  // track the button's current state
  boolean aButtonState = digitalRead(aButton);

  // if you pressed the button this frame, then
  // wait so you don't read the button again
  // while it is bouncing
  if (aButtonState == HIGH && aprevState == LOW) {
    delay(15);
  }
  // return the button's current state
  return aButtonState;
}

