#include "simon_sequence.hpp"

#define MAX_SEQUENCE 100

SimonSequence::SimonSequence() {
    current_sequence = new std::vector<SimonLeds::COLOR>(MAX_SEQUENCE);
}