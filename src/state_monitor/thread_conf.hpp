#ifndef THREAD_CONF_HPP
#define THREAD_CONF_HPP

#include <stdio.h>
#include <vector>
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <sys/types.h>

#define MAXSTATES_DEF 10
#define MAXLISTENERS_DEF 10

class ThreadConf {
  private:
   std::vector<int> *states;
   int nstates;
   void *arg;

  public:
   ThreadConf();

   void addState(int s);

   bool containsState(int s);

   int getNStates();
   
   void setArg(void *pointer);

   void *getArg();

};

#endif /* THREAD_CONF_HPP */
