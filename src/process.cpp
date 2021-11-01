#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include <iostream>

#include "process.h"

using namespace std;

int Process::Pid() { return _pid; }

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
  usage = (totalTime / Hertz) / seconds;
  return usage; 
}

string Process::Command() { return _command; }

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return _user; }

long int Process::UpTime() {   
  vector<string> list = LinuxParser::CpuUtilization(_pid);
  long uptime = LinuxParser::UpTime();
  float Hertz = sysconf(_SC_CLK_TCK);
  float starttime = stof(list[21]);

  float time = starttime / Hertz;
  time = uptime - time;
  return (int)time;
}

bool Process::operator<(Process const& a) const { 
    return a.usage < usage;
}