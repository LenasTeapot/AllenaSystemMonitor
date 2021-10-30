#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization(); 
  
 private:
    double _prevIdle = 0.0;
    double _prevNonIdle = 0.0;
    float _cumulativeCPU = 0.0;
};

#endif