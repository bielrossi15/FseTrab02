#include "gpio.h"
#include <bcm2835.h>


struct device{
    int port;
    int state;
};


struct device sensors[10] = {
    {SENSOR_PRESENCA_COZINHA,LOW}, 
    {SENSOR_PRESENCA_SALA,LOW}, 
    {SENSOR_ABERTURA_PORTA_COZINHA,LOW},
    {SENSOR_ABERTURA_JANELA_COZINHA,LOW}, 
    {SENSOR_ABERTURA_PORTA_SALA,LOW}, 
    {SENSOR_ABERTURA_JANELA_SALA,LOW},
    {SENSOR_ABERTURA_JANELA_QUARTO_01,LOW}, 
    {SENSOR_ABERTURA_JANELA_QUARTO_02,LOW}};

struct device machines[] = {
    {LAMPADA_01,LOW},
    {LAMPADA_02,LOW},
    {LAMPADA_03,LOW},
    {LAMPADA_04,LOW},
    {ARCONDICIONADO_01,LOW},
    {ARCONDICIONADO_02,LOW}};

int sensorsSize = 8,machinesSize=6;

int gpioLigaEquipamentos(int option)
{

    /* Situacao especial onde se deseja ativar os 2 ar-condicionados ao mesmo tempo.
     Observe que se um dos ar-condicionados estiver ligado e essa opcao for ativada
     um ar-condicionado sera ligado e o outro sera desligado.
    */
    if(option==7){
        bcm2835_gpio_write(machines[4].port, !machines[4].state);
        bcm2835_gpio_write(machines[5].port, !machines[5].state);
        machines[4].state = !machines[4].state;
        machines[5].state = !machines[5].state;
    }

    else{
        bcm2835_gpio_write(machines[option].port, !machines[option].state);
        machines[option].state = !machines[option].state;
    }

    return 1;
}

void gpioSensoresPresenca(){
    
    for(int i=0;i<sensorsSize;i++){
        sensors[i].state = bcm2835_gpio_lev(sensors[i].port);
        if(sensors[i].state){
            printf("Sensor na porta %d %d\n",sensors[i].port,sensors[i].state);
            //printf("Sensor na porta %d %d\n",sensors[i].port,sensors[i].state);
        }
    }
   
}

int init_bcm(){
    if (!bcm2835_init()){
        return 1;
    }
   // Configura pinos dos LEDs como saídas
    
    for(int i=0;i<machinesSize;i++){
        bcm2835_gpio_fsel(machines[i].port, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(machines[i].port, LOW);

    }

    for(int i=0;i<sensorsSize;i++){
        bcm2835_gpio_fsel(sensors[i].port, BCM2835_GPIO_FSEL_OUTP);
    }

    return 0;
}

void trata_interrupcao_gpio(void)
{
    for(int i=0;i<machinesSize;i++){
        bcm2835_gpio_write(machines[i].port, LOW);
    }
    
    
    fprintf(stderr,"fechei a gpio\n");
    bcm2835_close();
    
}

