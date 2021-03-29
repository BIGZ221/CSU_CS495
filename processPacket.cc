
#include "processPacket.h"

//75, 65, 80, 2a, 0e, 04, 3e, 49, 56, 65, bb, 24, 12, c0, bf, 7a, a0, 1d, 0e, 05, bbc7351dbb22ce023b0ef61a 0e 12405c1ab020c49ba600000006 1d37
//Sync, Desc, Payload Len, Field Len, Field Desc, Field Data, ..., Checksum

packetData_t processPacket::insData(const uint8_t* pkt) {
    packetData_t data;
    uint8_t payloadLen = pkt[3];
    for (uint8_t i = 4; i < payloadLen; i += pkt[i]) {
        
    }
}

packetData_t processPacket::processLORDPacket(const uint8_t* pkt) {
    uint8_t pktDesc = pkt[2];
    switch (pktDesc) {
        case 0x80:
            return insData(pkt);
    }

}

uint8_t * get8ByteField(uint8_t*,uint8_t) {

}

uint8_t * get4ByteField(uint8_t*,uint8_t) {

}

uint8_t * get2ByteField(uint8_t*,uint8_t) {

}

