#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//-----------------------------------------------------------------
//SYSTEM
//-----------------------------------------------------------------

// Read and return the operating system
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

//Read and return the system kernel
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

// Read and return the PIDs of all active processes
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

//Read and return the number of running processes
int LinuxParser::RunningProcesses(){
  std::string path = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  string RunningProcs = LinuxParser::ExtractKeyVal("procs_running",path);
  return std::stoi(RunningProcs);
}

//Read and return the total number of active processes
int LinuxParser::TotalProcesses(){
  std::string path = LinuxParser::kProcDirectory + LinuxParser::kStatFilename;
  string TotalProcs = LinuxParser::ExtractKeyVal("processes",path);
  return std::stoi(TotalProcs);
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string line, nonvalue, value;
  std::vector<std::string> MemMetrics;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    for(int i=0; i<2; i++){		//Collect the first two lines
      std::getline(stream,line);
      std::istringstream linestream(line);
      linestream>>nonvalue>>value>>nonvalue;	//Extract the value from each line
      MemMetrics.push_back(value);
    };
    float MemTotal = std::stof(MemMetrics[0]);
    float MemFree = std::stof(MemMetrics[1]);
    float TotalUsedMem = MemTotal - MemFree;
    return TotalUsedMem/MemTotal;
  };
  std::cout<<"Could not open memory utilization file!\n";
  return 0.0; }

//Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line;
  std::string uptimestr = "0.0";
  long uptime = 0.0;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptimestr;
    return std::stol(uptimestr);
  }
  else{
    return uptime;
  }
}

//Read and return the total number of processes
/*
int LinuxParser::TotalProcesses() {
  std::string line, nonvalue, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    for(int i=0; i<21; i++){		//Collect the first nine lines
      std::getline(stream,line);
      std::istringstream linestream(line);
      linestream>>nonvalue>>value;	//Overwrite the extracted value until the desired line
    };
    return std::stoi(value);
  }
  else {
    std::cout<<"Could not open the meminfo file!\n";
    return 0;
  }
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::string line, nonvalue, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    for(int i=0; i<22; i++){		//Collect the first nine lines
      std::getline(stream,line);
      std::istringstream linestream(line);
      linestream>>nonvalue>>value;	//Extract the value from each line, overwriting until the desired line
    };
    return std::stoi(value);
  }
  else {
    std::cout<<"Could not open the meminfo file!\n";
    return 0;
  }
}
*/
//----------------------------------------------------------------------
// UTILITIES
//----------------------------------------------------------------------

//Extract a value from a file based on locating it's corresponding label
string LinuxParser::ExtractKeyVal(std::string key, std::string path){
  std::string Value = "N/A";
  std::string line, Key, value;
  bool KeyFound = false;
  std::ifstream stream(path);
  if(stream.is_open()){
    while(KeyFound == false && stream.peek() != EOF){
      std::getline(stream, line);
      std::istringstream linestream(line);
      linestream>>Key>>value;
      if(Key==key){
        KeyFound = true;
        Value = value;
      }
    }
  }
  return Value;
}

//-----------------------------------------------------------------------
// Processor
//-----------------------------------------------------------------------

//Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<std::string> AggCPUTimers;
  std::string line, SingleTimer, SkipLabel;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>SkipLabel;		//Skip the aggregate cpu label at the beginning of the line
    for(int i=0;i<10;i++){
      linestream>>SingleTimer;	//Extract the values of the ten actual timers contained in the line
      AggCPUTimers.push_back(SingleTimer);	//Add each aggregate timer to the list
    }
  }
  return AggCPUTimers; 
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<std::string> jiffies = CpuUtilization();
  long total_jiffies = 0;
  for(std::string jiffie:jiffies){
    total_jiffies += std::stol(jiffie);
  }
  return total_jiffies;
}

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<std::string> jiffies = CpuUtilization();
  long idle_jiffies = 0;
  long idle = std::stol(jiffies[3]);
  long iowait = std::stol(jiffies[4]);
  idle_jiffies = idle + iowait;
  return idle_jiffies; 
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active_jiffies = 0;
  active_jiffies = Jiffies() - IdleJiffies();
  return active_jiffies; 
}

//-----------------------------------------------------------------
// Processes
//-----------------------------------------------------------------

//Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){std::getline(stream,command);};
  return command;
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::string ram;
  std::string path = kProcDirectory + to_string(pid) + kStatusFilename;
  ram = ExtractKeyVal("VmSize:",path);
  return std::to_string(std::stoi(ram)/1000); //Convert RAM from KB to MB
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  std::string path = kProcDirectory + std::to_string(pid) + kStatusFilename;
  return ExtractKeyVal("Uid:",path); 
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  std::string uid = Uid(pid);
  std::string user = "N/A";
  std::string test_user = "Not Found";
  std::string line, skip, test_uid;
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(user!=test_user && stream.peek() != EOF){
      std::getline(stream,line);
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line); 
      linestream >> test_user >> skip >> test_uid;
      if(uid == test_uid) {
        user = test_user;
      }
    }
  }
  return user;
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  long uptime = 0;
  std::string line, skip;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    for(int i = 1;i<22;i++){
      linestream>>skip;
    }
    linestream>>uptime;
  }
  return uptime/sysconf(_SC_CLK_TCK);
}

//Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { 
//  return 0; 
//}

//Read and return the cpu utilization of a process
int LinuxParser::CpuUtilization(int pid){
  std::string line, skip, uptime, utime, stime, cutime, cstime, starttime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  int total_time;
  int cpu_utilization = 0;
  if(stream.is_open()){
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream>>uptime;
    for(int i=2;i<14;i++){linestream>>skip;}
    linestream>>utime>>stime>>cutime>>cstime;
    for(int i=18;i<22;i++){linestream>>skip;}
    linestream>>starttime;
    total_time = std::stoi(utime) + std::stoi(stime);
    total_time = total_time + std::stoi(cutime) + std::stoi(cstime);
    int Hertz = sysconf(_SC_CLK_TCK);
    int seconds = std::stoi(uptime) - (std::stoi(starttime) / Hertz);
    cpu_utilization = 100 * ((total_time / Hertz) / seconds);
  }
  return cpu_utilization;
}