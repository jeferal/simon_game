#ifndef SIMON_SEQUENCE_HPP_
#define SIMON_SEQUENCE_HPP_

#include "simon_interfaces/simon_leds.hpp"

#include <vector>
#include <iostream>

class SimonSequence {
    public:
        SimonSequence();

        void reset();
        void reset_position();

        int get_num_steps();
        int get_length();

        void new_step(SimonLeds::COLOR color);

        SimonLeds::COLOR get_current_led();
        SimonLeds::COLOR step();

        bool is_correct(int order);

        bool is_finished();

        void show();
        void new_color();

    private:

        std::vector<SimonLeds::COLOR> *current_sequence;
        int position=0;
        int n=1;
};

#endif /* SIMON_SEQUENCE_HPP_ */
