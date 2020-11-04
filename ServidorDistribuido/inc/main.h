#ifndef _MAIN_H_
#define _MAIN_H_

#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>
#include <curses.h>
#include <stdlib.h>
#include <stdio.h>

#include "gpio.h"
#include "bme280.h"
#include "servidor.h"
#include "cliente.h"

void * gpioLampadasArCondicionado();
void * i2c_TemperaturaUmidade();
void *gpioSensores();
void trataSinalAlarme(int sinal);
void interruption(int sig);
void *sendUpdate();

#endif