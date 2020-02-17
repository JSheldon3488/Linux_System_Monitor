#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line, key; 
  int mem_total = -1;    //-1 used as flag for unset value
  int mem_free = -1;
  int possible_value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  // Getting and set correct values of total memory and free memory
  if (filestream.is_open()){
    while (mem_total == -1 || mem_free == -1){
      std::getline(filestream,line);
      std::istringstream linestream(line);
      linestream >> key >> possible_value;
      if (key == "MemTotal:") {
        mem_total = possible_value;
      }
      if (key == "MemFree:") {
        mem_free = possible_value;
      }
    }
  }
  // Return Memory Utilization as the percentage of memory in use
  return ((float)(mem_total-mem_free)/(float)(mem_total));
 }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  long time;
  string line;
  std::ifstream filestream(kProcDirectory+kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> time;
    return time;
  }
  return -1;
 }

// TODO: Read and return the number of jiffies for the system
// Decided to return a map of all the data I need from the Processor Jiffies
std::unordered_map<string,long> LinuxParser::Jiffies() {
  std::unordered_map<string,long> processor_stats;
  string key, line;
  long user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream filestream(kProcDirectory+kStatFilename);
  
  if (filestream.is_open()){
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    if (key == "cpu") {
      processor_stats["user"] = user;
      processor_stats["nice"] = nice;
      processor_stats["system"] = system;
      processor_stats["idle"] = idle;
      processor_stats["iowait"] = iowait;
      processor_stats["irq"] = irq;
      processor_stats["softirq"] = softirq;
      processor_stats["steal"] = steal;
      processor_stats["guest"] = guest;
      processor_stats["guest_nice"] = guest_nice;
      filestream.close();
      return processor_stats;
    }
  }
  throw "Linux Parser Jiffies Error";
 }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  int processes;
  string key, line;
  std::ifstream filestream(kProcDirectory+kStatFilename);

  if (filestream.is_open()){
    while (std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> processes;
      if (key == "processes") {
        return processes;
      }
    }
  }
  return -1;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running;
  string key, line;
  std::ifstream filestream(kProcDirectory+kStatFilename);

  if (filestream.is_open()){
    while (std::getline(filestream,line)){
      std::istringstream linestream(line);
      linestream >> key >> running;
      if (key == "procs_running") {
        return running;
      }
    }
  }
  return -1;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
// Decided to return a map of all the data I need from the Process Jiffies
std::unordered_map<std::string,long> LinuxParser::Jiffies(int pid) { 
  std::unordered_map<string,long> process_data;
  string line;
  string d1, d2, d3, d4, d5, d6, d7, d8, d9, d10, d11, d12, d13, d18, d19, d20, d21; //dummy strings to get to values we need
  long utime, stime, cutime, cstime, starttime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/" + kStatFilename);

  if (filestream.is_open()) {
    std::getline(filestream,line);
    std::istringstream linestream(line);
    linestream >> d1 >> d2 >> d3 >> d4 >> d5 >> d6 >> d7 >> d8 >> d9 >> d10 >> d11 >> d12 >> d13 >> utime >> stime >> cutime >> cstime >> d18 >> d19 >> d20 >> d21 >> starttime;
    process_data["utime"] = utime;
    process_data["stime"] = utime;
    process_data["cutime"] = utime;
    process_data["cstime"] = utime;
    process_data["starttime"] = utime;
  }
  return process_data;
 }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/" + kCmdlineFilename);

  if (filestream.is_open()) {
    std::getline(filestream,line);
    return line;
  }
  return "Error";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::Ram(int pid) { 
  string key, line;
  long ram;
  std::ifstream filestream(kProcDirectory + to_string(pid) + "/" + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> ram;
      if (key == "VmSize:") {
        return ram;
      }
    }
  }
  return -1;
 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string key, uid, line;
  std::ifstream filestream(kProcDirectory+ to_string(pid) + "/" + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      }
    }
  }
  return "Uid not found";
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(std::string uid) {
  string user_name, dummy, id, line;
  std::ifstream filestream(kPasswordPath);

  if (filestream.is_open()) {
    while (std::getline(filestream,line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user_name >> dummy >> id;
      if (id == uid) {
        return user_name;
      }
    }
  }
  return "User_name not found";
}