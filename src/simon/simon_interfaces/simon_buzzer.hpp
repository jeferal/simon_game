#include "../../analog_bbb/analog_input.hpp"
#include "../../analog_bbb/PWMuniv.hpp"

#define PERIOD_FAIL 7000000
#define PERIOD_SUCCESS 2272727
#define PERIOD_STARTING_GAME 1000000


class SimonBuzzer {
    public:
        SimonBuzzer();
        SimonBuzzer(BBB::PWM::pinPWM pwm_id);
        ~SimonBuzzer();

        void show_fail(int time);
        void show_success(int time);
        void show_starting_game(int time);

        void show_start(unsigned int period);
        void show_stop();

    private:

        BBB::PWM *pwm_buzzer;
};