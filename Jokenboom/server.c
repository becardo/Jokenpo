#include "common_module.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>

/* Alguns trechos de código foram inspirados nas aplicações da Playlist de Programação de Sockets
do Prof. Ítalo Cunha, e serão devidamente sinalizados. 
Vídeo-aula disponível em: https://www.youtube.com/watch?v=tJ3qNtv0HVs&list=PLyrH0CFXIM5Wzmbv-lC-qvoBejsa803Qk&ab_channel=%C3%8DtaloCunha*/

typedef enum {
    NUCLEAR_ATTACK = 0,
    INTERCEPT_ATTACK = 1,
    CYBER_ATTACK = 2,
    DRONE_STRIKE = 3,
    BIO_ATTACK = 4
} Acao_jogada;

int main(int argc, char **argv){
    // Verificação do número de argumentos passados pela linha de comando
    if (argc != 3) { /* Trecho inspirado na aula do Prof. Ítalo Cunha */
        printf("Entrada para utilização: %s <v4|v6> <server port>\n", argv[0]); 
        exit(1);
    }

    struct sockaddr_storage server_addr; // Armazena o endereço do servidor (suporta tanto v4 quanto v6)

    // Configurar um endereço de servidor para determinada rede
    inicializar_addr_server(argv[1], argv[2], &server_addr);

    int sock;
    sock = socket(server_addr.ss_family, SOCK_STREAM, 0); // Criação de um socket do tipo SOCK_STREAM

    printf("Servidor iniciado em modo IP%s na porta %s. Aguardando conexão...\n", argv[1], argv[2]);

    int opt = 1; // Ativação 
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Permitir reutilização do endereço IP e porta

    struct sockaddr *addr = (struct sockaddr *)(&server_addr); // Faz um cast de sockaddr_storage para sockaddr

    bind(sock, addr, sizeof(server_addr)); // Associa o socket ao endereço do servidor
    listen(sock, 1); // Socket em escuta para 1 cliente

    /* Trecho inspirado na aula do Prof. Ítalo Cunha: servidor aceitar conexões do cliente */
    struct sockaddr_storage addr_client; // Armazenar endereço do cliente
    struct sockaddr *c_addr = (struct sockaddr *)(&addr_client); 
    socklen_t addrlen_client = sizeof(addr_client); // Tamanho da estrutura que armazena o endereço do cliente

    int client_sock = accept(sock, c_addr, &addrlen_client); // Conexão aceita em um novo socket para o cliente
    // A comunicação será feita pelo socket do cliente
    printf("Cliente conectado.\n");
    
    while (1) {  
        printf("Apresentando opções para o cliente.\n");

        GameMessage msg_solicitacao, msg_recebida_client; 
        memset(&msg_solicitacao, 0, sizeof(GameMessage));
        msg_solicitacao.type = MSG_REQUEST; 

        snprintf(msg_solicitacao.message, MSG_SIZE, 
        "Escolha sua jogada:\n\n%d - Nuclear Attack\n%d - Intercept Attack\n%d - Cyber Attack\n%d - Drone Strike\n%d - Bio Attack\n",
        NUCLEAR_ATTACK, INTERCEPT_ATTACK, CYBER_ATTACK, DRONE_STRIKE, BIO_ATTACK);
        
        send(client_sock, &msg_solicitacao, sizeof(GameMessage), 0);

        recv(client_sock, &msg_recebida_client, sizeof(GameMessage), 0);

        if (msg_recebida_client.type == MSG_RESPONSE) {
            printf("Cliente escolheu %d.\n", msg_recebida_client.client_action);

            if (msg_recebida_client.client_action < NUCLEAR_ATTACK || msg_recebida_client.client_action > BIO_ATTACK) {
                printf("Erro: opção inválida de jogada.\n");

                GameMessage msg_erro;
                memset(&msg_erro, 0, sizeof(GameMessage));
                msg_erro.type = MSG_ERROR;
                snprintf(msg_erro.message, MSG_SIZE, "Por favor, selecione um valor de 0 a 4.\n");
                send(client_sock, &msg_erro, sizeof(GameMessage), 0);
                
            } else{
                printf("Deus é fiel!\n");
                break;
            }
        }
    }

printf("Encerrando conexão.\n");
close(client_sock);
printf("Cliente desconectado.\n");
close(sock);
}
