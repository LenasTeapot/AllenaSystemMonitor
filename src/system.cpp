#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    vector<int> pids = LinuxParser::Pids();
    for (unsigned int i = 0; i <= sizeof(pids); i++){
        Process process(pids[i]);
        // I assume if these two have values the others will as well
        if (!process.Ram().empty() && !process.Command().empty()) {
            processes_.emplace_back(process);
        }  
    }

    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

std::string System::Kernel() { return LinuxParser::Kernel(); }

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

long System::UpTime() { return LinuxParser::UpTime(); }
