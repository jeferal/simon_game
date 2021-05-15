#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>

#include "simon/simon_interfaces/simon_leds.hpp"
#include "simon/simon_interfaces/simon_buttons.hpp"
#include "simon/simon_led_strip.hpp"
#include "simon/simon_interfaces/simon_dial.hpp"
#include "simon/simon_sequence.hpp"

#include "state_monitor/state_monitor.hpp"
#include "state_monitor/thread_conf.hpp"

#define INIT_STATE 0
#define SHOW_STATE 1
#define INTRODUCE_STATE 2
#define MAX_SEQUENCE 100


StateMonitor stateManager;
SimonLeds simon_leds_out;
SimonButtons simon_buttons_in;
SimonLedStrip simon_led_strip("192.168.1.117",80);
SimonDial simon_dial_difficulty;
SimonSequence simon_sequence;


int vel_show = 1000000;
int time_out = 100;
bool use_leds=false;

void *init_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter=0;
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

    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        SimonLeds::COLOR current_color;

        if(simon_sequence.get_num_steps() < simon_sequence.get_length()-1) {
            //Show the vector
            printf("Showing previous led");
            current_color = simon_sequence.step();
            simon_leds_out.turn_on(current_color);
        } else {
            //Show random and store it
            printf("Showing random led\n");
            current_color = simon_leds_out.turn_on_random();
            simon_sequence.new_step(current_color);
        }

        vel_show = 1172.16*simon_dial_difficulty.get_value() + 1000000/5;

        //Time on
        usleep(vel_show);

        simon_leds_out.turn_off(current_color);

        //Time off
        usleep(vel_show);

        if(simon_sequence.is_finished()) {
            printf("SECUENCIA TERMINADA\n");
            stateManager.changeState(INTRODUCE_STATE);
            continue;
        }

        if(simon_buttons_in.read_button(SimonButtons::COLOR::INIT)) {
            printf("INIT BUTTON PRESSED!!!\n");
            simon_sequence.reset();
            stateManager.changeState(INIT_STATE);
            continue;
        } 
    }
}

void *introduce_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();

    int iter_time_out=0;

    std::vector<bool> pre_status(4);
    pre_status[0] = false;
    pre_status[1] = false;
    pre_status[2] = false;
    pre_status[3] = false;

    for (;;) {
        int state = stateManager.waitState(cfgPassed);

        //Read all buttons
        std::vector<bool> status = simon_buttons_in.read_status(false);
        simon_leds_out.show_array(status);
        std::vector<bool> rising_edges(4);

        for(int i=0; i<4; i++) {
            rising_edges[i] = status[i]==true && pre_status[i]==false;
        }

        pre_status = status;
        
        int current_led = simon_sequence.get_current_led();
        std::cout << "CURRENT LED: " << current_led << "and position: " << simon_sequence.get_num_steps() << std::endl;

        for(int i=0; i<4; i++) {
            if(i==current_led) {
                if(rising_edges[current_led]==true) {
                    simon_sequence.step();
                    std::cout << "Position: " << simon_sequence.get_num_steps() << ", n-1: " << simon_sequence.get_length()-1 << std::endl; 
                    //Reset time out
                    iter_time_out=0;
                    printf("CORRECT BUTTON!\n");
                    if(simon_sequence.is_finished()) {
                        printf("SEQUENCE COMPLETED!!\n");
                        iter_time_out=0;
                        stateManager.changeState(SHOW_STATE);
                        simon_sequence.new_color();

                        for(int i=0; i<4; i++)
                            pre_status[i] = false;
                        continue;
                    }
                }
            }

            else {
                if(rising_edges[i]==true) {
                    printf("WRONG BUTTON!!\n");
                    iter_time_out=0;
                    simon_sequence.reset();
                    stateManager.changeState(INIT_STATE);

                    for(int i=0; i<4; i++)
                            pre_status[i] = false;
                    continue;
                }
            }
        }

        if(simon_buttons_in.read_button(SimonButtons::COLOR::INIT)) {
            printf("INIT BUTTON PRESSED!!!\n");
            iter_time_out=0;
            simon_sequence.reset();
            stateManager.changeState(INIT_STATE);
            for(int i=0; i<4; i++)
                pre_status[i] = false;
            continue;
        }

        iter_time_out++;
        std::cout << iter_time_out << std::endl;
        if(iter_time_out > time_out) {
            iter_time_out=0;
            std::cout << "TIME FINISHED :( " << std::endl;
            simon_sequence.reset();
            stateManager.changeState(INIT_STATE);
            for(int i=0; i<4; i++)
                pre_status[i] = false;
            continue;
        }

        usleep(100000);
    }
}


void *changeStateHandler(int stFrom, int stTo) {
    printf("********************** Cambio de estado: desde %d a %d.\n",stFrom,stTo);
    printf("Length: N=%d", simon_sequence.get_length());

    simon_leds_out.turn_all_off();
    simon_sequence.show();

    usleep(2000000);
    return(NULL);
}

void *success_game(int stFrom, int stTo) {
    simon_leds_out.turn_all_off();
    if(use_leds)
        simon_led_strip.success_game();
    sleep(3);
    if(use_leds)
        simon_led_strip.in_game();
}

void *fail_game(int stFrom, int stTo) {

    simon_leds_out.turn_all_off();
    if(use_leds)
        simon_led_strip.fail_game();
    sleep(3);
    if(use_leds)
        simon_led_strip.in_game();
}

void *starting_game(int stFrom, int stTo) {
    //Turn all leds to say that game starts
    simon_leds_out.turn_all_on();
    sleep(1);
    simon_leds_out.turn_all_on();
    std::cout << "INTRODUCE THE SEQUENCE!!" << std::endl;
}

void *dial_velocity(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    
    for(;;) {
        int state = stateManager.waitState(cfgPassed);
        int value = simon_dial_difficulty.get_value();
        std::cout << "Dificulty value: " << value << std::endl;
        simon_dial_difficulty.set_pot_position();
        usleep(1000);
    } 
}


int main() {

    std::cout << "Starting program" << std::endl;

    pthread_t th01, th02, th03, th04;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    stateManager.addStateChangeListener(0,1,changeStateHandler);
    stateManager.addStateChangeListener(1,2,starting_game);
    stateManager.addStateChangeListener(2,1,success_game);
    stateManager.addStateChangeListener(1,0,changeStateHandler);
    stateManager.addStateChangeListener(2,0,fail_game);

    ThreadConf h01Cfg;
    h01Cfg.addState(0);
    h01Cfg.setArg((void*)200);

    ThreadConf h02Cfg;
    h02Cfg.addState(1);
    h02Cfg.setArg((void*)250);

    ThreadConf h11Cfg;
    h11Cfg.addState(2);
    h11Cfg.setArg((void*)100);

    ThreadConf h12Cfg;
    h12Cfg.addState(0);
    h12Cfg.setArg((void*)100);

    pthread_create(&th01,&attr,init_thread,(void*)&h01Cfg);
    pthread_create(&th02,&attr,show_thread,(void*)&h02Cfg);
    pthread_create(&th03,&attr,introduce_thread,(void*)&h11Cfg);
    pthread_create(&th04,&attr,dial_velocity,(void*)&h12Cfg);

    int myState = 0;
    stateManager.changeState(INIT_STATE);
    if(use_leds)
        simon_led_strip.in_game();

    while(true);    

    pthread_join(th01,NULL);
    pthread_join(th02,NULL);
    pthread_join(th03,NULL);

    return 0;
}