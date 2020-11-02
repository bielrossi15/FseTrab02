#include "main.h"
#include <bcm2835.h>
#include <errno.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
int contador = 0, temperatura, humidade;
pthread_t t0, t1, t2;
volatile float temperature=0,humidity = 0;
int keepThreading = 1;





int main()
{
    signal(SIGALRM, trataSinalAlarme);
    signal(SIGINT, interruption);
    while(init_bcm());
    while(initI2C());

    pthread_mutex_lock(&lock1);
    pthread_mutex_lock(&lock2);
    pthread_mutex_lock(&lock3);

    pthread_create(&t0, NULL, gpioLampadasArCondicionado, NULL);
    pthread_create(&t1, NULL, gpioSensores, NULL);
    pthread_create(&t2, NULL, i2c_TemperaturaUmidade, NULL);
    
    ualarm(100000, 100000);
    while (1)
    {
        sleep(2);
    }

    return 0;
}



void trataSinalAlarme(int sinal)
{
    pthread_mutex_unlock(&lock1);
    if(contador==10){
        pthread_mutex_unlock(&lock2);
        pthread_mutex_unlock(&lock3);
        contador=0;
    }
    contador++;
}

void interruption(int sig)
{
    ualarm(0,0);
    keepThreading=0;
    pthread_cancel(t0);
    pthread_cancel(t1);
    pthread_cancel(t2);
    pthread_mutex_unlock(&lock1),
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock3);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    
    pthread_join(t2,NULL);
    pthread_join(t0,NULL);
    pthread_join(t1,NULL);
    
    //fprintf(stderr,"Passei das threads\n");
    
    trata_interrupcao_gpio();
    trata_interrupcao_I2C();
    trata_interrupcao_Servidor();

    exit(0);
}







void *gpioLampadasArCondicionado(void *arg)
{
    while(keepThreading)
    {
        pthread_mutex_lock(&lock3);
        Servidor();
    }
    return NULL;
}

void *gpioSensores(void *arg)
{
    while (keepThreading)
    {
        pthread_mutex_lock(&lock1);
        gpioSensoresPresenca();
    }
    return NULL;
}


void * i2c_TemperaturaUmidade(){
    //get external temperature
    while(keepThreading){
        pthread_mutex_lock(&lock2);
        float a = temperature_humidity(1);
        if(a>0){temperature = a;}
        printf("Temperatura = %f ",temperature);
        a = temperature_humidity(0);
        if(a>0){humidity = a;}
        printf("Umidade = %f\n",humidity);
    }
    return NULL;
}



