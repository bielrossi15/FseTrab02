#include "myNcurses.h"

WINDOW *windowImprimeDados,*windowEntradaUsuario;

void initNcurs()
{
    initscr();
    //noecho();
    cbreak();
    keypad(stdscr, TRUE);
    curs_set(0);

    createImprimeDadosWindow();
    createErrosWindow();
    createEntradaUsuarioWindow();
    

}

void  createEntradaUsuarioWindow(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int tamXwindow = xMax/10 * 8, posX = 2;
    int tamanhoYwindowErro = 3;
    int tamanhoYwindowImprimeDados = yMax / 10 * 6;
    int tamanhoYwindowEntrada = 6;
    int posY =  tamanhoYwindowErro + tamanhoYwindowImprimeDados +2;
  

    windowEntradaUsuario = newwin(tamanhoYwindowEntrada, tamXwindow, posY, posX);
    mvwprintw(windowEntradaUsuario, 1, 1, "Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
    box(windowEntradaUsuario, 0, 0);
    wrefresh(windowEntradaUsuario);
}

void createImprimeDadosWindow(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int tamXwindow = xMax/10 * 8, posX = 2;
    int tamanhoYwindow = yMax/10 * 6;

    windowImprimeDados = newwin(tamanhoYwindow, tamXwindow, 1, posX);
    
    box(windowImprimeDados, 0, 0);
    wrefresh(windowImprimeDados);
}

void createErrosWindow(){
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    int tamXwindow = xMax/10 * 8, posX = 2;
    int tamanhoYwindowErro = 3 ;
    int posY = yMax / 10 * 6 + 2;
   

    WINDOW *windowImprimeErros = newwin(tamanhoYwindowErro, tamXwindow, posY, posX);
    box(windowImprimeErros, 0, 0);
    mvwprintw(windowImprimeErros, 1, 1, "Erros:");
    wrefresh(windowImprimeErros);
}


void * EntradaUsuario(void* parameters){

   
    mvwprintw(windowEntradaUsuario, 1, 1, "Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
    mvwprintw(windowEntradaUsuario, 4, 1, "Comando:");
    box(windowEntradaUsuario, 0, 0);
    wrefresh(windowEntradaUsuario);
    
    int validation;
    while(mvwscanw(windowEntradaUsuario, 4, 9, "%d",&validation)){
        mvwprintw(windowEntradaUsuario, 4, 9, "%*c",20,' ');
        box(windowEntradaUsuario, 0, 0);
        wrefresh(windowEntradaUsuario);
        
        while(validation<0|| validation> 6){
            mvwprintw(windowEntradaUsuario, 1, 1, "Comando Incorreto. Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
            box(windowEntradaUsuario, 0, 0);
            wrefresh(windowEntradaUsuario);
            
            mvwscanw(windowEntradaUsuario, 4, 9, "%d",&validation);
            mvwprintw(windowEntradaUsuario, 4, 9, "%*c",20,' ');
            box(windowEntradaUsuario, 0, 0);
            wrefresh(windowEntradaUsuario);

        }
        
        sendCommand(validation);
        mvwprintw(windowEntradaUsuario, 1, 1, "Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
        box(windowEntradaUsuario, 0, 0);
        wrefresh(windowEntradaUsuario);
    }



    return NULL;
}

void * ImprimeDados(void* parameters){

    char sensorsName [10][100] = {"SENSOR_PRESENCA_COZINHA",
                  "SENSOR_PRESENCA_SALA",
                  "SENSOR_ABERTURA_PORTA_COZINHA",
                  "SENSOR_ABERTURA_JANELA_COZINHA",
                  "SENSOR_ABERTURA_PORTA_SALA",
                  "SENSOR_ABERTURA_JANELA_SALA",
                  "SENSOR_ABERTURA_JANELA_QUARTO_01",
                  "SENSOR_ABERTURA_JANELA_QUARTO_02"};

    struct atualizacao *updateValues = (struct atualizacao *)parameters;
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    int k;

    while(1){
        wclear(windowImprimeDados);
        mvwprintw(windowImprimeDados, 1, xMax/4, "Temperatura = %f     Umidade = %f",updateValues->temperatura,updateValues->umidade);
        for(int j=0;j<4;j++){
            k = 2 * j;
            mvwprintw(windowImprimeDados, k+3, xMax/10, "Estado Lâmpada %d = %d %*c %s = %d", j+1 , updateValues->machines[j].state,16,' ',sensorsName[j], updateValues->sensors[j].state);
        }
        for (int j = 4; j < 6; j++)
        {
            k = 2 * j;
            mvwprintw(windowImprimeDados, k + 3, xMax/10, "Estado Arcondicionado %d = %d %*c %s = % d ",j-4, updateValues->machines[j].state,10,' ',sensorsName[j], updateValues->sensors[j].state);
        }
        for(int j=6;j<8;j++){
            k = 2 * j;
            mvwprintw(windowImprimeDados, k + 3, xMax/10+39, "%s = % d ", sensorsName[j], updateValues->sensors[j].state);
        }

        box(windowImprimeDados, 0, 0);
        wrefresh(windowImprimeDados);
        sleep(1);


    }

    return NULL;
}

void * Erros(void* parameters){

    return NULL;
}
