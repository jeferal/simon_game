#include "simon_buttons.hpp"

#include <iostream>

SimonButtons::SimonButtons() {

    srand (time(NULL));

    inButton[0] = new GPIO(BUTTON_GPIO::RED_BUTTON);
    inButton[1] = new GPIO(BUTTON_GPIO::GREEN_BUTTON);
    inButton[2] = new GPIO(BUTTON_GPIO::YELLOW_BUTTON);
    inButton[3] = new GPIO(BUTTON_GPIO::BLUE_BUTTON);

    inButton[0]->setDirection(GPIO::INPUT);
    inButton[1]->setDirection(GPIO::INPUT);
    inButton[2]->setDirection(GPIO::INPUT);
    inButton[3]->setDirection(GPIO::INPUT);
    
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