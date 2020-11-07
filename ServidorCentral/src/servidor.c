#include "servidor.h"

int servidorSocket;
int socketCliente;



void TrataClienteTCP(struct atualizacao *  updateValues) {
	int tamanhoRecebido;
	int alarmPlaying = 0;
	int cont=0;
	do{
		if((tamanhoRecebido = recv(socketCliente,(void *) updateValues, sizeof(struct atualizacao), 0)) < 0){
			printError("Erro no recv()");
		}
		/*
		printf("%f %f\n",updateValues->temperatura,updateValues->umidade);
		for(int i=0;i<6;i++){
			printf("machines %d %d\n",updateValues->machines[i].port,updateValues->machines[i].state);
		}
		*/
		
		int i=0;
		for(i=0;i<8;i++){
			if(updateValues->sensors[i].state){
				if(!alarmPlaying){
					system("omxplayer --no-keys src/example.mp3 & > /dev/null 2>&1");	
					alarmPlaying=1;	
				}
					break;
			}
		}
		if(i==8 || cont==4){
			alarmPlaying=0;
			cont=0;
		}
		
		cont++;
	}while(tamanhoRecebido>0);
	

}

void Servidor(struct atualizacao *  updateValues) {
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;


    // Porta Servidor Distribuido
	servidorPorta = 10021;
	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printError("falha no socket do Servidor");
		close(servidorSocket);
		return;
	}

	// Montar a estrutura sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servidorAddr.sin_port = htons(servidorPorta);

	// Bind
	if(bind(servidorSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
		printError("Falha no Bind do Servidor Central");
		close(servidorSocket);
		return;
	}


	// Listen
	if(listen(servidorSocket, 10) < 0){
		printError("Falha no Listen\n");
		close(servidorSocket);		
		return;
	}
  
	while(1) {
		clienteLength = sizeof(clienteAddr);
		if((socketCliente = accept(servidorSocket,  (struct sockaddr *) &clienteAddr, &clienteLength)) < 0)
		{

			printError("Falha no Accept\n");
			continue;
	 	}
	
		setClientConection(inet_ntoa(clienteAddr.sin_addr));
		
		TrataClienteTCP(updateValues);
		close(socketCliente);

	}
	close(servidorSocket);
}

void trata_interrupcao_Servidor(){
	close(socketCliente);
	close(servidorSocket);
}