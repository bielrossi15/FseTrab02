#include "servidor.h"

int servidorSocket;
int socketCliente;

struct device{
	int port;
    int state;
};


struct atualizacao{
    float temperatura,humidade;
	struct device machines[10];
	struct device sensors[10];
};

void TrataClienteTCP() {
	int tamanhoRecebido;
	
	
	
	
	struct atualizacao resposta;
	do{
		if((tamanhoRecebido = recv(socketCliente,(void *) &resposta, sizeof(struct atualizacao), 0)) < 0){
			printf("Erro no recv()\n");
		}
		
		printf("%f %f\n",resposta.temperatura,resposta.humidade);
		for(int i=0;i<6;i++){
			printf("machines %d %d\n",resposta.machines[i].port,resposta.machines[i].state);
		}

		for(int i=0;i<8;i++){
			printf("sensors %d %d\n",resposta.sensors[i].port,resposta.sensors[i].state);
		}
		
	}while(tamanhoRecebido>0);
	

}

void Servidor() {
	fprintf(stderr,"1\n");
	struct sockaddr_in servidorAddr;
	struct sockaddr_in clienteAddr;
	unsigned short servidorPorta;
	unsigned int clienteLength;


    // Porta Servidor Distribuido
	servidorPorta = 10021;
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
		printf("Falha no Bind do Servidor Central\n");
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
		printf("Servidor\n");
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