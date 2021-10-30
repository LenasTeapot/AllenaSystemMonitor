#include <string>
#include <math.h>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string _sSeconds;
    string _sMinutes;
    string _sHours;

    int mins = floor(seconds/60);
    int hours = floor(mins/60);
    _sHours = to_string(hours);
    _sMinutes = to_string(mins - (hours * 60));
    _sSeconds = to_string(seconds - (mins * 60));

    return (reformat(_sHours) + ":" + reformat(_sMinutes) + ":" + reformat(_sSeconds)); 
}


string Format::reformat(string num) {
    std::ostringstream out;
    out << std::internal << std::setfill('0') << std::setw(2) << num;
    return out.str();
}

