#include "common_module.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

/* Alguns trechos de código foram inspirados nas aplicações da Playlist de Programação de Sockets
do Prof. Ítalo Cunha, e serão devidamente sinalizados. 
Vídeo-aula disponível em: https://www.youtube.com/watch?v=tJ3qNtv0HVs&list=PLyrH0CFXIM5Wzmbv-lC-qvoBejsa803Qk&ab_channel=%C3%8DtaloCunha*/


int main(int argc, char **argv){
    if (argc != 3) { 
        printf("Entrada para utilização: %s <server IP> <server port>\n", argv[0]);
    }

    struct sockaddr_storage server_addr; // Armazena o endereço do servidor

    // Configurar o endereço de rede (v4 ou v6) em uma estrutura do tipo sockaddre_storage
    configura_addr(argv[1], argv[2], &server_addr);

    int sock;
    sock = socket(server_addr.ss_family, SOCK_STREAM, 0); // Criação de um socket do tipo SOCK_STREAM

    struct sockaddr *addr = (struct sockaddr *)(&server_addr); // Faz um cast de sockaddr_storage para sockaddr

    int conn = connect(sock, addr, sizeof(server_addr)); // Tenta conexão com o servidor através do socket criado

    if ( conn == 0) { // Se conexão bem-sucedida, exibe mensagem de exito
        printf("Conectado ao servidor.\n");
    }

    // Lógica do Jokenboom
    while(1){
        GameMessage msg_recebida_server, msg_resposta; 
        memset(&msg_resposta, 0, sizeof(GameMessage));

        recv(sock, &msg_recebida_server, sizeof(GameMessage), 0); 

        // Verifica o tipo da mensagem recebida
        if (msg_recebida_server.type == MSG_REQUEST) {

            printf("%s\n", msg_recebida_server.message);
            msg_resposta.type = MSG_RESPONSE;
            scanf("%d", &msg_resposta.client_action);

            send(sock, &msg_resposta, sizeof(GameMessage), 0);
        } 
        
        else if (msg_recebida_server.type == MSG_ERROR) {

            printf("%s\n", msg_recebida_server.message);
        }
    }
   

    close(sock); // Fecha o socket
    return 0;
}
