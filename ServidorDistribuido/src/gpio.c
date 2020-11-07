#include "gpio.h"
#include <bcm2835.h>




struct device sensors[] = {
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


volatile int restartClient;
int sensorsSize = 8,machinesSize=6;

int gpioLigaEquipamentos(int option)
{
    /* Situacao especial onde se deseja ativar os 2 ar-condicionados ao mesmo tempo.
     Observe que se um dos ar-condicionados estiver ligado e essa opcao for ativada
     um ar-condicionado sera ligado e o outro sera desligado.
    */
    if(option==6){
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
       
    struct atualizacao * rapidUpdate = malloc(sizeof(struct atualizacao));
    rapidUpdate->temperatura = -1.0;
    int cont=0;
    for(int i=0;i<sensorsSize;i++){
    
        sensors[i].state = bcm2835_gpio_lev(sensors[i].port);
        if(sensors[i].state){
            cont++;
            rapidUpdate->sensors[i].state = sensors[i].state;
        }
       
       if(cont>0 && !restartClient){
           send_TCP_message(rapidUpdate);
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
        //bcm2835_gpio_write(sensors[i].port, LOW);
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

struct atualizacao *updated_Values(){

    struct atualizacao *update = malloc(sizeof(struct atualizacao));

    
    for (int i = 0; i < 6; i++)
    {
       update->machines[i].state = machines[i].state;
       update->machines[i].port = machines[i].port;

    }

    for (int i = 0; i < 8; i++)
    {
       update->sensors[i].state = sensors[i].state;
       update->sensors[i].port = sensors[i].port;

    }

    return update;

}
