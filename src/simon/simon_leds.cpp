#include "simon_leds.hpp"

SimonLeds::SimonLeds(int number) {

    outLed1 = new GPIO(60);
    outLed2 = new GPIO(31);
    outLed3 = new GPIO(50);
    outLed4 = new GPIO(48);

    outLed1->setDirection(GPIO::OUTPUT);
    outLed2->setDirection(GPIO::OUTPUT);
    outLed3->setDirection(GPIO::OUTPUT);
    outLed4->setDirection(GPIO::OUTPUT);

    outLed1->setValue(GPIO::LOW);
    outLed2->setValue(GPIO::LOW);
    outLed3->setValue(GPIO::LOW);
    outLed4->setValue(GPIO::LOW);
}