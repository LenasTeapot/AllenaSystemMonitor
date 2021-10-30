#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
#include "format.h"

using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id) : _pid(id) {}
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  float usage;

 private:
    int _pid;
    string _user = LinuxParser::User(_pid);
    string _command = LinuxParser::Command(_pid);
};

#endif