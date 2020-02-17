#include "processor.h"
#include "linux_parser.h"
#include <string>

using std::string;

Processor::Processor() {
    //Parsed Dictionary from /proc/stat to get CPU data
    // Keys: "user", "nice", "system", "idle", "iowait", "irq", "softirq", "steal", "guest", "guest_nice"
    cpu_data = LinuxParser::Jiffies();
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long prev_active, prev_idle, active, idle, total_change, active_change;
    //Use data to calculate active and idle time
    prev_active = cpu_data["user"] + cpu_data["nice"] + cpu_data["system"] + cpu_data["irq"] + cpu_data["softirq"] + cpu_data["steal"];
    prev_idle = cpu_data["idle"] + cpu_data["iowait"];
    //Reparse files to get new current data
    cpu_data = LinuxParser::Jiffies();
    active = cpu_data["user"] + cpu_data["nice"] + cpu_data["system"] + cpu_data["irq"] + cpu_data["softirq"] + cpu_data["steal"];
    idle = cpu_data["idle"] + cpu_data["iowait"];
    total_change = ((active+idle) - (prev_active+prev_idle));
    active_change = (active-prev_active);
    //Return percentage of active time for CPU Utilization
    return ((float)(active_change)/(total_change));
}