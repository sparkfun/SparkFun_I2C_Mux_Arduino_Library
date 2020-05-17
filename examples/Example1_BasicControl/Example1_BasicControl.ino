/*
  Use the Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Nathan Seidle @ SparkFun Electronics
  Date: May 17th, 2020
  License: This code is public domain but you buy me a beer if you use this 
  and we meet someday (Beerware license).

  Some I2C devices respond to only one I2C address. This can be a problem
  when you want to hook multiple of a device to the I2C bus. An I2C Mux 
  solves this issue by allowing you to change the 'channel' or port that
  a device is connected to on the mux.

  This example shows how to hook up two MMA8452Q accelerometers with the same address.
  You can read the MMA8452Q hookup guide and get the library from https://learn.sparkfun.com/tutorials/mma8452q-accelerometer-breakout-hookup-guide

  The TCA9548A is a mux. This means when you enableMuxPort(2) then the SDA and SCL lines of the master (Arduino)
  are connected to port 2. Whatever I2C traffic you do, such as accel.init() will be communicated to whatever
  sensor you have on port 2. You don't need accel2.init() or accel5.init().

  Outputs two sets of XYZ acceleration

  Hardware Connections:
  Attach the Qwiic Mux Shield to your RedBoard or Uno.
  Plug two Qwiic MMA8452Q breakout boards into ports 0 and 1.
  Serial.print it out at 9600 baud to serial monitor.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14685

*/

#include <Wire.h>

TwoWire Wire1(1); //Will use pads 8/9

#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;

#include <SparkFun_MMA8452Q.h> //Click here to get the library: http://librarymanager/All#SparkFun_MMA8452
MMA8452Q accel;

#define NUMBER_OF_SENSORS 2

void setup()
{
  Serial.begin(115200);
  Serial.println("Qwiic Mux Shield Read Example");

  Wire1.begin();
  if (myMux.begin(0x70, Wire1) == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");

  //myMux.setPort(8);
  byte currentPortNumber = myMux.getPort();
  Serial.print("CurrentPort: ");
  Serial.println(currentPortNumber);

  while (1)
    ;

  //Initialize all the sensors
  for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
  {
    myMux.setPort(x); //Tell mux to connect to port X
    accel.init();     //Init the sensor connected to this port
  }

  Serial.println("Mux Shield online");
}

void loop()
{
  for (byte x = 0; x < NUMBER_OF_SENSORS; x++)
  {
    myMux.setPort(x); //Tell mux to connect to this port, and this port only

    if (accel.available())
    {
      accel.read();

      Serial.print("Accel ");
      Serial.print(x);
      Serial.print(": ");
      Serial.print(accel.cx, 2);
      Serial.print(" ");
      Serial.print(accel.cy, 2);
      Serial.print(" ");
      Serial.print(accel.cz, 2);
      Serial.print(" ");

      Serial.println(); // Print new line every time.
    }
  }

  delay(1); //Wait for next reading
}