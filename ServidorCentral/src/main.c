#include "main.h"


pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t t0,t1;

int contador = 0,keepThreading=1,restartClient=1;
int main(){

    signal(SIGPIPE, trataErroSocket);

    pthread_create(&t1, NULL, connectServidor, NULL);

    while(keepThreading){
        Cliente();
        sleep(2);
    }


    return 0;
}


void * connectServidor(){
    while(keepThreading){
        Servidor();
        sleep(2);
    }
    return NULL;
}


void trataErroSocket(int signal){
    trata_interrupcao_Cliente();
    printf("1 - entrei no socket int\n");

    sleep(5);
    printf("2 - entrei no socket int\n");
    // espera 5 segundos e tenta reconectar o cliente
    Cliente();
    printf("3 - entrei no socket int\n");
    return;
}


void trataInterrupcao(int sig)
{
    keepThreading=0;
   
    trata_interrupcao_Cliente();

    exit(0);
}