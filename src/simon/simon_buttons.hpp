#ifndef SIMON_BUTTONS_HPP_
#define SIMON_BUTTONS_HPP_

#include "../gpio_bbb/GPIO.hpp"
#include <vector>

using namespace BBB;

class SimonButtons {
    public:
        enum COLOR{ RED=0, GREEN=1, YELLOW=2, BLUE=3 };

    private:
        enum BUTTON_GPIO{ RED_BUTTON=30, GREEN_BUTTON=51, YELLOW_BUTTON=5, BLUE_BUTTON=4};
        GPIO *inButton[4];
        
    public:

        SimonButtons();
        bool read_button(COLOR color);
        std::vector<bool> read_status(bool=false);

};

#endif /* SIMON_BUTTONS_HPP_ */