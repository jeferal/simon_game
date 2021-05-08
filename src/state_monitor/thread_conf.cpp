#include "thread_conf.hpp"


ThreadConf::ThreadConf() {
    nstates = 0;
    arg = NULL;
    states = new std::vector<int>(MAXSTATES_DEF,0);
}

void ThreadConf::addState(int s) {
    states->at(nstates) = s;
    nstates++;
}

bool ThreadConf::containsState(int s) {

    for(int i=0; i<nstates; i++) {
        if(states->at(i) == s)
        //Found a match
        return true;
    }
    //Nothing found
    return false;
}

int ThreadConf::getNStates() {
    return nstates;
}
void ThreadConf::setArg(void *pointer) {
    arg = pointer;
}
void *ThreadConf::getArg() {
    return arg;
}