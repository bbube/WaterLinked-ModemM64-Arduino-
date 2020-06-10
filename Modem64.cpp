#include "Modem64.h"

Modem64::Modem64()
{
    MODEM64SERIAL.begin(BAUDRATE);
}
bool Modem64::connect(char role, byte channel)
{
    MODEM64SERIAL.print("wcs,");
    MODEM64SERIAL.print(role);
    MODEM64SERIAL.print(SEPERATOR);
    MODEM64SERIAL.print(channel);
    MODEM64SERIAL.print(EOP);
    // unsigned long looptimer = millis();
    while (!MODEM64SERIAL.available())
    {
        // unsigned long currentMillis = millis();
        // if ((currentMillis - looptimer) >= 20000)
        // {
        //     return false;
        // }
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_SET_SETTINGS && _buffer[4] == ACK)
    {
        return true;
    }
    return false;
}

int Modem64::getPayload()
{
    MODEM64SERIAL.print("wcn");
    MODEM64SERIAL.print(EOP);
    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_GET_PAYLOAD_SIZE)
    {
        return _buffer[4] - '0';
    }
    return 0;
}

int Modem64::getVersion()
{
    MODEM64SERIAL.print("wcv");
    MODEM64SERIAL.print(EOP);
    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_GET_VERSION)
    {
        int tmp = concatenate(_buffer[4] - '0', _buffer[6] - '0');
        return concatenate(tmp, _buffer[8] - '0');
    }
    return 0;
}

bool Modem64::flushTransmitQueue()
{
    MODEM64SERIAL.print("wcf");
    MODEM64SERIAL.print(EOP);
    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_FLUSH && _buffer[4] == ACK)
    {
        return true;
    }
    return false;
}
int Modem64::getTransmitQueueLenght()
{
    MODEM64SERIAL.print("wcl");
    MODEM64SERIAL.print(EOP);
    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_GET_BUFFER_LENGTH)
    {
        return _buffer[4] - '0';
    }
    return 0;
}
bool Modem64::linked()
{
    MODEM64SERIAL.print("wcd");
    MODEM64SERIAL.print(EOP);
    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[4] == CMD_GET_DIAGNOSTIC && _buffer[6] == LINKED)
    {
        return true;
    }
    return false;
}
bool Modem64::updateAvailiable()
{
    if (MODEM64SERIAL.available())
    {
        MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
        if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == RESP_GOT_PACKET)
        {
            _depth = concatenate(_buffer[6] - '0', _buffer[7] - '0');
            _temp = concatenate(_buffer[8] - '0', _buffer[9] - '0');
            _bpm = concatenate(_buffer[10] - '0', _buffer[11] - '0');
            return true;
        }
        else
        {
            //Ich hab absolut keine Ahnung ob sowas passieren kann. Aber wenn, dann haben wir ein Paket verloren :D
        }
    }
    return false;
}

bool Modem64::sendUpdate(int depth, int temp, int bpm)
{

    MODEM64SERIAL.print("wcq,");
    MODEM64SERIAL.print("8");
    MODEM64SERIAL.print(SEPERATOR);
    if ((floor(log10(abs(depth))) + 1) == 2)
    {
        MODEM64SERIAL.print(depth);
    }
    else
    {
        MODEM64SERIAL.print("00");
    }
    if ((floor(log10(abs(temp))) + 1) == 2)
    {
        MODEM64SERIAL.print(temp);
    }
    else
    {
        MODEM64SERIAL.print("00");
    }
    if ((floor(log10(abs(bpm))) + 1) == 2)
    {
        MODEM64SERIAL.print(bpm);
    }
    else
    {
        MODEM64SERIAL.print("00");
    }
    MODEM64SERIAL.print("00");
    MODEM64SERIAL.print(EOP);

    while (!MODEM64SERIAL.available())
    {
    }
    MODEM64SERIAL.readBytesUntil(EOP, _buffer, 30);
    // last_Message_Modem = _buffer;
    if (_buffer[0] == SOP && _buffer[1] == DIR_RESP && _buffer[2] == CMD_QUEUE_PACKET && _buffer[4] == ACK)
    {
        return true;
    }
    return false;
}

unsigned Modem64::concatenate(unsigned x, unsigned y)
{
    unsigned pow = 10;
    while (y >= pow)
        pow *= 10;
    return x * pow + y;
}

void Modem64::split(unsigned x)
{
    //Lets assume we got 102560
    //then we want to seperate this into 60 for bpm
    _bpm = concatenate(x / 10 % 10, x % 10);
    //25 for temp
    _temp = concatenate(x / 1000 % 10, x / 100 % 10);
    //and 10 for depth
    _depth = concatenate(x / 100000 % 10, x / 10000 % 10);
    //for this reason we extract each value and combine (concatenate) them together.
}

int Modem64::getBPM()
{
    return _bpm;
}

int Modem64::getDepth()
{
    return _depth;
}

int Modem64::getTemp()
{
    return _temp;
}
