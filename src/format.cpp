#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours, mins, secs;
    //Calcuate number of hours
    hours = seconds/3600;
    //Calculate number of minutes after hours removed
    mins = (seconds - hours*3600)/60;
    //Calculate number of seconds after hours and mins removed
    secs = (seconds - hours*3600 - mins*60);
    return (std::to_string(hours) + ":" + std::to_string(mins) + ":" + std::to_string(secs));
 }