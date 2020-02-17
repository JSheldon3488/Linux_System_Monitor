#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>

#include "process.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
       process_data = LinuxParser::Jiffies(pid);
       system_uptime = LinuxParser::UpTime();
       cpu_util = Process::CpuUtilization();
}
 
// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long total_time_running, total_time_alive, hertz;
    hertz = sysconf(_SC_CLK_TCK);
    total_time_running = (process_data["utime"] + process_data["stime"] + process_data["cutime"] + process_data["cstime"])/hertz;
    total_time_alive = system_uptime - (process_data["starttime"]/hertz);
    return ((float)(total_time_running)/total_time_alive);
 }

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// TODO: Return this process's memory utilization
string Process::Ram() { return (to_string(LinuxParser::Ram(pid_)/1000)); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(LinuxParser::Uid(pid_)); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return process_data["starttime"]/sysconf(_SC_CLK_TCK); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(const Process& a) const { return (this->cpu_util > a.cpu_util); }