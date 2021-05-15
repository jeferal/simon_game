#include "state_monitor.hpp"

StateMonitor::StateMonitor() {
    internalState = 0;
    previousInternalState = internalState;
    int i,j,k;
    //////////////////////////////////////////////////////////////////////////
    //// EJERCICIO 2: inicializar tanto el mutex "mutex" como la variable  
    ////              condicion "condition" 
    /////////////////////////////////////////////////////////////////////////

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condition, NULL);

    for(i=0;i<MAXSTATES;i++) 
        for(j=0;j<MAXSTATES;j++) 
            for(k=0;k<MAXLISTENERS;k++) {
                func_table[i][j][k] = NULL;
            }
}

int StateMonitor::waitState(ThreadConf *cfg) {
    int auxState;
    pthread_mutex_lock(&mutex); 
    //////////////////////////////////////////////////////////////////////////
    //// EJERCICIO 3: en la siguiente linea "while(false)" sustituir "false" por
    ////              la expresión boolena adecuada (observe que "cfg" contiene
    ////              el vector de estados en el que se debe ejecutar el hilo).
        //////////////////////////////////////////////////////////////////////////
        while (!cfg->containsState(internalState)) {
            pthread_cond_wait(&condition, &mutex);
        }
    auxState = internalState;
    pthread_mutex_unlock(&mutex); 
    return(auxState);
}

int StateMonitor::changeState(int st) {
    pthread_mutex_lock(&mutex); 
        int i;
        for(i=0;i<MAXLISTENERS;i++) {
            if (func_table[internalState][st][i] != NULL) {
                (*func_table[internalState][st][i])(internalState,st);
            }
        }
    //Store the previous state
    previousInternalState = internalState;
    internalState = st;
    pthread_cond_broadcast(&condition);
    pthread_mutex_unlock(&mutex); 
    return(0);
}

int StateMonitor::getState() {
    int aux;
    
    pthread_mutex_lock(&mutex); 
    aux = internalState;
    pthread_mutex_unlock(&mutex); 
    
    return(aux);
}

int StateMonitor::getPreviousState() {
    int aux;
    
    pthread_mutex_lock(&mutex); 
    aux = previousInternalState;
    pthread_mutex_unlock(&mutex); 
    
    return(aux);
}

int StateMonitor::addStateChangeListener(int fromState, int toState, void* (*handle)(int,int)) {
    pthread_mutex_lock(&mutex); 
        int i = 0; 
        int retval = 1;
        for(i=0;i<MAXLISTENERS;i++) {
            if (func_table[fromState][toState][i] == NULL) {
                retval = 0; 
                func_table[fromState][toState][i] = handle;
                break;
            }
        }
    pthread_mutex_unlock(&mutex); 
    return(retval);
}
