#include "cliente.h"

int clienteSocket;


int init_Cliente() {
	
	struct sockaddr_in servidorAddr;
	unsigned short servidorPorta;
	char *IP_Servidor;
	



	// Ip servidor distribuido, porta servidor distribuido
	IP_Servidor = "192.168.0.53";
	servidorPorta = 10021;

	// Criar Socket
	if((clienteSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		printf("Erro no socket()\n");
		return 1 ;
	}

	// Construir struct sockaddr_in
	memset(&servidorAddr, 0, sizeof(servidorAddr)); // Zerando a estrutura de dados
	servidorAddr.sin_family = AF_INET;
	servidorAddr.sin_addr.s_addr = inet_addr(IP_Servidor);
	servidorAddr.sin_port = htons(servidorPorta);

	// Connect
	if(connect(clienteSocket, (struct sockaddr *) &servidorAddr, sizeof(servidorAddr)) < 0){
		printf("Erro. Cliente Distribuido não conseguiu conectar com o Servidor Central \n");
		return 1;
	}
    return 0;
}	

int send_TCP_message(struct atualizacao * updates){
	
    unsigned int tamanhoMensagem;
    tamanhoMensagem = sizeof(struct atualizacao);
    
    int t1;
    if(t1 = send(clienteSocket, (void*)updates, tamanhoMensagem, 0),t1 != tamanhoMensagem){
        printf("Erro no envio: numero de bytes enviados diferente do esperado\n");
		return 1;
    }
    
    
	return 0;
	//close(clienteSocket);
	
}

void trata_interrupcao_Cliente(){
	close(clienteSocket);
}