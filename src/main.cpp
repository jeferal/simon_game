#include <iostream>
#include <unistd.h>
#include <vector>
#include <pthread.h>
#include <getopt.h>

#include "simon/simon_interfaces/simon_leds.hpp"
#include "simon/simon_interfaces/simon_buttons.hpp"
#include "simon/simon_interfaces/simon_dial.hpp"
#include "simon/simon_interfaces/simon_matrix.hpp"
#include "simon/simon_interfaces/simon_buzzer.hpp"

#include "simon/simon_led_strip.hpp"
#include "simon/simon_sequence.hpp"

#include "state_monitor/state_monitor.hpp"
#include "state_monitor/thread_conf.hpp"

#define INIT_STATE 0
#define SHOW_STATE 1
#define INTRODUCE_STATE 2
#define PAUSE_STATE 3

#define MAX_SEQUENCE 100


StateMonitor stateManager;
SimonLeds simon_leds_out;
SimonButtons simon_buttons_in;
SimonLedStrip simon_led_strip("192.168.1.117",80);
SimonDial simon_dial_difficulty(0,BBB::PWM::P9_22);
SimonDial simon_dial_velocity(1,BBB::PWM::P9_21);
SimonSequence simon_sequence;
SimonMatrix simon_matrix(2,112);
SimonBuzzer simon_buzzer(BBB::PWM::P8_19);

int vel_show = 1000000;
int time_out = 50;
int iter_time_out = 0;
bool use_leds=false;
bool pause_pre_st = false;
bool rising_edge_init = false;
bool rising_edge_pause = false;

int init_event_handler(int arg) {
    printf("INIT BUTTON PRESSED!!!\n");
    rising_edge_init = true;
    return 0;
}

int pause_event_handler(int arg) {
    printf("PAUSE BUTTON PRESSED!!!\n");
    rising_edge_pause = true;
    return 0;
}

void *init_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    int iter=0;

    bool pre_status = false;

    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        //printf("INIT STATE\n");

        bool button_status = simon_buttons_in.read_button(SimonButtons::COLOR::INIT);

        if(button_status == true && pre_status == false) {
            printf("INIT BUTTON PRESSED!!!\n");
            pre_status = false;
            stateManager.changeState(SHOW_STATE);
        }

        pre_status = button_status;
        usleep(100000);
    }
}

void *show_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();

    bool pause_pre=false;

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

        if(simon_buttons_in.get_set_interruption(rising_edge_init)) {
            printf("INIT BUTTON PRESSED!!!\n");
            simon_sequence.reset();
            stateManager.changeState(INIT_STATE);
            continue;
        }

        //Read pause button
        bool pause_status = simon_buttons_in.read_button(SimonButtons::COLOR::PAUSE);
        if(simon_buttons_in.get_set_interruption(rising_edge_pause)) {
            stateManager.changeState(PAUSE_STATE);
            pause_pre = false;
            continue;
        }

    }
}

void *introduce_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();

    std::vector<bool> pre_status(4);

    pre_status[0] = false;
    pre_status[1] = false;
    pre_status[2] = false;
    pre_status[3] = false;

    bool pause_pre=false;

    for (;;) {
        int state = stateManager.waitState(cfgPassed);

        //Read all buttons
        std::vector<bool> status = simon_buttons_in.read_status(true);
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
                        for(int i=0; i<4; i++)
                            pre_status[i] = false;

                        stateManager.changeState(SHOW_STATE);
                        continue;
                    }
                }
            }

            else {
                if(rising_edges[i]==true) {
                    printf("WRONG BUTTON!!\n");
                    stateManager.changeState(INIT_STATE);

                    for(int i=0; i<4; i++)
                            pre_status[i] = false;
                    continue;
                }
            }
        }

        if(simon_buttons_in.get_set_interruption(rising_edge_init)) {
            printf("INIT BUTTON PRESSED!!!\n");
            stateManager.changeState(INIT_STATE);
            for(int i=0; i<4; i++)
                pre_status[i] = false;
            continue;
        }

        iter_time_out++;
        std::cout << iter_time_out << std::endl;
        if(iter_time_out > time_out) {
            std::cout << "TIME FINISHED :( " << std::endl;
            stateManager.changeState(INIT_STATE);
            for(int i=0; i<4; i++)
                pre_status[i] = false;
            continue;
        }

        //Read pause button
        //bool pause_status = simon_buttons_in.read_button(SimonButtons::COLOR::PAUSE);
        if(simon_buttons_in.get_set_interruption(rising_edge_pause)) {
            stateManager.changeState(PAUSE_STATE);
            pause_pre_st = false;
            continue;
        }
        //pause_pre_st = pause_status;
        usleep(100000);
    }
}

void *pause_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();

    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        std::cout << "PAUSE THREAD" << std::endl;

        //Read pause button
        if(simon_buttons_in.get_set_interruption(rising_edge_pause)) {
            //Go to the previous state
            std::cout << "Previous State: " << stateManager.getPreviousState() << std::endl;
            stateManager.changeState(stateManager.getPreviousState());
            continue;
        }

        //Check also init button
        if(simon_buttons_in.get_set_interruption(rising_edge_init)) {
            //Go to init state
            simon_sequence.reset();
            //Reset time out
            iter_time_out = 0;
            stateManager.changeState(INIT_STATE);
            continue;
        }
        usleep(100000);
    }
}

void *score_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();

    for (;;) {
        int state = stateManager.waitState(cfgPassed);
        //std::cout << "division time: "<< (iter_time_out*8)/time_out << std::endl;
        simon_matrix.display_score_time(simon_sequence.get_length(), (int) (iter_time_out*8)/time_out);

        usleep(100000);
    }
}

void *dial_velocity_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    
    for(;;) {
        int state = stateManager.waitState(cfgPassed);
        int value = simon_dial_difficulty.get_value();
        
        vel_show = 1172.16*value + 1000000/5;
        std::cout << "Dificulty value (show velocity): " << vel_show << std::endl;
        simon_dial_difficulty.set_pot_position();
        usleep(100000);
    } 
}

void *dial_difficulty_thread(void *param) {
    ThreadConf *cfgPassed = (ThreadConf*)param;
    long longPassed = (long) cfgPassed->getArg();
    
    for(;;) {
        int state = stateManager.waitState(cfgPassed);
        int value = simon_dial_velocity.get_value();
        
        time_out = 0.0225*value + 10;
        std::cout << "Time out value: " << value << std::endl;
        simon_dial_velocity.set_pot_position();
        usleep(100000);
    } 
}

void *success_game(int stFrom, int stTo) {
    //n++
    simon_sequence.new_color();
    //Reset time out
    iter_time_out=0;

    simon_buzzer.show_start(PERIOD_SUCCESS);
    simon_leds_out.turn_all_off();

    if(use_leds)
        simon_led_strip.success_game();
    sleep(3);
    if(use_leds)
        simon_led_strip.in_game();
    simon_buzzer.show_stop();
}

void *fail_game(int stFrom, int stTo) {
    //Reset time out
    iter_time_out=0;
    //Reset sequence
    simon_sequence.reset();


    simon_buzzer.show_start(PERIOD_FAIL);
    simon_leds_out.turn_all_off();
    if(use_leds)
        simon_led_strip.fail_game();
    sleep(3);
    if(use_leds)
        simon_led_strip.in_game();
    simon_buzzer.show_stop();
}

void *starting_game(int stFrom, int stTo) {
    //Turn all leds to say that game starts
    simon_buzzer.show_starting_game(1000000);
    std::cout << "INTRODUCE THE SEQUENCE!!" << std::endl;
}

void *pause_game(int stFrom, int stTo) {
    sleep(1);
    //Reset value pause
    pause_pre_st=true;

}

void *init_game(int stFrom, int stTo) {

    rising_edge_init = false;
    rising_edge_pause = false;

    simon_leds_out.turn_all_on();
    sleep(2);
    simon_leds_out.turn_all_off();
    sleep(1);
}

int main(int argc, char *argv[]) {

    use_leds = false;

    std::cout << "Starting program" << std::endl;

    pthread_t th01, th02, th03, th04, th05, th06, th07;
    pthread_attr_t attr;

    pthread_attr_init(&attr);

    stateManager.addStateChangeListener(SHOW_STATE,INTRODUCE_STATE,starting_game);
    stateManager.addStateChangeListener(INTRODUCE_STATE,SHOW_STATE,success_game);
    stateManager.addStateChangeListener(INTRODUCE_STATE,INIT_STATE,fail_game);
    stateManager.addStateChangeListener(INIT_STATE, SHOW_STATE, *init_game);
    stateManager.addStateChangeListener(SHOW_STATE, PAUSE_STATE, *pause_game);
    stateManager.addStateChangeListener(INTRODUCE_STATE, PAUSE_STATE, *pause_game);

    ThreadConf h01Cfg;
    h01Cfg.addState(INIT_STATE);
    h01Cfg.setArg((void*)200);

    ThreadConf h02Cfg;
    h02Cfg.addState(SHOW_STATE);
    h02Cfg.setArg((void*)250);

    ThreadConf h11Cfg;
    h11Cfg.addState(INTRODUCE_STATE);
    h11Cfg.setArg((void*)100);

    ThreadConf h12Cfg;
    h12Cfg.addState(INIT_STATE);
    h12Cfg.setArg((void*)100);

    ThreadConf h13Cfg;
    h13Cfg.addState(PAUSE_STATE);
    h13Cfg.setArg((void*)100);

    ThreadConf h14Cfg;
    h14Cfg.addState(INIT_STATE); h14Cfg.addState(INTRODUCE_STATE); h14Cfg.addState(SHOW_STATE);
    h14Cfg.addState(PAUSE_STATE);
    h14Cfg.setArg((void*)100);

    ThreadConf h16Cfg;
    h16Cfg.addState(INIT_STATE);
    h16Cfg.setArg((void*)100);

    pthread_create(&th01,&attr,init_thread,(void*)&h01Cfg);
    pthread_create(&th02,&attr,show_thread,(void*)&h02Cfg);
    pthread_create(&th03,&attr,introduce_thread,(void*)&h11Cfg);
    pthread_create(&th04,&attr,dial_velocity_thread,(void*)&h12Cfg);
    pthread_create(&th05,&attr,pause_thread,(void*)&h13Cfg);
    pthread_create(&th06,&attr,score_thread,(void*)&h14Cfg);
    pthread_create(&th07,&attr,dial_difficulty_thread,(void*)&h16Cfg);

    int myState = 0;

    simon_buttons_in.set_init_interruption(init_event_handler);
    simon_buttons_in.set_pause_interruption(pause_event_handler);

    stateManager.changeState(INIT_STATE);
    if(use_leds)
        simon_led_strip.in_game();

    while(true);    

    pthread_join(th01,NULL);
    pthread_join(th02,NULL);
    pthread_join(th03,NULL);

    return 0;
}