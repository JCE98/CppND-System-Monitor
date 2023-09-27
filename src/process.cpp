#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

//Process Class Accessors
int Process::Pid() { return pid_; }
float Process::CpuUtilization() { return cpu_utilization_; }
string Process::Command() { return command_; }
string Process::Ram() { return ram_; }
string Process::User() { return user_; }
long int Process::UpTime() { return uptime_; }

//Overload the "less than" and "greater than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
  return cpu_utilization_ > a.cpu_utilization_;
}
bool Process::operator<(Process const& a) const {
  return cpu_utilization_ < a.cpu_utilization_;
}

// Process Class Mutators
void Process::Pid(int pid){pid_ = pid;}
void Process::User(int pid){user_ = LinuxParser::User(pid);}
void Process::Command(int pid){command_ = LinuxParser::Command(pid);}
void Process::CpuUtilization(int pid){cpu_utilization_ = LinuxParser::CpuUtilization(pid);}
void Process::Ram(int pid){ram_ = LinuxParser::Ram(pid);} 
void Process::UpTime(int pid){uptime_ = LinuxParser::UpTime(pid);}