#ifndef _SERVIDOR_H_
#define _SERVIDOR_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gpio.h"

void trata_interrupcao_Servidor();
void TrataClienteTCP();
void Servidor();
extern volatile double userDefinedTemp;
extern int tempControlledbyUser;

#endif