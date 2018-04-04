/*
Sketch for controlling light seeking robot.
Senses light color temp via TCS34725. TCS 34725 setup for arduino nano:

   Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 5V DC or 3.3 to 3.3VDC
   Connect GROUND to common ground 
   Connect LED to ground

Senses light intensity via two photoresistors. Left PR is connected to pin 0.
Right is connected to pin 1. Basic PR wiring:

           PhotoR     47Kohm
 +5    o---/\/\/--.--/\/\/---o GND
                  |
 Pin 0 o-----------

Motor is controlled via the nano motor shield. Right wheel is connected A, left to B. 
To control the Right wheel:

  Channel 3 sets PWM signal from 0-255 to control speed. Channel 12 controls direction -
  HIGH is forward,  LOW is reverse. Channel 8 is the brake.

To control the Left wheel:

  Channel 3 sets PWM signal from 0-255 to control speed. Channel 12 controls direction -
  HIGH is forward,  LOW is reverse. Channel 8 is the brake.
*/


#include <Adafruit_TCS34725.h>
#include <Wire.h>

int PR_LEFT = 0;   // left photoresistor at analog pin0
int PR_RIGHT = 1;  // right photoresistor at analog pin1
int left_level, right_level; // stores the analog PR readings

/* Initialise with default values (int time = 2.4ms, gain = 1x) */
//Adafruit_TCS34725 tcs = Adafruit_TCS34725();
/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void setup() {

  Serial.begin(9600);
  
  pinMode(12,OUTPUT);     //Channel A (RIGHT) Direction Pin Initialize
  pinMode(13,OUTPUT);     //Channel B (LEFT) Direction Pin Initialize

// print sensor status while connected to PC
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }

}

void loop() {

  uint16_t r, g, b, c, lux, colorTemp;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);
  
// read levels from photoresistors and assign this value to the left_ and right_level variables
// serial print for testing while connected to PC
  left_level = analogRead(PR_LEFT);
  right_level = analogRead(PR_RIGHT);

  
  Serial.print("left_level: "); Serial.print(left_level, DEC); Serial.print(" ");
  Serial.print("right_level: "); Serial.print(right_level, DEC); Serial.print(" ");
  Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  Serial.print("B: "); Serial.print(b, DEC); Serial.print(" \n");

// Tell robot to move forward if the color sensor reads the appropriate color (r, g, b) & the photoresistors
// read approximately the same intensity.
    if ( (left_level > (right_level - 10)) && (left_level < (right_level + 10)) && (r > 1000)){
    analogWrite(3, 0);
    analogWrite(11, 0);
    delay(1000);
    digitalWrite(12, HIGH);
    digitalWrite(13, LOW);
    analogWrite(3, 150);
    analogWrite(11, 150);
  }

// Tell robot to turn right if right PR is 10 higher than left.
  else if ( (left_level < (right_level - 10)) && (r > 750) ){
    analogWrite(3, 0);
    analogWrite(11, 0);
    delay(1000);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
    analogWrite(3, 150);
    analogWrite(11, 150);    
  }

// Tell robot to turn left if left PR is 10 higher than right.
  else if ( (left_level > (right_level + 10)) && (r > 750) ){
    analogWrite(3, 0);
    analogWrite(11, 0);
    delay(1000);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    analogWrite(3, 150);
    analogWrite(11, 150);    
  }

/*
// Rotate left to search for red light
  else if ( (r < 750)){
    analogWrite(3, 0);
    analogWrite(11, 0);
    delay(1000);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
    analogWrite(3, 150);
    analogWrite(11, 150);
  }
*/

// Don't move if red light is low.
  else if (r<750){
    analogWrite(3, 0);
    analogWrite(11, 0);
    delay(1000);
    digitalWrite(12, 0);
    digitalWrite(13, 0);
    analogWrite(3, 0);
    analogWrite(11, 0); 
  }

}


