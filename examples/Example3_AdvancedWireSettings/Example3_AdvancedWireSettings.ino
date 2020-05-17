/*
  Use the Qwiic Mux to access multiple I2C devices on seperate busses.
  By: Nathan Seidle @ SparkFun Electronics
  Date: May 17th, 2020
  License: This code is public domain but you buy me a beer if you use this
  and we meet someday (Beerware license).

  This library supports different Wire ports and different I2C addresses
  of the mux itself. By sending the mux address to the library you can
  ostensibly have up to 8 mux boards with 64 devices behind it!

  This example shows how to start the lib with a different Wire port and 
  a different I2C address on the mux. It will fail on the Uno because the Uno
  has only one Wire port.

  Hardware Connections:
  Close the ADR0 jumper on the Mux board to change the address to 0x71.
  Attach the Qwiic Mux to your RedBoard or Uno.
  Serial.print it out at 115200 baud to serial monitor.

  SparkFun labored with love to create this code. Feel like supporting open
  source? Buy a board from SparkFun!
  https://www.sparkfun.com/products/14685
*/

#include <Wire.h>

#include <SparkFun_I2C_Mux_Arduino_Library.h> //Click here to get the library: http://librarymanager/All#SparkFun_I2C_Mux
QWIICMUX myMux;

void setup()
{
  Serial.begin(115200);
  Serial.println("Qwiic Mux Shield Read Example");

  Wire1.begin(); //This line will fail to compile on an Uno. Use a dev platform with multiple Wire ports

  //Setup mux to use Wire1. If you have multiple muxes, pass address as first argument
  //ADR0 must have a the jumper closed with solder to use address 0x71. Use 0x70 for all jumpers open.
  if (myMux.begin(0x71, Wire1) == false)
  {
    Serial.println("Mux not detected. Freezing...");
    while (1)
      ;
  }
  Serial.println("Mux detected");

  myMux.setPort(1); //Connect master to port labeled '1' on the mux

  byte currentPortNumber = myMux.getPort();
  Serial.print("CurrentPort: ");
  Serial.println(currentPortNumber);

  Serial.println("Begin scanning for I2C devices");
}

void loop()
{
  Serial.println();

  byte nDevices = 0;
  for (byte address = 1; address < 127; address++)
  {
    Wire1.beginTransmission(address);
    byte error = Wire1.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 0x10)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println();

      nDevices++;
    }
  }

  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("Done");

  delay(1000);
}