#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Done.
float LinuxParser::MemoryUtilization() {
  float memTotal;
  float memFree;
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal") 
        {
          memTotal = std::stof(value);
        }
        else if (key == "MemFree")
        {
          memFree = std::stof(value);
        }
      }
    }
  }
  return (memTotal - memFree)/memTotal;
}
// Done?
long LinuxParser::UpTime() { 
  string upTime;
  string idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
    return std::stol(upTime);
  }
  return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Done. This is used in Processor class
vector<string> LinuxParser::CpuUtilization() { 
  string key, line, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestNice) {
      if (key == "cpu") {
        return {user, nice, system, idle, iowait, irq, softirq, steal, guest, guestNice};
      }
    }
  }
  return {}; 
}

//Done.
int LinuxParser::TotalProcesses() {
  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") 
        {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

//Done.
int LinuxParser::RunningProcesses() {  string key;
  string value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") 
        {
          return std::stoi(value);
        }
      }
    }
  }
  return 0;
}

// Done.
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    return line;
  }
  return string(); 
}

// Done.
string LinuxParser::Ram(int pid) { 
  string key, size;
  string line;
  string sPid = to_string(pid);
  std::ifstream stream(kProcDirectory + sPid + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> size){
        if (key == "VmSize:"){
          int mbSize = floor(std::stoi(size) / 100);
          return std::to_string(mbSize);
        }
      }
    }
  }
  return string();
}

// Done.
string LinuxParser::Uid(int pid) { 
  string key, uid;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key >> uid){
      if (key == to_string(pid)){
        return uid;
      }
    }
  }
  return string();
}

// Done.
string LinuxParser::User(int pid) {   
  string key, uid;
  string line;
  string sPid = to_string(pid);
  std::ifstream stream(kProcDirectory + sPid + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> uid){
        if (key == "Uid:"){
          return LinuxParser::Username(uid);
        }
      }
    }
  }
  return string();
}

string LinuxParser::Username(string uid) {
  string username, spacer, id;
  string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> spacer >> id){
        if (uid == id){
          return username;
        }
      }
    }
  }
  return string();
}

//TODO: Process CPU
vector<string> LinuxParser::CpuUtilization(int pid) { 
  string line;
  string value;
  vector<string> list;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        list.push_back(value);
      }
    }
  }
  return list;
}


// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
