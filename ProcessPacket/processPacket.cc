
#include "processPacket.h"
#include <iostream>

using namespace std;

/*
75, 65, 80, 2a, 0e, 04, 3e, 49, 56, 65, bb, 24, 12, c0, bf, 7a, a0, 1d, 0e, 05, bbc7351dbb22ce023b0ef61a 0e 12405c1ab020c49ba600000006 1d37
Sync, Desc, Payload Len, Field Len, Field Desc, Field Data, ..., Checksum
Should correspond to: 
Accel:
X: 0.196619
Y: -0.00250356
Z: -0.979006
Gyro:
X: -0.00607933
Y: -0.0024842
Z: 0.00218142
*/

Vector3f processPacket::populateVector3f(const uint8_t* pkt, uint8_t offset) {
    Vector3f data;
    uint32_t tmp[3];
    for (uint8_t j = 0; j < 3; j++) {
        tmp[j] = get4ByteField(pkt, offset + j * 4 + 2);
    }
    data.x = *reinterpret_cast<float*>( &tmp[0] );
    data.y = *reinterpret_cast<float*>( &tmp[1] );
    data.z = *reinterpret_cast<float*>( &tmp[2] );
    return data;
}

LORDpacketData_t processPacket::insData(const uint8_t* pkt) {
    LORDpacketData_t data;
    uint8_t payloadLen = pkt[3];
    for (uint8_t i = 4; i < payloadLen; i += pkt[i]) {
        uint8_t fieldDesc = pkt[i+1];
        switch (fieldDesc) {
            case 04:
                data.accel = populateVector3f(pkt, i);
                break;
            case 05:
                data.gyro = populateVector3f(pkt, i);
                break;
        }
    }
    return data;
}

LORDpacketData_t processPacket::processLORDPacket(const uint8_t* pkt) {
    uint8_t pktDesc = pkt[2];
    switch (pktDesc) {
        case 0x80:
            return insData(pkt);
        default:
            LORDpacketData_t nullPacket = { -999, -999, -999, -999, -999, -999 };
            cerr << "FATAL ERROR UNKNOWN PACKET DESCRIPTOR" << endl;
            return nullPacket;
    }
}

uint64_t processPacket::get8ByteField(const uint8_t* pkt, uint8_t offset) {
    uint64_t res = 0;
    for (int i = 0; i < 2; i++)
        res = res << 32 | get4ByteField(pkt,offset + 4 * i);
    return res;
}

uint32_t processPacket::get4ByteField(const uint8_t* pkt, uint8_t offset) {
    uint32_t res = 0;
    for (int i = 0; i < 2; i++)
        res = res << 16 | get2ByteField(pkt, offset + 2 * i);
    return res;
}

uint16_t processPacket::get2ByteField(const uint8_t* pkt, uint8_t offset) {
    uint16_t res = 0;
    for (int i = 0; i < 2; i++)
        res = res << 8 | pkt[offset + i];
    return res;
}

