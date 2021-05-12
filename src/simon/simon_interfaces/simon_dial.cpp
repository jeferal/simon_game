#include "simon_dial.hpp"

SimonDial::SimonDial() {
    analog_id = DEFAULT_ANALOG_ID;
    
    pwm_servo = new BBB::PWM(BBB::PWM::P9_22);
    pwm_servo->setPeriod(period);
    pwm_servo->run();

    m = (sval_max-sval_min)/v_pot_max;
}

SimonDial::SimonDial(int analog_id, BBB::PWM pwm_id) {
    this->analog_id = analog_id;

    pwm_servo = new BBB::PWM(pwm_id);
    pwm_servo->setPeriod(period);
    pwm_servo->run();

    m = (sval_max-sval_min)/v_pot_max;
}

int SimonDial::get_value() {
    return BBB::readAnalog(analog_id);
}

void SimonDial::set_position(unsigned duty) {
    pwm_servo->setDutyCycle(duty);
}

void SimonDial::set_pot_position() {
    int value = get_value();
    int duty = calculate_duty(value);
    set_position(duty);
}

int SimonDial::calculate_duty(int value) {
    int duty = m*value + sval_min;
    return duty;
}