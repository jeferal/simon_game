#include "analog_input.hpp"

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

namespace BBB {

    int readAnalog(int number) {
        stringstream ss;
        ss << ADC_PATH << number << "_raw";
        fstream fs;
        fs.open(ss.str().c_str(), fstream::in);
        fs >> number;
        fs.close();
        return number;
    }


    void setRealTimeCurrentThread() {
    // If pid equals zero, the scheduling policy and attributes
    // of the calling thread will be set.
    int my_pid = 0;
    struct sched_param param;
    int retval;
    int low_priority, high_priority;

    // Get min and max priority values of SCHED_FIFO policy
    high_priority = sched_get_priority_max(SCHED_FIFO);
    if (high_priority == -1) {
        perror("Error in sched_get_priority_max"); _exit(-1);
    }
    low_priority = sched_get_priority_min(SCHED_FIFO);
    if (low_priority == -1) {
        perror("Error in sched_get_priority_min"); _exit(-1);
    }

    // Change to SCHED_FIFO and medium priority.
    param.sched_priority = 50; //(high_priority + low_priority) / 2;
    retval = sched_setscheduler(my_pid, SCHED_FIFO, &param);
    if (retval == -1) {
        perror("Error in sched_setscheduler"); _exit(-1);
    }
    }
    /**
     *
     */
    void openAnalog(fstream &fs, int number) {
    stringstream ss;
    ss << ADC_PATH << number << "_raw";
    fs.open(ss.str().c_str(), fstream::in);
    }
    /**
     *
     */
    int readAnalogFast(fstream &fs) {
        int sample;
        fs >> sample;
        fs.seekg(0);
        return sample;
    }
    /**
     *
     */
    void closeAnalog(fstream &fs){
        fs.close();
    }

}