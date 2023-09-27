#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <fstream>

#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

//Return the system's CPU
Processor& System::Cpu() { return cpu_; }

//Return a container composed of the system's processes (class composition)
vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();
  processes_.clear();
  for(int pid:pids){
    Process proc;
    proc.Pid(pid);
    proc.User(pid);
    proc.Command(pid);
    proc.CpuUtilization(pid);
    proc.Ram(pid);
    proc.UpTime(pid);
    processes_.push_back(proc);
  }
  std::sort(processes_.begin(), processes_.end());
  return processes_; 
}

// System Class Accessors
std::string System::Kernel() {return LinuxParser::Kernel();}
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization();}
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem();}
int System::RunningProcesses() {return LinuxParser::RunningProcesses();}
int System::TotalProcesses() {return LinuxParser::TotalProcesses();}
long int System::UpTime() { return LinuxParser::UpTime(); }
