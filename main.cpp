#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <queue>

#include "data.h"
#include "commands.h"

using namespace std;


// For each char in pkt print it to cout with a width of 2 and padding left with zeros, uint8_t doesn't play nice so you have
// call unsigned() to force the char to print correctly because cout tries to do some conversion and unsigned() stops it
void printPacket(const string &pkt) {
    for (uint8_t i : pkt) {
        cout << setfill('0') << setw(2) << hex << unsigned(i);
    }
    cout << '\n';
}

// Grab the last two bytes of the packet and return them as a uint16_t
// Must cast from char to uint8_t then recast later to a uint16_t
uint16_t getCheckSumFromPacket(string s) {
    s = s.substr(s.length()-2);
    uint8_t first = s[0];
    uint8_t second = s[1];
   return (static_cast<uint16_t>(first) << 8) + static_cast<uint16_t>(second);
}

// Sum every byte in the packet except the last two then and sum the first sum each iteration
// Overflow is ignored then you bitshift sum1 left 8 times and add in sum2's bits for final checksum
// Values must be the type they are in this calculation
uint16_t calcFletcherCheckSum(string s) {
    s = s.substr(0, s.length()-2);
    uint8_t sum1 = 0;
    uint8_t sum2 = 0;
    for (uint8_t i : s) {
        sum1 = (sum1 + i);
        sum2 = (sum2 + sum1);
    }
    return (static_cast<uint16_t>(sum1) << 8) + static_cast<uint16_t>(sum2);
}

// Read from the file stream till you can't
// Each packet starts with "UE" so that is how you check for start of a packet then when the packet
// is read completely check the checksum in the packet with the one we calculate. If the checksums
// match push the packet onto the packet buffer and go for the next packet
//
// TODO: Decide what to do with a packet if checksum is bad
//      ignore?
//
void packetReader(fstream &in, queue<string> &pq) {
    string pkt = "";
    char temp;
    while (in.get(temp)) {
        uint8_t pktByte = temp;
        if (temp == 'u' && in.peek() == 'e' && pkt.length() > 0) { // If pkt isn't empty and you encounter "UE" then you are done reading in the previous packet
            in.putback(temp); // You read in a byte and you aren't ready so put it back ... if this fails explode? 
            //cout << "Packet Read!" << '\n'; // TODO: Remove this
            uint16_t checkSum = calcFletcherCheckSum(pkt);
            //cout << "Checksum: " << hex << unsigned(checkSum) << '\n'; // TODO: Remove this
            uint16_t pktCheckSum = getCheckSumFromPacket(pkt);
            if (checkSum != pktCheckSum) {
                cout << pktCheckSum << '\n';
                printPacket(pkt);
                cout << "Packet has wrong checksum, discarding packet!" << '\n'; // TODO: Decide what to do here
            } else {
                pq.push(pkt);
            }
            pkt = "";
        } else {
            pkt.push_back(pktByte); // Push the packet onto the packet buffer
        }
    }
    //cout << '\n';
}

// TODO: Switch statement depending on the Descriptor byte, should be in pkt[2]
//      0x01 is Base Commands
//      0x0C is 3DM Commands
//      0x0D is Estimation Filter Commands
//      0x7F is System Commands
//      0x80 is IMU Data
//      0x81 is GNSS Data
//      0x82 is Estimation filter Data
void parser(queue<string> &pq) {
    int runs = 4;
    while (pq.size() > 0 && runs > 0) {
        runs--;
        string pkt = pq.front();
        pq.pop();
        uint8_t descriptor = pkt[2];
        cout << hex << setfill('0') << setw(2) << unsigned(descriptor) << '\n';
        printPacket(pkt);
        switch (descriptor)
        {
        case 0x80:
            imuHandler(pkt);
            return;
            break;
        case 0x81:

            break;
        case 0x82:

            break;
        case 0x01:
            break;
        case 0x0c:
            break;
        case 0x0d:
            break;
        case 0x7f:
            break;
        default:
            cout << "Descriptor not identified: " << unsigned(descriptor) << '\n';
            break;
        }
        //break;
    }
}

int main() { // Could probably do this with another thread or another process but for testing not doing that yet
    fstream in("testdata.bin");
    queue<string> packetBuffer;
    packetReader(in, packetBuffer);
    parser(packetBuffer);
    float withoutHack = 0x3e4a636d;// = 1,045,060,000 ... Lord outputs data in g's I don't think that is right 
    uint32_t holder = 0x3e4a636d;
    float withHack = * ( float * ) &holder; // Evil floating point bit hack, (WARNING PROFANITY) https://youtu.be/p8u_k2LIZyo?t=730 Thank you quake devs
                                    // test = 0.197645g or about 2 m/s^2 (Fast but possible maybe do some testing?)
    cout << withHack << "g" << '\n';
    cout << withoutHack << "g" << '\n';
    return 0;
}
