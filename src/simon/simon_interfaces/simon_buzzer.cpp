#include "simon_buzzer.hpp"

SimonBuzzer::SimonBuzzer() {
    pwm_buzzer = new BBB::PWM(BBB::PWM::P8_19);
    pwm_buzzer->setPeriod(PERIOD_SUCCESS);
    pwm_buzzer->stop();
}

SimonBuzzer::SimonBuzzer(BBB::PWM::pinPWM pwm_id) {
    pwm_buzzer = new BBB::PWM(pwm_id);
    pwm_buzzer->setPeriod(PERIOD_SUCCESS);
    pwm_buzzer->stop();
}

SimonBuzzer::~SimonBuzzer() {
    pwm_buzzer->stop();
}

void SimonBuzzer::show_fail() {
    pwm_buzzer->setPeriod(PERIOD_FAIL);
    pwm_buzzer->setDutyCycle((unsigned int)PERIOD_FAIL/2);
    pwm_buzzer->run();
}

void SimonBuzzer::show_success() {
    pwm_buzzer->setPeriod(PERIOD_SUCCESS);
    pwm_buzzer->setDutyCycle((unsigned int)PERIOD_SUCCESS/2);

    pwm_buzzer->run();
}

void SimonBuzzer::show_starting_game() {
    pwm_buzzer->setPeriod(PERIOD_STARTING_GAME);
    pwm_buzzer->setDutyCycle((unsigned int)PERIOD_STARTING_GAME/2);

    pwm_buzzer->run();
}

void SimonBuzzer::show_start(unsigned int period) {
    pwm_buzzer->setPeriod(PERIOD_STARTING_GAME);
    pwm_buzzer->setDutyCycle((unsigned int)PERIOD_STARTING_GAME/2);

    pwm_buzzer->run();
}

void SimonBuzzer::show_stop() {
    pwm_buzzer->stop();
}