/*
    Modem64.h - Library for handling the Modem64 selled by Waterlinked.
    Created by Benjamin Bube, 28.02.2020.
    Released into the public domain.
*/
#ifndef MODEM64_H_
#define MODEM64_H_
#include "Arduino.h"

#define MODEM64SERIAL Serial3
#define BAUDRATE 115200

#define SOP 'w'      //Start byte: Packets sent to and received from the modem start with w
#define EOP '\n'     //End byte: also valid: \r\n Packets sent to and received from the modem end with LF or CR+LF
#define DIR_CMD 'c'  //Direction: commands (to the modem)
#define DIR_RESP 'r' //Direction: response (from the modem)
#define CHECKSUM '*' //Checksum: *xx (Optional to the modem. Modem will return always a checksum) checksum algorithm: CRC-8

#define CMD_GET_VERSION 'v'       //protocol version
#define CMD_GET_PAYLOAD_SIZE 'n'  //Get supported payload size
#define CMD_GET_BUFFER_LENGTH 'l' //Get transmit queue length
#define CMD_GET_DIAGNOSTIC 'd'    //Get diagnostic
#define CMD_GET_SETTINGS 'c'      //Get modem configuration
#define CMD_SET_SETTINGS 's'      //Set modem configuration - Where role=a or b and channel 1-7: eg: wrs,b,3
#define CMD_QUEUE_PACKET 'q'      //Queue packet for transmission. Number of characters in payload need to be exactly same as size.
#define CMD_FLUSH 'f'             //Flush transmit queue
#define RESP_GOT_PACKET 'p'       //Got packet from other modem eg: wrp,8,Welcome!
#define SEPERATOR ','             //To seperate the commands.
#define ACK 'a'                   //Acknowledgement
#define LINKED 'y'                //Negative acknowledgement

class Modem64
{
public:
    /** Constructor
     * 
     */
    Modem64();
    /** """ Connect to modem and get version and supported payload size. Remember: There has to be 1 Role a and 1 Role b """
    *   Returns True if successfull or false if not.
    */
    bool connect(char role, byte channel);
    int getPayload();
    int getVersion();
    bool flushTransmitQueue();
    int getTransmitQueueLenght();
    bool linked();
    bool updateAvailiable();
    bool sendUpdate(int depth, int temp, int bpm);

    int getBPM();
    int getDepth();
    int getTemp();
    String last_Message_Modem;

private:
    char _buffer[20];
    unsigned concatenate(unsigned x, unsigned y);
    void split(unsigned x);
    int _bpm = 0;
    int _temp = 0;
    int _depth = 0;
};

#endif /* MODEM64_H_ */
