#include "../../analog_bbb/analog_input.hpp"
#include "../../analog_bbb/PWMuniv.hpp"

#define PERIOD_FAIL 4000000
#define PERIOD_SUCCESS 2272727
#define PERIOD_STARTING_GAME 900000


class SimonBuzzer {
    public:
        SimonBuzzer();
        SimonBuzzer(BBB::PWM::pinPWM pwm_id);
        ~SimonBuzzer();

        void show_fail();
        void show_success();
        void show_starting_game();

        void show_stop();

    private:

        BBB::PWM *pwm_buzzer;
};