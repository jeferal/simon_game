#include "../../analog_bbb/analog_input.hpp"
#include "../../analog_bbb/PWMuniv.hpp"

#define PERIOD_FAIL 2272727
#define PERIOD_SUCCESS 2500000


class SimonBuzzer {
    public:
        SimonBuzzer();
        SimonBuzzer(BBB::PWM::pinPWM pwm_id);
        ~SimonBuzzer();

        void show_fail(int time);
        void show_success(int time);

    private:

        BBB::PWM *pwm_buzzer;
};