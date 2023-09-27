#include <string>

#include "format.h"
#define HOUR 3600
#define MINUTE 60

using std::string;

// Format time values
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
  int hours, minutes, sec;
  hours = seconds/HOUR;
  minutes = (seconds % HOUR)/MINUTE;
  sec = (seconds % HOUR) % MINUTE;
  return (std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(sec));
}