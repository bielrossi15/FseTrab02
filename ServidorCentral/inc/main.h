#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <signal.h>

#include "cliente.h"
#include "servidor.h"
#include "myNcurses.h"

void * connectClient();
void * connectServidor();
void trataSinalAlarme();
void trataInterrupcao(int sig);
void trataErroSocket(int signal);

#endif