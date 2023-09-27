#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"

using std::string;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  //Accessors
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization();                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  
  //Operator Overloading
  bool operator>(Process const& a) const;  // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp
  
  //Mutators
  void Pid(int pid);                       // See src/process.cpp
  void User(int pid);                      // See src/process.cpp
  void Command(int pid);                   // See src/process.cpp
  void CpuUtilization(int pid);            // See src/process.cpp
  void Ram(int pid);                       // See src/process.cpp
  void UpTime(int pid);                    // See src/process.cpp
  
 private:
  int pid_;
  float cpu_utilization_;
  string command_, ram_, user_;
  long uptime_;
};

#endif