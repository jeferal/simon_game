#include "simon_buttons.hpp"

#include <iostream>
#include <unistd.h>
#include <functional>

SimonButtons::SimonButtons() {

    inButton[0] = new GPIO(BUTTON_GPIO::RED_BUTTON);
    inButton[1] = new GPIO(BUTTON_GPIO::GREEN_BUTTON);
    inButton[2] = new GPIO(BUTTON_GPIO::YELLOW_BUTTON);
    inButton[3] = new GPIO(BUTTON_GPIO::BLUE_BUTTON);
    initButton = new GPIO(BUTTON_GPIO::INIT_BUTTON);
    pauseButton = new GPIO(BUTTON_GPIO::PAUSE_BUTTON);

    inButton[0]->setDirection(GPIO::INPUT);
    inButton[1]->setDirection(GPIO::INPUT);
    inButton[2]->setDirection(GPIO::INPUT);
    inButton[3]->setDirection(GPIO::INPUT);
    initButton->setDirection(GPIO::INPUT);
    pauseButton->setDirection(GPIO::INPUT);
    
    initButton->setActiveHigh();
    initButton->setEdgeType(GPIO::RISING);
	initButton->setDebounceTime(200);
	initButton->waitForEdge(init_event_handler);

    pauseButton->setActiveHigh();
    pauseButton->setEdgeType(GPIO::RISING);
	pauseButton->setDebounceTime(200);
	pauseButton->waitForEdge(pause_event_handler);
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

int init_event_handler(int arg) {
    printf("INIT BUTTON PRESSED!!!\n");
    usleep(1000000);
    return 0;
}

int pause_event_handler(int arg) {
    printf("PAUSE BUTTON PRESSED!!!\n");
    usleep(1000000);
    return 0;
}