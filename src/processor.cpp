#include "processor.h"
#include "linux_parser.h"

//Return the aggregate CPU utilization
float Processor::Utilization() { 
  long total_jiffies = LinuxParser::Jiffies();
  long active_jiffies = LinuxParser::ActiveJiffies();
  float cpu_utilization = (float) active_jiffies/total_jiffies;
  return cpu_utilization;
}