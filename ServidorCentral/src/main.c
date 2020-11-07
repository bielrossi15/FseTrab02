#include "main.h"


pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t t0,t1,t2,t3;

int contador = 0,keepThreading=1,restartClient=0;
struct atualizacao updateValues;

int main(){

    signal(SIGPIPE, trataErroSocket);
    signal(SIGINT, trataInterrupcao);

    initNcurs();
    pthread_create(&t0, NULL, connectServidor, NULL);
    pthread_create(&t1, NULL, ImprimeDados, &updateValues);
    pthread_create(&t2, NULL, EntradaUsuario, NULL);
    pthread_create(&t3, NULL, Erros, NULL);


    while(keepThreading){
        Servidor(&updateValues);
        sleep(2);
    }


    return 0;
}


void * connectServidor(){
    int cont=0;
    while(keepThreading){
        if(restartClient || cont==0){
            restartClient = Cliente();
            cont=1;
        }
        sleep(2);
    }
   
    return NULL;
}


void trataErroSocket(int signal){
    trata_interrupcao_Cliente();
    sleep(5);
    // espera 5 segundos e tenta reconectar o cliente
    restartClient=1;
    printf("3 - entrei no socket int\n");
    return;
}


void trataInterrupcao(int sig)
{
    keepThreading=0;
   
    trata_interrupcao_Cliente();
    clear();
    endwin();
    exit(0);
}