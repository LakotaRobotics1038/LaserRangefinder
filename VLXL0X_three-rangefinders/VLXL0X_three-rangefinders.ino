//#include <Adafruit_VL53L0X.h>

/* This example shows how to get single-shot range
 measurements from the VL53L0X. The sensor can optionally be
 configured with different ranging profiles, as described in
 the VL53L0X API user manual, to get better performance for
 a certain application. This code is based on the four
 "SingleRanging" examples in the VL53L0X API.

 The range readings are in units of mm. */

#include <Wire.h>
#include <VL53L0X.h>

VL53L0X sensor;


// Uncomment this line to use long range mode. This
// increases the sensitivity of the sensor and extends its
// potential range, but increases the likelihood of getting
// an inaccurate reading because of reflections from objects
// other than the intended target. It works best in dark
// conditions.

#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
//#define HIGH_ACCURACY

//Set the maximum range - 8.190 meters converted to inches.
float laserMaxRange = 8190 * 0.0393701;

//Sonar Pins
const int leftTRIG_PIN = 4;
const int leftECHO_PIN = 3;
const int rightTRIG_PIN = 6;
const int rightECHO_PIN = 5;


void setup()
{
  //Front laser:
  Serial.begin(9600);
  Wire.begin();
  sensor.init();
  sensor.setTimeout(500);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

//Set the timing budget to 100ms - a midpoint between speed and accuracy.
 sensor.setMeasurementTimingBudget(100000);

//Side sonar
  // The Trigger pin will tell the sensor to range find
  pinMode(leftTRIG_PIN, OUTPUT);
  digitalWrite(leftTRIG_PIN, LOW);
  pinMode(rightTRIG_PIN, OUTPUT);
  digitalWrite(rightTRIG_PIN, LOW);
 
}

void loop()
{
  //Laser distance sensing:
  float laserTt = sensor.readRangeSingleMillimeters();
  //Convert to inches
  laserTt = laserTt * 0.0393701;

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if (laserTt < (laserMaxRange) ) 
  {
    //Serial.print(" Measure : "); 
    //Serial.println(tt);
  }

 if(Serial.available() > 0)
 {
    char letter = Serial.read();
    if(letter == 'M')
    {
     digitalWrite(13,HIGH);
    }
     if(letter == 'N')
    {
     digitalWrite(13,LOW);
    }
 }



 //Sonar distance sensing:
  unsigned long leftT1;
  unsigned long leftT2;
  unsigned long leftPulse_width;
  float leftInches;
  unsigned long rightT1;
  unsigned long rightT2;
  unsigned long rightPulse_width;
  float rightInches;

  digitalWrite(leftTRIG_PIN, HIGH);
  delayMicroseconds(50);
  digitalWrite(leftTRIG_PIN, LOW);
  digitalWrite(rightTRIG_PIN, HIGH);
  delayMicroseconds(50);
  digitalWrite(rightTRIG_PIN, LOW);

//Left
  //wait for pulse on left pin
  while ( digitalRead(leftECHO_PIN) == 0 );
  // Measure how long the echo pin was held high (pulse width)
  leftT1 = micros();
  while ( digitalRead(leftECHO_PIN) == 1);
  leftT2 = micros();
  leftPulse_width = leftT2 - leftT1;
  leftInches = leftPulse_width / 148.0;

//Right

  while ( digitalRead(rightECHO_PIN) == 0 );
  // Measure how long the echo pin was held high (pulse width)
  rightT1 = micros();
  while ( digitalRead(rightECHO_PIN) == 1);
  rightT2 = micros();
  rightPulse_width = rightT2 - rightT1;
  rightInches = rightPulse_width / 148.0;



//Output results:
  String output = "Front: ";
  output.concat(laserTt);
  output.concat("; Left: ");
  output.concat(leftInches);
  output.concat("; Right: ");
  output.concat(rightInches);
  Serial.println(output);

//Wait before running again
  delay(100);

}



