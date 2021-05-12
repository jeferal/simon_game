#ifndef SIMON_SEQUENCE_HPP_
#define SIMON_SEQUENCE_HPP_

#include "simon_interfaces/simon_leds.hpp"

#include <vector>

class SimonSequence {
    public:
        SimonSequence();

        void reset();

        int get_num_steps();

        void new_step();

        bool is_correct(int order);

    private:

        std::vector<SimonLeds::COLOR> *current_sequence;


};

#endif /* SIMON_SEQUENCE_HPP_ */
