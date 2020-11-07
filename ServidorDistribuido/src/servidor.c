#include "servidor.h"

int servidorSocket;
int socketCliente;
int tempControlledbyUser=0;

volatile double userDefinedTemp;

void TrataClienteTCP() {
	char buffer[16];
	int tamanhoRecebido;
	int receiveTemp=0;
	
	do{
		if(receiveTemp){
			double temp=153.4;
			if((tamanhoRecebido = recv(socketCliente,(void*)&temp, sizeof(double), 0)) < 0){
				printf("Erro no recv()\n");
				return;
			}
			userDefinedTemp = temp;
			receiveTemp=0;	
		}
		else{
			if((tamanhoRecebido = recv(socketCliente, buffer, 16, 0)) < 0){
				printf("Erro no recv()\n");
				return;
			}
		
			buffer[tamanhoRecebido] = '\0';
			if(buffer[0]=='0' ||buffer[0]=='1'||buffer[0]=='2'|| buffer[0]=='3'){
				gpioLigaEquipamentos(atoi(buffer));
			}
			else if( buffer[0]=='5' || buffer[0]=='6' || buffer[0]=='4'){
				tempControlledbyUser=0;
				gpioLigaEquipamentos(atoi(buffer));
			}
			else if(buffer[0]=='7'){
				receiveTemp=1;
				tempControlledbyUser=1;
			}

		}
		

    } while (tamanhoRecebido > 0);
}

void Servidor() {
	
	
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;


    // Porta Servidor Distribuido
	servidorPorta = 10121;

	// Abrir Socket
	if((servidorSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("falha no socket do Servidor\n");
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
		printf("Falha no Bind do Servidor Distribuido\n");
		close(servidorSocket);
		return;
	}

	// Listen
	if(listen(servidorSocket, 10) < 0){
		printf("Falha no Listen\n");
		close(servidorSocket);		
		return;
	}

    
	while(1) {
		clienteLength = sizeof(clienteAddr);
		if((socketCliente = accept(servidorSocket,  (struct sockaddr *) &clienteAddr, &clienteLength)) < 0)
		{

			printf("Falha no Accept\n");
			continue;
	 	}
		
		printf("Conexão do Cliente %s\n", inet_ntoa(clienteAddr.sin_addr));
		
		TrataClienteTCP();
		close(socketCliente);

	}
	close(servidorSocket);
}

void trata_interrupcao_Servidor(){
	close(socketCliente);
	close(servidorSocket);
}