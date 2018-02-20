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

//#define LONG_RANGE


// Uncomment ONE of these two lines to get
// - higher speed at the cost of lower accuracy OR
// - higher accuracy at the cost of lower speed

//#define HIGH_SPEED
#define HIGH_ACCURACY


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  sensor.init();
  sensor.setTimeout(5000);

#if defined LONG_RANGE
  // lower the return signal rate limit (default is 0.25 MCPS)
  sensor.setSignalRateLimit(0.1);
  // increase laser pulse periods (defaults are 14 and 10 PCLKs)
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
  sensor.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
#endif

#if defined HIGH_SPEED
  // reduce timing budget to 20 ms (default is about 33 ms)
  sensor.setMeasurementTimingBudget(20000);
#elif defined HIGH_ACCURACY
  // increase timing budget to 200 ms
  sensor.setMeasurementTimingBudget(200000);
#endif
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);
pinMode(A3,INPUT);

}

void loop()
{
  
float sensor0v = analogRead(A0);
float sensor1v = analogRead(A1);
float sensor2v = analogRead(A2);
float sensor3v = analogRead(A3);

sensor0v = sensor0v * 0.497;
sensor1v = sensor1v * 0.497;
sensor2v = sensor2v * 0.497;
sensor3v = sensor3v * 0.497;
int tt = sensor.readRangeSingleMillimeters();

   if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if (tt < 8190 ) {
  //Serial.print(" Measure : "); 
  Serial.print(tt);
  Serial.print("|");
  Serial.print(tt * .0393701 * .9);Serial.print("|");
  Serial.print(sensor0v);Serial.print("|");
  Serial.print(sensor1v);Serial.print("|");
  Serial.print(sensor2v);Serial.print("|");
  Serial.print(sensor3v);
  Serial.println("");      // prints another carriage return
 
                  }
  //                             then adds the carriage return with "println"
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

 
}



