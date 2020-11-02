#include "cliente.h"

int clienteSocket;
void Cliente() {
	
	struct sockaddr_in servidorAddr;
	unsigned short servidorPorta;
	char *IP_Servidor;
	unsigned int tamanhoMensagem;



	// Ip servidor distribuido, porta servidor distribuido
	IP_Servidor = "192.168.0.52";
	servidorPorta = 10121;

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("Erro no socket()\n");
		return;
	}

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

	// Connect
	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
		printf("Erro no connect()\n");
		return;
	}

	

	char comando[20];
	printf("Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas, 4 e 5 ar-condicionados\ne 6 caso deseje acionar os 2 ar-condicionados\n");

	int validation;
	while(scanf("%s",comando),validation = atoi(comando),validation!=-1){
		
		while(validation < 0 || validation> 6){
			printf("Comando Incorreto. Por Favor escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas, 4 e 5 ar-condicionados\ne 6 caso deseje acionar os 2 ar-condicionados\n");
			scanf("%s",comando);
			validation = atoi(comando);
		}
		
		tamanhoMensagem = strlen(comando);
		if(send(clienteSocket, comando, tamanhoMensagem, 0) != tamanhoMensagem)
			printf("Erro no envio: numero de bytes enviados diferente do esperado\n");

		printf("Escolha um comando de 0 a 6. 0 a 3 sendo as lâmpadas, 4 e 5 ar-condicionados\ne 6 caso deseje acionar os 2 ar-condicionados\n");

	}
	close(clienteSocket);
	
}

void trata_interrupcao_Cliente(){
	close(clienteSocket);
}