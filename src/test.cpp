#include <iostream>
#include <unistd.h>

#include "simon/simon_leds.hpp"

/*
arm-linux-gnueabihf-g++-8 src/test.cpp src/analog_bbb/analog_input.cpp src/state_monitor/state_monitor.cpp src/state_monitor/thread_conf.cpp src/util/util.cpp src/analog_bbb/PWMuniv.cpp src/gpio_bbb/GPIO.cpp src/simon/simon_leds.cpp -o build/test -lpthread

scp build/test root@192.168.1.111:/root/targets/
*/

int main() {

    std::cout << "Starting program" << std::endl;

    SimonLeds simon_leds_out(1);

    while(true) {
        simon_leds_out.outLed1->setValue(GPIO::HIGH);
        usleep(1000000);
        simon_leds_out.outLed1->setValue(GPIO::LOW);
        usleep(1000000);
        simon_leds_out.outLed2->setValue(GPIO::HIGH);
        usleep(1000000);
        simon_leds_out.outLed2->setValue(GPIO::LOW);
        usleep(1000000);
        simon_leds_out.outLed3->setValue(GPIO::HIGH);
        usleep(1000000);
        simon_leds_out.outLed3->setValue(GPIO::LOW);
        usleep(1000000);
        simon_leds_out.outLed4->setValue(GPIO::HIGH);
        usleep(1000000);
        simon_leds_out.outLed4->setValue(GPIO::LOW);
        usleep(1000000);
    }

    return 0;
}