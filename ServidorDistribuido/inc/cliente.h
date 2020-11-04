#ifndef _CLIENTE_H_
#define _CLIENTE_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

int init_Cliente();
void trata_interrupcao_Cliente();
void send_TCP_message();

#endif