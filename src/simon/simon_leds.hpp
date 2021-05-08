#ifndef SIMON_LEDS_HPP_
#define SIMON_LEDS_HPP_

#include "../gpio_bbb/GPIO.hpp"

using namespace BBB;

class SimonLeds {


    public:

        GPIO *outLed1=NULL;
        GPIO *outLed2=NULL;
        GPIO *outLed3=NULL;
        GPIO *outLed4=NULL;

        SimonLeds(int number);

};

#endif /* SIMON_LEDS_HPP_ */
