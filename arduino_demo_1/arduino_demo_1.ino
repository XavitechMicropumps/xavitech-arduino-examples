// SPDX-License-Identifier: MIT
// Copyright © 2020 Xavitech Micropums AB

// This example demonstrates reading and writing to RAM using commands for reading/writing
// pump speed and start/stop.

#include "xavitech.h"

SerialReadCommand readPumpSpeed;
SerialWriteCommand writePumpSpeed1;
SerialWriteCommand writePumpSpeed2;
SerialWriteCommand stopPump;
SerialWriteCommand startPump;

void setup() 
{
    // Setup commands;
    readPumpSpeed.address = 382;
    readPumpSpeed.numberOfBytes = 2;

    writePumpSpeed1.address = 382;
    writePumpSpeed1.numberOfBytes = 2;
    writePumpSpeed1.data = 5000;

    writePumpSpeed2.address = 382;
    writePumpSpeed2.numberOfBytes = 2;
    writePumpSpeed2.data = 500;

    stopPump.address = 122;
    stopPump.numberOfBytes = 2;
    stopPump.data = 0;

    startPump.address = 122;
    startPump.numberOfBytes = 2;
    startPump.data = 220;
     
    Serial.begin(115200);
    Serial1.begin(9600);
}

void loop() 
{
    if(!writeToPump(Serial1, writePumpSpeed1)) Serial.println("Error Writing...");
    
    int32_t pumpSpeed = readFromPump(Serial1, readPumpSpeed);
    if(pumpSpeed < 0) Serial.println("Error Reading...");
    Serial.print("Pump Speed: ");
    Serial.println(pumpSpeed);
    
    delay(2500);

    if(!writeToPump(Serial1, writePumpSpeed2)) Serial.println("Error Writing...");

    pumpSpeed = readFromPump(Serial1, readPumpSpeed);
    if(pumpSpeed < 0) Serial.println("Error Reading...");
    Serial.print("Pump Speed: ");
    Serial.println(pumpSpeed);

    delay(2500);

    if(!writeToPump(Serial1, stopPump)) Serial.println("Error Writing...");

    pumpSpeed = readFromPump(Serial1, readPumpSpeed);
    if(pumpSpeed < 0) Serial.println("Error Reading...");
    Serial.print("Pump Speed: ");
    Serial.println(pumpSpeed);

    delay(2500);

    if(!writeToPump(Serial1, startPump)) Serial.println("Error Writing...");

    pumpSpeed = readFromPump(Serial1, readPumpSpeed);
    if(pumpSpeed < 0) Serial.println("Error Reading...");
    Serial.print("Pump Speed: ");
    Serial.println(pumpSpeed);

    delay(2500);

}
