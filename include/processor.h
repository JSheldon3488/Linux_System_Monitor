#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <unordered_map>
#include "linux_parser.h"

class Processor {
 public:
  Processor();
  float Utilization();  // TODO: See src/processor.cpp

 private:
  std::unordered_map<std::string,long> cpu_data;
};

#endif