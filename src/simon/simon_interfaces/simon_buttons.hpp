#ifndef SIMON_BUTTONS_HPP_
#define SIMON_BUTTONS_HPP_

#include "../../gpio_bbb/GPIO.hpp"
#include <vector>

using namespace BBB;

//Preguntar duda
//Cómo pasar esta función si estuviera dentro de la clase
//Cómo darle argumentos

class SimonButtons {
    public:
        enum COLOR{ RED=0, GREEN=1, YELLOW=2, BLUE=3, INIT=4, PAUSE=5 };

    private:
        enum BUTTON_GPIO{ RED_BUTTON=30, GREEN_BUTTON=51, YELLOW_BUTTON=5, BLUE_BUTTON=26, INIT_BUTTON=45, PAUSE_BUTTON=44};
        GPIO *inButton[6];
        
    public:

        SimonButtons();
        bool read_button(COLOR color);
        std::vector<bool> read_status(bool=false);
        bool get_set_interruption(bool &status);

        void set_init_interruption(BBB::CallbackType function);
        void set_pause_interruption(BBB::CallbackType function);

};

#endif /* SIMON_BUTTONS_HPP_ */