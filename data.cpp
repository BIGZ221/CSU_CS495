#include "data.h"

// Take in a packet, extract the data fields with their lengths and field descriptor bytes
// then pass the data on to their final destination
void imuHandler(string pkt) {
    uint16_t payloadLength = pkt[3]; // Keep in mind I can use this in a for loop and subtract off the field length in a for loop to capture all fields in packet
    cout << unsigned(payloadLength) << '\n';
    string payload = pkt.substr(4,payloadLength);
    
    uint8_t fieldLen = payload[0];
    uint8_t fieldDescriptor = payload[1];
    string fieldData = payload.substr(2,fieldLen-2);
    cout << unsigned(fieldLen) << '\n';
    uint32_t midx = 0;
    for (uint8_t i = 0; i < 4; i++ ) {
        midx = (midx << 8) + static_cast<uint32_t>(fieldData[i]);
        cout << unsigned(fieldData[i]) << ' ';
    }
    cout << '\n' << unsigned(midx) << '\n';
    float finalx = * (float *) &midx;
    cout << finalx << '\n';
}

void gnssHandler(string pkt) {
    cout << pkt << '\n';
}

void estimationHandler(string pkt) {
    cout << pkt << '\n';
} 