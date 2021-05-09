#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>

#include "simon/simon_leds.hpp"
#include "simon/simon_buttons.hpp"

#include "state_monitor/state_monitor.hpp"
#include "state_monitor/thread_conf.hpp"

#define INIT_STATE 0
#define SHOW_STATE 1
#define INTRODUCE_STATE 2
#define MAX_SEQUENCE 100

/*
arm-linux-gnueabihf-g++-8 src/test.cpp src/analog_bbb/analog_input.cpp src/state_monitor/state_monitor.cpp src/state_monitor/thread_conf.cpp src/util/util.cpp src/analog_bbb/PWMuniv.cpp src/gpio_bbb/GPIO.cpp src/simon/simon_leds.cpp src/simon/simon_buttons.cpp -o build/test -lpthread

scp build/test root@192.168.1.111:/root/targets/
*/

StateMonitor stateManager;
SimonLeds simon_leds_out;
SimonButtons simon_buttons_in;
int n=1;
std::vector<SimonLeds::COLOR> current_sequence(MAX_SEQUENCE);

void *init_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        printf("INIT STATE\n");

        if(simon_buttons_in.read_button(SimonButtons::COLOR::INIT)) {
            printf("INIT BUTTON PRESSED!!!\n");
            stateManager.changeState(SHOW_STATE);
        }
        usleep(600000);
    }
}

void *show_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        SimonLeds::COLOR current_color;
        if(iter < n-1) {
            //Show the vector
            current_color = current_sequence[iter];
            simon_leds_out.turn_on(current_color);
        } else {
            //Show random and store it
            current_color = simon_leds_out.turn_on_random();
            current_sequence[iter] = current_color;
        }
        //Time on
        usleep(1000000);

        simon_leds_out.turn_off(current_color);
        //Time off
        usleep(1000000);
        iter++;

        if(iter==n) {
            printf("SECUENCIA TERMINADA\n");
            stateManager.changeState(INTRODUCE_STATE);
            iter=0;
            n++;
        }

        if(simon_buttons_in.read_button(SimonButtons::COLOR::INIT)) {
            printf("INIT BUTTON PRESSED!!!\n");
            stateManager.changeState(INIT_STATE);
        } 
    }
}

void *introduce_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        //Read all buttons
        std::vector<bool> status = simon_buttons_in.read_status(true);
        usleep(10000);

        if(iter == n-1) {
            printf("SEQUENCE COMPLETED!!\n");
            iter=0;
            stateManager.changeState(SHOW_STATE);
        }
        printf("LED NUMBER: %d",(int)current_sequence[iter]);

        if(status[(int)current_sequence[iter]]) {
            iter++;
            printf("CORRECT BUTTON!\n");
        }

        if(simon_buttons_in.read_button(SimonButtons::COLOR::INIT)) {
            printf("INIT BUTTON PRESSED!!!\n");
            stateManager.changeState(INIT_STATE);
        }
    }
}
//void *pause_thread();

void *changeStateHandler(int stFrom, int stTo) {
    printf("********************** Cambio de estado: desde %d a %d.\n",stFrom,stTo);
    return(NULL);
}


int main() {

    std::cout << "Starting program" << std::endl;

    pthread_t th01, th02, th03, th04;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    stateManager.addStateChangeListener(0,1,changeStateHandler);
    stateManager.addStateChangeListener(1,2,changeStateHandler);
    stateManager.addStateChangeListener(2,1,changeStateHandler);
    stateManager.addStateChangeListener(1,0,changeStateHandler);
    stateManager.addStateChangeListener(2,0,changeStateHandler);

    ThreadConf h01Cfg;
    h01Cfg.addState(0);
    h01Cfg.setArg((void*)200);

    ThreadConf h02Cfg;
    h02Cfg.addState(1);
    h02Cfg.setArg((void*)250);

    ThreadConf h11Cfg;
    h11Cfg.addState(2);
    h11Cfg.setArg((void*)100);

    pthread_create(&th01,&attr,init_thread,(void*)&h01Cfg);
    pthread_create(&th02,&attr,show_thread,(void*)&h02Cfg);
    pthread_create(&th03,&attr,introduce_thread,(void*)&h11Cfg);

    int myState = 0;
    stateManager.changeState(INIT_STATE);

    while(true);    

    pthread_join(th01,NULL);
    pthread_join(th02,NULL);
    pthread_join(th03,NULL);

    return 0;
}