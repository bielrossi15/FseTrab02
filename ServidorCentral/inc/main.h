#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <signal.h>

#include "cliente.h"

void * connectClient();
void trataSinal();
void trataInterrupcao(int sig);
#endif