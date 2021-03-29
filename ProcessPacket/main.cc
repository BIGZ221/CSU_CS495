
#include <iostream>
#include <cstdint>
#include "processPacket.h"

using namespace std;

/*
75, 65, 80, 2a, 0e, 04, 3e, 49, 56, 65, bb, 24, 12, c0, bf, 7a, a0, 1d, 0e, 05, bbc7351dbb22ce023b0ef61a, 0e, 12, 405c1ab020c49ba600000006, 1d37
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

int main() {
    uint8_t testingPacket[] = { 0x75, 0x65, 0x80, 0x2a, 0x0e, 0x04, 0x3e, 0x49, 0x56, 0x65, 0xbb, 0x24, 0x12, 0xc0, 0xbf, 0x7a, 0xa0, 0x1d, 0x0e, 0x05, 0xbb, 0xc7, 0x35, 0x1d, 0xbb, 0x22, 0xce, 0x02, 0x3b, 0x0e, 0xf6, 0x1a, 0x0e, 0x12, 0x40, 0x5c, 0x1a, 0xb0, 0x20, 0xc4, 0x9b, 0xa6, 0x00, 0x00, 0x00, 0x06, 0x1d, 0x37 };
    for (int i = 0; i < 48; i++) {
        cout << hex << (unsigned)testingPacket[i] << ' ';
    }
    cout << endl;
    LORDpacketData_t results = processPacket::processLORDPacket(testingPacket);
    cout << "Accel:\n";
    cout << results.accel.x << '\n';
    cout << results.accel.y << '\n';
    cout << results.accel.z << '\n';
    cout << "Gyro:\n";
    cout << results.gyro.x << '\n';
    cout << results.gyro.y << '\n';
    cout << results.gyro.z << '\n';
}