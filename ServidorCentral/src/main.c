#include "main.h"


pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_t t0;
int contador = 0,keepThreading=1;
int main(){

    signal(SIGALRM, trataSinal);
    signal(SIGPIPE, trataInterrupcao);
    pthread_mutex_lock(&lock1);
    pthread_create(&t0, NULL, connectClient, NULL);

    ualarm(100000, 500000);
    while (1)
    {
        sleep(2);
    }

    return 0;
}


void * connectClient(){

    while(keepThreading){
        pthread_mutex_lock(&lock1);
        Cliente();
    }
    return NULL;

}

void trataSinal(int signal){
    
    if(contador==4){
        pthread_mutex_unlock(&lock1);
        contador=0;
    }
    contador++;
}

void trataInterrupcao(int sig)
{
    keepThreading=0;
    printf("%d\n",sig);
    ualarm(0,0);
    
    pthread_mutex_unlock(&lock1),
    pthread_mutex_destroy(&lock1);
    pthread_join(t0,NULL);
    
   
    trata_interrupcao_Cliente();

    exit(0);
}