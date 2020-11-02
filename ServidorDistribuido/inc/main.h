#ifndef _main_H_
#define _main_H_

#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <ncurses.h>
#include <curses.h>
#include <stdlib.h>

#include "gpio.h"
#include "bme280.h"

void * gpioLampadasArCondicionado();
void * i2c_TemperaturaHumidade();
void *gpioSensores();
void trataSinal(int sinal);
void interruption(int sig);

#endif