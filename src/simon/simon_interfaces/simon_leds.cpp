#include "simon_leds.hpp"

SimonLeds::SimonLeds() {
    srand (time(NULL));

    outLed[0] = new GPIO(LED_GPIO::RED_LED);
    outLed[1] = new GPIO(LED_GPIO::GREEN_LED);
    outLed[2] = new GPIO(LED_GPIO::YELLOW_LED);
    outLed[3] = new GPIO(LED_GPIO::BLUE_LED);

    outLed[0]->setDirection(GPIO::OUTPUT);
    outLed[1]->setDirection(GPIO::OUTPUT);
    outLed[2]->setDirection(GPIO::OUTPUT);
    outLed[3]->setDirection(GPIO::OUTPUT);

    turn_off(COLOR::RED);
    turn_off(COLOR::GREEN);
    turn_off(COLOR::YELLOW);
    turn_off(COLOR::BLUE);

}

void SimonLeds::turn_on(COLOR color) {
    outLed[color]->setValue(GPIO::HIGH);
}

void SimonLeds::turn_off(COLOR color) {
    outLed[color]->setValue(GPIO::LOW);
}

void SimonLeds::turn_all_off() {
    for(int i=0; i<4; i++)
        turn_off((COLOR)i);
}

void SimonLeds::turn_all_on() {
    for(int i=0; i<4; i++)
        turn_on((COLOR)i);
}


void SimonLeds::show_array(const std::vector<bool> &status) {
    for(int i=0; i<4; i++) {
        if(status[i])
            turn_on((COLOR)i);
        else
            turn_off((COLOR)i);
    }
} 

SimonLeds::COLOR SimonLeds::turn_on_random() {
    COLOR rand_color = COLOR(rand()%4);
    turn_on(rand_color);
    return rand_color;
}