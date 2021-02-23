#include "data.h"

using namespace std;
using packet_t = vector<uint8_t>;

vector<float> accelGyroData(const packet_t &fieldData) {
    vector<float> xyzData;
    uint32_t midx = 0;
    for (uint8_t i = 0; i < 4; i++ ) {
        midx = (midx << 8) + static_cast<uint32_t>(fieldData[i]);
    }
    uint32_t midy = 0;
    for (uint8_t i = 4; i < 8; i++ ) {
        midy = (midy << 8) + static_cast<uint32_t>(fieldData[i]);
    }
    uint32_t midz = 0;
    for (uint8_t i = 8; i < 12; i++ ) {
        midz = (midz << 8) + static_cast<uint32_t>(fieldData[i]);
    }
    xyzData.push_back( (* (float *) &midx) );
    xyzData.push_back( (* (float *) &midy) );
    xyzData.push_back( (* (float *) &midz) );
    return xyzData;
}

// Take in a packet, extract the data fields with their lengths and field descriptor bytes
// then pass the data on to their final destination
void imuHandler(vector<uint8_t> &pkt, vector<float> *accelBuffer, vector<float> *gyroBuffer, uint8_t &firsta, uint8_t &firstg) {
    uint8_t payloadLength = pkt[3]; // Keep in mind I can use this in a for loop and subtract off the field length in a for loop to capture all fields in packet
    //cout << unsigned(payloadLength) << '\n';

    packet_t payload;
    for (uint8_t i = 4; i < payloadLength; i++) {
        payload.push_back(pkt[i]);
    }

    for (uint8_t i = 0; i < payloadLength; i += payload[i]) {
        uint8_t fieldLength = payload[i];
        uint8_t fieldDescriptor = payload[i+1];
        packet_t fieldData;
        for (uint8_t j = 2; j < fieldLength; j++) {
            fieldData.push_back(payload[i+j]);
        }
        vector<float> xyz;
        switch (fieldDescriptor) {
            case 4: 
                xyz = accelGyroData(fieldData);
                cout << "Accel:\nX: " << xyz[0] << "\nY: " << xyz[1] << "\nZ: " << xyz[2] << '\n';
                accelBuffer[firsta] = xyz;
                firsta = (firsta + 1) % 20;
                break;
            case 5:
                xyz = accelGyroData(fieldData);
                cout << "Gyro:\nX: " << xyz[0] << "\nY: " << xyz[1] << "\nZ: " << xyz[2] << '\n';
                gyroBuffer[firstg] = xyz;
                firstg = (firstg + 1) % 20;
                break;
            default:
                cout << "Don't want anything but Gyro and Accel\n";
                break;
        }
    }
}

void gnssHandler([[maybe_unused]] vector<uint8_t> &pkt) {
    //cout << pkt << '\n';
}

void estimationHandler([[maybe_unused]] vector<uint8_t> &pkt) {
    //cout << pkt << '\n';
} 
