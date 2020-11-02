// SPDX-License-Identifier: MIT
// Copyright © 2020 Xavitech Micropums AB

// This example reads "Serial" and looks for '+' or '-'.
// For every '+' or '-' it will increase or decrease pump speed 10 steps.

#include "xavitech.h"

SerialReadCommand readPumpSpeed;
SerialWriteCommand writePumpSpeed;

uint16_t currentPumpSpeed;

void setup() 
{
    // Setup commands;
    readPumpSpeed.address = 382;
    readPumpSpeed.numberOfBytes = 2;

    writePumpSpeed.address = 382;
    writePumpSpeed.numberOfBytes = 2;
    writePumpSpeed.data = 0;

    currentPumpSpeed = 0;
     
    Serial.begin(115200);
    Serial1.begin(9600);
}

void loop() 
{
    if(Serial.available())
    {
        char c = Serial.read();
        if(c == '-' && currentPumpSpeed < 65535) 
        {
            uint16_t newSpeed = currentPumpSpeed + 10;
            writePumpSpeed.data = newSpeed;
            writeToPump(Serial1, writePumpSpeed);
            currentPumpSpeed = newSpeed;
        }
        else if(c == '+' && currentPumpSpeed > 9) 
        {
            uint16_t newSpeed = currentPumpSpeed - 10;
            writePumpSpeed.data = newSpeed;
            writeToPump(Serial1, writePumpSpeed);
            currentPumpSpeed = newSpeed;
        }
    }

    int32_t pumpSpeed = readFromPump(Serial1, readPumpSpeed);
    delay(50);
    if(pumpSpeed < 0) Serial.println("ERROR READING!!!");
    Serial.print("Pump Speed: ");
    Serial.println(pumpSpeed);
}
