#ifndef ANALOG_IN_HPP
#define ANALOG_IN_HPP
#define ADC_PATH "/sys/bus/iio/devices/iio:device0/in_voltage"

#include "util/util.hpp"

#include <iostream>
#include <unistd.h>
#include <fstream>
#include <string>
#include <sstream>

namespace BBB {

    int readAnalog(int number);
            
    void setRealTimeCurrentThread();

    void openAnalog(fstream &fs, int number);

    int readAnalogFast(fstream &fs);

    void closeAnalog(fstream &fs);
}


#endif /* ANALOG_IN_HPP */
