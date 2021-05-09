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

/*
arm-linux-gnueabihf-g++-8 src/test.cpp src/analog_bbb/analog_input.cpp src/state_monitor/state_monitor.cpp src/state_monitor/thread_conf.cpp src/util/util.cpp src/analog_bbb/PWMuniv.cpp src/gpio_bbb/GPIO.cpp src/simon/simon_leds.cpp src/simon/simon_buttons.cpp -o build/test -lpthread

scp build/test root@192.168.1.111:/root/targets/
*/

StateMonitor stateManager;
SimonLeds simon_leds_out;
SimonButtons simon_buttons_in;

void *init_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        printf("INIT STATE\n");
        //Do something
        usleep(600000);
    }
}

void *show_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        printf("SHOWING RANDOM COLOR\n");
        SimonLeds::COLOR color = simon_leds_out.turn_on_random();
        usleep(200000);
        simon_leds_out.turn_off(color);
        usleep(200000);
    }
}

void *introduce_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter = 0;
    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        printf("READING BUTTONS\n");
        std::vector<bool> status = simon_buttons_in.read_status(true);
        //Do something
        usleep(200000);
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

    while(true) {
        myState = (stateManager.getState() + 1) % 3;
        stateManager.changeState(myState);
        usleep(5000000);
    }

    pthread_join(th01,NULL);
    pthread_join(th02,NULL);
    pthread_join(th03,NULL);

    return 0;
}