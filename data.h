#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

void imuHandler(vector<uint8_t>&, vector<float>*, vector<float>*, uint8_t&, uint8_t&);
void gnssHandler(vector<uint8_t>&);
void estimationHandler(vector<uint8_t>&);
