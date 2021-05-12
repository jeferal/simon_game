#ifndef SIMON_LEDS_HPP_
#define SIMON_LEDS_HPP_

#include "../gpio_bbb/GPIO.hpp"

#include <vector>

using namespace BBB;


class SimonLeds {
    public:
        enum COLOR{ RED=0, GREEN=1, YELLOW=2, BLUE=3 };

    private:
        enum LED_GPIO{ RED_LED=60, GREEN_LED=31, YELLOW_LED=50, BLUE_LED=48};
        GPIO *outLed[4];
        
    public:

        SimonLeds();
        void turn_on(COLOR color);
        void turn_off(COLOR color);
        void show_array(const std::vector<bool> &status);
        void turn_all_off();
        void turn_all_on();

        SimonLeds::COLOR turn_on_random();

};

#endif /* SIMON_LEDS_HPP_ */
