#ifndef DATA_H
#define DATA_H

#include <vector>

#define BUFFSIZE 20

void imuHandler(std::vector<uint8_t>&, std::vector<float>*, std::vector<float>*, uint8_t&, uint8_t&);
void gnssHandler(std::vector<uint8_t>&);
void estimationHandler(std::vector<uint8_t>&);

#endif