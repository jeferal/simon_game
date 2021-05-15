#ifndef STATE_MONITOR_HPP_
#define STATE_MONITOR_HPP_

#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>

#include "thread_conf.hpp"

class StateMonitor {
    private:
        static const int MAXSTATES = MAXSTATES_DEF;
        static const int MAXLISTENERS = MAXLISTENERS_DEF;
        int internalState;
        int previousInternalState;
        pthread_mutex_t mutex;
        pthread_cond_t condition;
        void* (*func_table[MAXSTATES][MAXSTATES][MAXLISTENERS])(int stFrom, int stTo);
    
    public:
        StateMonitor();

        int waitState(ThreadConf *cfg);

        int changeState(int st);

        int getState();
        int getPreviousState();
        int addStateChangeListener(int fromState, int toState, void* (*handle)(int,int));

};

#endif //STATE_MONITOR_HPP_