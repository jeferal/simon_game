#include "simon_leds.hpp"

SimonLeds::SimonLeds(int number) {

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

SimonLeds::COLOR SimonLeds::turn_on_random() {
    COLOR rand_color = COLOR(rand()%4);
    turn_on(rand_color);
    return rand_color;
}