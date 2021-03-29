#include <iostream> // Only needed for testing purposes

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
    xyzData.push_back( (* (float *) &midx) ); // Reinterpret_cast does not work for this so
    xyzData.push_back( (* (float *) &midy) );
    xyzData.push_back( (* (float *) &midz) );
    //xyzData.push_back(* reinterpret_cast<float*>(&midx)); // Works, but its doing the same thing as above, if you do it without address' it doesn't work
    return xyzData;
}

// Take in a packet, extract the data fields with their lengths and field descriptor bytes
// then push the data vector into the appropriate buffer
void imuHandler(vector<uint8_t> &pkt, vector<float> *accelBuffer, vector<float> *gyroBuffer, uint8_t &firsta, uint8_t &firstg) {
    uint8_t payloadLength = pkt[3]; // Keep in mind I can use this in a for loop and subtract off the field length in a for loop to capture all fields in packet
    //cout << unsigned(payloadLength) << '\n';

    packet_t payload;
    for (uint8_t i = 4; i < payloadLength; i++) {
        payload.push_back(pkt[i]);
    }   // extract the payload from the packet (Could probably delete this if we tinkered with section below)

    for (uint8_t i = 0; i < payloadLength; i += payload[i]) {
        uint8_t fieldLength = payload[i]; //Each field in the payload has its own length property as the first byte of the field
        uint8_t fieldDescriptor = payload[i+1]; // Second byte is the field descriptor. Whether it is accel or gyro, etc.
        packet_t fieldData;
        for (uint8_t j = 2; j < fieldLength; j++) {
            fieldData.push_back(payload[i+j]);
        }   // Extract the data in the field, skip the first two bytes and the rest of the data is what we want
        vector<float> xyz; // Maybe call accelGyroData() here instead to reduce reuse of code but could be a waste if we include other data in the field like GPS data
        switch (fieldDescriptor) {  // Switch to relevant course for accel or gyro or etc. data
            case 4:
                xyz = accelGyroData(fieldData);
                cout << "Accel:\nX: " << xyz[0] << "\nY: " << xyz[1] << "\nZ: " << xyz[2] << '\n';
                firsta = (firsta + 1) % BUFFSIZE; // Before we push on data increment the index so that firsta will equal the most recent data
                accelBuffer[firsta] = xyz; // Push vector with accel xyz onto the buffer
                break;
            case 5:
                xyz = accelGyroData(fieldData);
                cout << "Gyro:\nX: " << xyz[0] << "\nY: " << xyz[1] << "\nZ: " << xyz[2] << '\n';
                firstg = (firstg + 1) % BUFFSIZE; // Before we push on data increment the index so that firstg will equal the most recent data
                gyroBuffer[firstg] = xyz; // Push vector with gyro xyz onto the buffer
                break;
            default:
                cout << "Don't want anything but Gyro and Accel\n";
                break;
        }
    }
}

void gnssHandler([[maybe_unused]] vector<uint8_t> &pkt) { // TODO: Decide if we need these
}

void estimationHandler([[maybe_unused]] vector<uint8_t> &pkt) { // TODO: Decide if we need these
} 
