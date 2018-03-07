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
float maxRange = 8190 * 0.0393701;

void setup()
{
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
}

void loop()
{
  float tt = sensor.readRangeSingleMillimeters();
  //Convert to inches
  tt = tt * 0.0393701;

  if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
  if (tt < (maxRange) ) 
  {
    //Serial.print(" Measure : "); 
    Serial.println(tt);
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

 
}



