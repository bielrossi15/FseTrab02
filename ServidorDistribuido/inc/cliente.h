#ifndef _CLIENTE_H_
#define _CLIENTE_H_

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct device{
    int port;
    int state;
};

struct atualizacao{
    float temperatura,umidade;
	struct device machines[10];
	struct device sensors[10];
};


int init_Cliente();
void trata_interrupcao_Cliente();
int send_TCP_message(struct atualizacao * updates);

#endif