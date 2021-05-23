#include "simon_buttons.hpp"

#include <iostream>
#include <unistd.h>
#include <functional>

SimonButtons::SimonButtons() {

    inButton[0] = new GPIO(BUTTON_GPIO::RED_BUTTON);
    inButton[1] = new GPIO(BUTTON_GPIO::GREEN_BUTTON);
    inButton[2] = new GPIO(BUTTON_GPIO::YELLOW_BUTTON);
    inButton[3] = new GPIO(BUTTON_GPIO::BLUE_BUTTON);
    inButton[4] = new GPIO(BUTTON_GPIO::INIT_BUTTON);
    inButton[5] = new GPIO(BUTTON_GPIO::PAUSE_BUTTON);

    inButton[0]->setDirection(GPIO::INPUT);
    inButton[1]->setDirection(GPIO::INPUT);
    inButton[2]->setDirection(GPIO::INPUT);
    inButton[3]->setDirection(GPIO::INPUT);
    inButton[4]->setDirection(GPIO::INPUT);
    inButton[5]->setDirection(GPIO::INPUT);
}

void SimonButtons::set_init_interruption(BBB::CallbackType function) {
    inButton[4]->setActiveHigh();
    inButton[4]->setEdgeType(GPIO::RISING);
	inButton[4]->setDebounceTime(200);
	inButton[4]->waitForEdge(function);
}

void SimonButtons::set_pause_interruption(BBB::CallbackType function) {
    inButton[5]->setActiveHigh();
    inButton[5]->setEdgeType(GPIO::RISING);
	inButton[5]->setDebounceTime(200);
	inButton[5]->waitForEdge(function);
}

bool SimonButtons::read_button(COLOR color) {
    return inButton[color]->getValue()==GPIO::HIGH;
}

std::vector<bool> SimonButtons::read_status(bool show) {
    std::vector<bool> buttons_status(4,0);
    
    buttons_status[0] = read_button(RED);
    buttons_status[1] = read_button(GREEN);
    buttons_status[2] = read_button(YELLOW);
    buttons_status[3] = read_button(BLUE);

    if(show) {
        std::cout << "Button Status: (";
        for(int i=0; i<4; i++) {
            std::cout << buttons_status[i] << ",";
        }
        std::cout << ")" << std::endl;
    }

    return buttons_status;
}

bool SimonButtons::get_set_interruption(bool &status) {
    bool value = status;
    if (status)
        status = false;
        
    return value;
}