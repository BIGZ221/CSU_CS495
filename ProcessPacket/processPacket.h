#pragma once

#include <cstdint>

//7565 80 2a 0e 04 3e495665bb2412c0bf7aa01d 0e 05 bbc7351dbb22ce023b0ef61a 0e 12405c1ab020c49ba600000006 1d37
//Sync, Desc, Total Len, Field Len, Field Desc, Field Data, ..., Checksum

typedef struct {
    float x, y, z;
} Vector3f;

typedef struct {
    Vector3f accel;
    Vector3f gyro;
} LORDpacketData_t;

class processPacket {
    public:
    static LORDpacketData_t processLORDPacket(const uint8_t*);

    private:
    static LORDpacketData_t insData(const uint8_t*);
    static Vector3f populateVector3f(const uint8_t*,uint8_t);
    static uint64_t get8ByteField(const uint8_t*,uint8_t);
    static uint32_t get4ByteField(const uint8_t*,uint8_t);
    static uint16_t get2ByteField(const uint8_t*,uint8_t);

};
