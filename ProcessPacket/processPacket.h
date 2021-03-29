#pragma once

#include "vector3.h"
#include <cstdint>

//7565 80 2a 0e 04 3e495665bb2412c0bf7aa01d 0e 05 bbc7351dbb22ce023b0ef61a 0e 12405c1ab020c49ba600000006 1d37
//Sync, Desc, Total Len, Field Len, Field Desc, Field Data, ..., Checksum

typedef struct packetData_t {
    Vector3f accel;
    Vector3f gyro;
};

class processPacket {
    public:
    processPacket() = delete;
    ~processPacket() = delete;
    static packetData_t processLORDPacket(const uint8_t*);
    private:
    static packetData_t insData(const uint8_t*);
    static uint8_t * get8ByteField(uint8_t*,uint8_t);
    static uint8_t * get4ByteField(uint8_t*,uint8_t);
    static uint8_t * get2ByteField(uint8_t*,uint8_t);

};
