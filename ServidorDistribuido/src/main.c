#include "main.h"
#include <bcm2835.h>
#include <errno.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock5 = PTHREAD_MUTEX_INITIALIZER;
pthread_t t0, t1, t2,t3,t4;

int contador = 0,contador2=0 ,contador3=0;
volatile int restartClient=1;
float * tempHumidity;
int keepThreading = 1;
struct atualizacao *update;



int main()
{
    update=  malloc(sizeof(struct atualizacao));
    tempHumidity=  malloc(sizeof(float)*5);
    signal(SIGPIPE, trataErroSocket);
    signal(SIGALRM, trataSinalAlarme);
    signal(SIGINT, interruption);
    while(init_bcm());
    while(initI2C());
  

    pthread_mutex_lock(&lock5);
   

    pthread_create(&t1, NULL, gpioSensores, NULL);
    pthread_create(&t2, NULL, i2c_TemperaturaUmidade, NULL);
    pthread_create(&t3, NULL, sendUpdate, NULL);
    pthread_create(&t4, NULL, connectClient, NULL);
    
    ualarm(100000, 100000);
   
    while(keepThreading)
    {
        Servidor();

        sleep(2);   
    }
  

    return 0;
}



void trataSinalAlarme(int sinal)
{
    pthread_mutex_unlock(&lock1);
    if(contador==8){
        pthread_mutex_unlock(&lock2); 
        contador=0;
    }

    if(contador2==10){
        pthread_mutex_unlock(&lock5);
        contador2=0;
    }

    if(contador3==20){
        pthread_mutex_unlock(&lock4);  
        contador3=0;  
    }
    
    contador++;
    contador2++;
    contador3++;
}

void interruption(int sig)
{
    fprintf(stderr,"%d\n",sig);
    ualarm(0,0);
    keepThreading=0;
    
    pthread_cancel(t1);
    pthread_cancel(t2);
    pthread_cancel(t3);
    pthread_cancel(t4);
    pthread_mutex_unlock(&lock1),
    pthread_mutex_unlock(&lock2);
    //pthread_mutex_unlock(&lock3);
    pthread_mutex_unlock(&lock4);
    pthread_mutex_unlock(&lock5);
    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);
    pthread_mutex_destroy(&lock3);
    pthread_mutex_destroy(&lock4);
    pthread_mutex_destroy(&lock5);
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    pthread_join(t4,NULL);
    
    //fprintf(stderr,"Passei das threads\n");
    trata_interrupcao_gpio();
    trata_interrupcao_I2C();
    trata_interrupcao_Servidor();
    free(update);
    free(tempHumidity);

    exit(0);
}

void trataErroSocket(int signal){
    restartClient=1; 
}


void * connectClient(){

    while(keepThreading)
    {
        pthread_mutex_lock(&lock4);
        if(restartClient){
            restartClient = init_Cliente();
        }

        
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
    float *temp = malloc(sizeof(float)*5);
    while(keepThreading){
        pthread_mutex_lock(&lock2);
        temp = temperature_humidity();
        if(temp[0]>0){
            tempHumidity[0] = temp[0]; 
        }
        if(temp[1]>0){
            tempHumidity[1] = temp[1]; 
        }

    }
    return NULL;
}


void *sendUpdate(){

    while(keepThreading){

        pthread_mutex_lock(&lock5);
        if(!restartClient){

            update = updated_Values();
            /*
            for (int i = 0; i < 6; i++)
            {
                if(update->machines[i].state){
                    printf("Machine Port = %d Machine State = %d\n", update->machines[i].port, update->machines[i].state);  
                }
            
            }
            

            for (int i = 0; i < 8; i++)
            {
                if(update->sensors[i].state){
                    printf("Sensor Port = %d Sensor State = %d\n", update->sensors[i].port, update->sensors[i].state);  
                }
            }
            

            printf("Umidade2 = %f Temperatura2 = %f\n", update->umidade,update->temperatura);
            */
            update->temperatura = tempHumidity[0];
            update->umidade = tempHumidity[1];
            restartClient = send_TCP_message(update);
        
        }
    }
    
   return NULL;
}
