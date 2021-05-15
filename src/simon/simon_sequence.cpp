#include "simon_sequence.hpp"

#define MAX_SEQUENCE 100

SimonSequence::SimonSequence() {
    current_sequence = new std::vector<SimonLeds::COLOR>(MAX_SEQUENCE);
}

void SimonSequence::reset() {
    position = 0;
    n=1;
}

void SimonSequence::reset_position() {
    position = 0;
    n=1;
}

void SimonSequence::new_step(SimonLeds::COLOR color) {
    current_sequence->at(position) = color;
    position++;
}

SimonLeds::COLOR SimonSequence::step() {

    SimonLeds::COLOR color = current_sequence->at(position);
    position++;

    return color;
}

int SimonSequence::get_num_steps() {
    return position;
}

int SimonSequence::get_length() {
    return n;
}

SimonLeds::COLOR SimonSequence::get_current_led() {
    return current_sequence->at(position);
}

bool SimonSequence::is_finished() {
    bool is_finished = n==position;
    if(is_finished) {
        position=0;
    }
    return is_finished;
}

void SimonSequence::new_color() {
    n++;
}

void SimonSequence::show() {
    std::cout << "Sequence: ";
    for(int i=0; i<this->get_length(); i++) {
        std::cout << current_sequence->at(i) << ", ";
    }
    std::cout << std::endl;
}