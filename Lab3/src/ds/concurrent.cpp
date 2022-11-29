#include "ds/concurrent.hpp"
void *concurrentLegalization(void *arg){
   Abacus *abacus = (Abacus*)arg; // get data
   abacus->main();
   
   pthread_exit(NULL);
}