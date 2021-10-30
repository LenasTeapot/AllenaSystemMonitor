#include <vector>
#include <iostream>
#include "processor.h"
#include "linux_parser.h"

using std::vector;
using std::string;
using std::stof;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> data = LinuxParser::CpuUtilization();
    float user = stof(data[0]);
    float nice = stof(data[1]);
    float system = stof(data[2]);
    float idle = stof(data[3]);
    float iowait = stof(data[4]);
    float irq = stof(data[5]);
    float softirq = stof(data[6]);
    float steal = stof(data[7]);
    float guest = stof(data[8]);
    float guestNice = stof(data[9]);

    _cumulativeCPU = user + nice + system + idle + iowait + irq + softirq + steal + guest + guestNice;

    double Idle = idle + iowait;
    double NonIdle = user + nice + system + irq + softirq + steal;
    double PrevTotal = _prevIdle + _prevNonIdle;
    double Total = Idle + NonIdle;
    double totald = Total - PrevTotal;
    double idled = Idle - _prevIdle;
   
    float CPU_Percentage = (totald - idled) / totald;

    _prevIdle = Idle;
    _prevNonIdle = NonIdle;

    return CPU_Percentage; 
}