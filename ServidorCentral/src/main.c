#include "main.h"


pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t t0,t1;
int contador = 0,keepThreading=1;
int main(){

    signal(SIGALRM, trataSinalAlarme);
    signal(SIGPIPE, trataInterrupcao);

    pthread_create(&t0, NULL, connectClient, NULL);
    pthread_create(&t1, NULL, connectServidor, NULL);

    alarm(1);
    while (1)
    {
        sleep(2);
    }

    return 0;
}


void * connectClient(){
    
    sleep(2);
    while(keepThreading){
        pthread_mutex_lock(&lock1);
        Cliente();
    }
    return NULL;

}

void * connectServidor(){
    while(keepThreading){
        pthread_mutex_lock(&lock2);
        //fprintf(stderr,"3\n");
        Servidor();

    }
    return NULL;
}

void trataSinalAlarme(int signal){
    
    if(contador==4){
        pthread_mutex_unlock(&lock1);
        pthread_mutex_unlock(&lock2);
        contador=0;
    }
    contador++;
}



void trataInterrupcao(int sig)
{
    keepThreading=0;
    if(sig ==13){
        printf("Ocorreu um erro com os sockets\n");
    }
    ualarm(0,0);
    
    pthread_mutex_unlock(&lock1),
    pthread_mutex_destroy(&lock1);
    pthread_mutex_unlock(&lock2),
    pthread_mutex_destroy(&lock2);
    pthread_join(t0,NULL);
    
   
    trata_interrupcao_Cliente();

    exit(0);
}