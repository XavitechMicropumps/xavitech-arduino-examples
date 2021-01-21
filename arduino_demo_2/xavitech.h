// SPDX-License-Identifier: MIT
// Copyright © 2020 Xavitech Micropums AB

#define SUCCESS   165
#define FAIL      90

// Serial read command struct, does not support serial number or netid
struct SerialReadCommand
{
    uint16_t address = 0;
    bool ramEeprom = 0;             // 0 = RAM (default), 1 = EEPROM
    uint8_t numberOfBytes = 1;
};

// Serial write command struct, does not support serial number or netid
struct SerialWriteCommand    // For 1-4 bytes of data
{
    uint16_t address = 0;
    bool ramEeprom = 0;             // 0 = RAM (default), 1 = EEPROM
    uint8_t numberOfBytes = 1;
    uint32_t data;
};

int64_t readFromPump(Stream &port, SerialReadCommand command);
bool writeToPump(Stream &port, SerialWriteCommand command);

// Read from RAM or EEPROM (Serial number and netid are default - 0)
int64_t readFromPump(Stream &port, SerialReadCommand command)
{
    uint8_t sendBufferSize = 8 + command.numberOfBytes;
    uint8_t receiveBufferSize = 1 + command.numberOfBytes;
    uint8_t sendBuffer[sendBufferSize] = { };
    uint8_t receiveBuffer[receiveBufferSize] = { };

    uint8_t checksum = 0; 

    sendBuffer[4] = 0b00111111 & (command.address >> 8);                    // MSB
    if(command.ramEeprom) sendBuffer[4] = 0b01000000 + sendBuffer[4];       // If we should read from EEPROM
    sendBuffer[5] = command.address & 0x00FF;                               // LSB
    sendBuffer[6] = 0b00111111 & (command.numberOfBytes - 1);

    for (uint8_t i = 0; i < sendBufferSize; i++) checksum += sendBuffer[i];     // Calculate checksum
    sendBuffer[sendBufferSize-1] = checksum;

    port.flush();
    port.write(sendBuffer, sendBufferSize);

    delay(20);
    uint8_t receiveTimer = 0;
    uint8_t it = 0;
    while (receiveTimer < 10)
    {
        while (port.available())
        {
            receiveBuffer[it] = port.read();
            if (it == command.numberOfBytes)
            { 
                checksum = 0;
                for (uint8_t i = 0; i < receiveBufferSize-1; i++) checksum += receiveBuffer[i];     // Calculate checksum

                if (checksum == receiveBuffer[receiveBufferSize-1])
                {
                    uint32_t returnValue = 0;;
                    for(uint8_t i = 0; i < command.numberOfBytes; i++)
                    {
                        uint32_t temp = receiveBuffer[i];
                        returnValue += temp << (8*i);
                    }

                    return returnValue;
                }
                else return -1;
            }
            it++;
        }
        receiveTimer++;
        delay(5);
    }
    return -1;
}


// Write to RAM or EEPROM (Serial number and netid are default - 0)
bool writeToPump(Stream &port, SerialWriteCommand command)
{
    uint8_t sendBufferSize = 8 + command.numberOfBytes;
    uint8_t sendBuffer[sendBufferSize] = { };
    uint8_t pumpAnswer = 0;
    uint8_t checksum = 0; 

    sendBuffer[4] = 0b00111111 & (command.address >> 8);                    // MSB
    if(command.ramEeprom) sendBuffer[4] = 0b01000000 + sendBuffer[4];       // If we should write to EEPROM
    sendBuffer[5] = command.address & 0x00FF;                               // LSB
    sendBuffer[6] = (0b00111111 & (command.numberOfBytes - 1)) + 0b10000000;

    // Set data bytes in sendBuffer
    uint8_t *dataPointer; // = &command.data;
    for(int i = 0; i<command.numberOfBytes; i++)
    {
        dataPointer = reinterpret_cast<uint8_t*>(&command.data) + i;
        sendBuffer[7+i] = *dataPointer;
    }

    for (uint8_t i = 0; i < sendBufferSize; i++) checksum += sendBuffer[i];     // Calculate checksum
    sendBuffer[sendBufferSize-1] = checksum;

    port.flush();
    port.write(sendBuffer, sendBufferSize);    

    delay(20);
    uint8_t receiveTimer = 0;
    while (receiveTimer < 10)
    {
        if (port.available())
        {
            pumpAnswer = port.read();
            if (pumpAnswer == SUCCESS) return true;
            else return false;
        }
        receiveTimer++;
        delay(5);
    }
    return false;
}
