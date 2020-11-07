#include "myNcurses.h"

WINDOW *windowImprimeDados,*windowEntradaUsuario,*windowImprimeErros;

char * connection;
double userDefinedTemp;

void initNcurs()
{
    fp=fopen("relatorio.csv","w");
    fprintf(fp,"Comando, Alarme Ativado, Horário\n");
    fclose(fp);
    fp=fopen("relatorio.csv","a");
    
    initscr();
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
    int tamanhoYwindowErro = 5;
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
    int tamanhoYwindowErro = 5 ;
    int posY = yMax / 10 * 6 + 2;
   

    windowImprimeErros = newwin(tamanhoYwindowErro, tamXwindow, posY, posX);
    mvwprintw(windowImprimeErros, 1, 1, "Erros: ");
    box(windowImprimeErros, 0, 0);
    wrefresh(windowImprimeErros);
}


void * EntradaUsuario(void* parameters){

   
    mvwprintw(windowEntradaUsuario, 1, 1, "Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
    mvwprintw(windowEntradaUsuario, 4, 1, "Comando:");
    clearThenBox(0);
    
    int validation;
    while(mvwscanw(windowEntradaUsuario, 4, 9, "%d",&validation)){
       clearThenBox(1);
        
        while(validation<0|| validation> 7){
            mvwprintw(windowEntradaUsuario, 1, 1, "Comando Incorreto. Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
            clearThenBox(0);
            
            mvwscanw(windowEntradaUsuario, 4, 9, "%d",&validation);
            clearThenBox(1);
            
        }
        
        double temp = -15.0;
        if(validation==7){
            sendCommand(validation,temp);
            mvwprintw(windowEntradaUsuario, 1, 1, "%*c",100,' ');
            mvwprintw(windowEntradaUsuario, 2, 1, "%*c",100,' ');
            mvwprintw(windowEntradaUsuario, 1, 1, "Digite a temperatura Correta");
            mvwprintw(windowEntradaUsuario, 4, 1, "Temperatura:");
            mvwprintw(windowEntradaUsuario, 4, 13, "%*c",20,' ');
            box(windowEntradaUsuario, 0, 0);
            wrefresh(windowEntradaUsuario);
            mvwscanw(windowEntradaUsuario, 4, 13, "%lf",&userDefinedTemp);
            temp = userDefinedTemp;
        }
        
        sendCommand(validation,temp);
        mvwprintw(windowEntradaUsuario, 1, 1, "Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas,\n 4 e 5 ar-condicionados e 6 caso deseje acionar os 2 ar-condicionados e 7 para definir a temperatura");
        mvwprintw(windowEntradaUsuario, 4, 1, "Comando:");
        clearThenBox(1);
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

    

    while(1){
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        
        wclear(windowImprimeDados);
        mvwprintw(windowImprimeDados, 1, 1, "Temperatura = %f     Umidade = %f      Temperatura definida pelo Usuário %lf",updateValues->temperatura,updateValues->umidade,userDefinedTemp);
        
        for(int j=0;j<4;j++){
           
            mvwprintw(windowImprimeDados, j+3, xMax/10, "Estado Lâmpada %d = %d %*c %s = %d", j+1 , updateValues->machines[j].state,16,' ',sensorsName[j], updateValues->sensors[j].state);
            if(updateValues->sensors[j].state){
                fprintf(fp,"nenhum, %s , %s", sensorsName[j], asctime(tm));
            }
        }
        for (int j = 4; j < 6; j++)
        {
           
            mvwprintw(windowImprimeDados, j + 3, xMax/10, "Estado Arcondicionado %d = %d %*c %s = % d ",j-4, updateValues->machines[j].state,10,' ',sensorsName[j], updateValues->sensors[j].state);
            if(updateValues->sensors[j].state){
                fprintf(fp,"nenhum, %s , %s", sensorsName[j], asctime(tm));
            }
        }
        for(int j=6;j<8;j++){
          
            mvwprintw(windowImprimeDados, j + 3, xMax/10+39, "%s = % d ", sensorsName[j], updateValues->sensors[j].state);
            if(updateValues->sensors[j].state){
                fprintf(fp,"nenhum, %s , %s", sensorsName[j], asctime(tm));
            }
        }

        printClientConection();

        box(windowImprimeDados, 0, 0);
        wrefresh(windowImprimeDados);
        sleep(1);


    }

    return NULL;
}

void * Erros(void* parameters){

    while(1){
        wclear(windowImprimeErros);
        mvwprintw(windowImprimeErros, 1, 1, "Erros:");
        box(windowImprimeErros, 0, 0);
        wrefresh(windowImprimeErros);
        sleep(8);
    }

    return NULL;
}

void clearThenBox(int option){
    if(option){
        mvwprintw(windowEntradaUsuario, 4, 9, "%*c",20,' ');
    }
    box(windowEntradaUsuario, 0, 0);
    wrefresh(windowEntradaUsuario);

}

void printError(char erro[500]){

    wprintw(windowImprimeErros, "%s\n ",erro);
	box(windowImprimeErros, 0, 0);
	wrefresh(windowImprimeErros);

}


void setClientConection(char msg[500]){
    connection = msg;
}

void printClientConection(){
    int yMax,xMax;
    getmaxyx(windowImprimeDados, yMax, xMax);
    mvwprintw(windowImprimeDados, yMax-2,1, "Conexão do Cliente %s\n",connection);
}

