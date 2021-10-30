#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include <iostream>

#include "process.h"


using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  vector<string> list = LinuxParser::CpuUtilization(_pid);
  float totalTime, seconds, Hertz;
  Hertz = sysconf(_SC_CLK_TCK);
  long uptime = LinuxParser::UpTime();
  float utime, stime, cutime, cstime, starttime;

  utime = stof(list[13]);
  stime = stof(list[14]);
  cutime = stof(list[15]);
  cstime = stof(list[16]);

  starttime = stof(list[21]);
  totalTime = utime + stime + cutime + cstime;
  seconds = uptime - (starttime / Hertz);
  usage = 100 * ((totalTime / Hertz) / seconds);
  return usage; 
}

string Process::Command() { return _command; }

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return _user; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {   
  vector<string> list = LinuxParser::CpuUtilization(_pid);
  float Hertz = sysconf(_SC_CLK_TCK);
  float starttime = stof(list[21]);

  float time = starttime / Hertz;
  return (int)time;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
    return a.usage < usage;
}