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

    char *server_ip = argv[1];
    char *port = argv[2];

    // Configurar o endereço de rede (v4 ou v6) em uma estrutura do tipo sockaddre_storage
    configura_addr(server_ip, port, &server_addr);

    int sock;
    sock = socket(server_addr.ss_family, SOCK_STREAM, 0); // Criação de um socket do tipo SOCK_STREAM

    struct sockaddr *addr = (struct sockaddr *)(&server_addr); // Faz um cast de sockaddr_storage para sockaddr

    connect(sock, addr, sizeof(server_addr)); // Tenta conexão com o servidor através do socket criado

    printf("Conectado ao servidor.\n");

    while(1){
        // Loop com toda a lógica do jogo, após conexão com o servidor
        GameMessage msg_recebida_server, msg_resposta; 
        memset(&msg_resposta, 0, sizeof(GameMessage));

        recv(sock, &msg_recebida_server, sizeof(GameMessage), 0); 

        // Laço if - else if para verificar o tipo de mensagem recebida pelo servidor
        if (msg_recebida_server.type == MSG_REQUEST){
            // Mensagem de solicitação de ataque, e envio da escolha 
            printf("%s\n", msg_recebida_server.message);
            msg_resposta.type = MSG_RESPONSE;
            scanf("%d", &msg_resposta.client_action);

            send(sock, &msg_resposta, sizeof(GameMessage), 0);

        } else if (msg_recebida_server.type == MSG_ERROR || msg_recebida_server.type == MSG_RESULT){
            // MSG_ERROR e MSG_RESULT apenas enviam uma mensagem a ser exibida no terminal
            printf("%s\n", msg_recebida_server.message);

        } else if (msg_recebida_server.type == MSG_PLAY_AGAIN_REQUEST){
            // Mensagem de solicitação se quer jogar novamente, e envio da escolha
            printf("%s\n", msg_recebida_server.message);
            msg_resposta.type = MSG_PLAY_AGAIN_RESPONSE;
            scanf("%d", &msg_resposta.client_action);

            send(sock, &msg_resposta, sizeof(GameMessage), 0);

        } else if(msg_recebida_server.type == MSG_END){
            // Mensagem de finalização, exibindo no terminal do cliente o placar com os parametros passados através de cliente.wins e server.wins
            printf("%s\n", msg_recebida_server.message);
            printf("Placar final: Você %d x %d Servidor\n", msg_recebida_server.client_wins, msg_recebida_server.server_wins);
            printf("Obrigado por jogar!\n");
            break;
        }
    }

close(sock); // Fecha o socket
return 0;
}
