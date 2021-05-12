#include "../../analog_bbb/analog_input.hpp"
#include "../../analog_bbb/PWMuniv.hpp"

#include <pthread.h>

#define DEFAULT_ANALOG_ID 0

class SimonDial {
    public:
        SimonDial();
        SimonDial(int analog_id, BBB::PWM pwm_id);

        int get_value();
        void set_position(unsigned duty);
        void set_pot_position();
        int calculate_duty(int value);
        
    private:

        int analog_id;
        BBB::PWM *pwm_servo;
        int period=20000000;
        unsigned int sval_min = 500000;
	    unsigned int sval_max = 2500000;
	    unsigned int v_pot_max = 4095;
        float m;
};