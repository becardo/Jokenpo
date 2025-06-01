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

typedef enum { // Enumeração das jogadas possíveis, de 0 a 4
    NUCLEAR_ATTACK = 0,
    INTERCEPT_ATTACK,
    CYBER_ATTACK,
    DRONE_STRIKE,
    BIO_ATTACK
} Acao_jogada;

const char* str_Acao_jogada[] = { // Representação textual das jogadas possíveis
    "Nuclear Attack",
    "Intercept Attack",
    "Cyber Attack",
    "Drone Strike",
    "Bio Attack"
};

Acao_jogada Escolha_server(){ // Função para o servidor escolher uma jogada aleatória, dentre as opções da struct Acao_jogada (0 a 4)
    return(Acao_jogada)(rand() % 5);
}

int batalha(Acao_jogada acao_server, Acao_jogada acao_client){
    // Função para a batalha. Retorna -1 quando o jogo fica empatado, 1 quando o CLIENTE vence o SERVIDOR, 
    // e 0 quando o SERVIDOR vence o CLIENTE
    if(acao_server == acao_client){
        return -1; // Retorna empate
    }
    else {
        switch (acao_client){ 
            case NUCLEAR_ATTACK:
                // Nuclear Attack vence Cyber Attack e Drone Strike
                if(acao_server == CYBER_ATTACK || acao_server == DRONE_STRIKE){
                    return 1; 
                } else break;

            break;
            
            case INTERCEPT_ATTACK:
                // Intercept Attack vence Nuclear Attack e Bio Attack
                if(acao_server == NUCLEAR_ATTACK || acao_server == BIO_ATTACK){
                    return 1; 
                } else break; 

            break;

            case CYBER_ATTACK:
                // Cyber Attack vence Intercept Attack e Drone Strike
                if(acao_server == INTERCEPT_ATTACK || acao_server == DRONE_STRIKE){
                    return 1; 
                } else break; 

            break;

            case DRONE_STRIKE:
                // Drone Strike vence Intercept Attack e Bio Attack
                if(acao_server == INTERCEPT_ATTACK || acao_server == BIO_ATTACK){
                    return 1;
                } else break; 

            break;

            case BIO_ATTACK:
                // Bio Attack vence Cyber Attack e Nuclear Attack
                if(acao_server == CYBER_ATTACK || acao_server == NUCLEAR_ATTACK){
                    return 1; 
                } else break; 

            break;
        }
    }
    return 0; // Não houve empate e nenhuma jogada retornou vitória, então cliente foi derrotado
}

int main(int argc, char **argv){
    // Verificação do número de argumentos passados pela linha de comando
    if (argc != 3) { /* Trecho inspirado na aula do Prof. Ítalo Cunha */
        printf("Entrada para utilização: %s <v4|v6> <server port>\n", argv[0]); 
        exit(1);
    }

    struct sockaddr_storage server_addr; // Armazena o endereço do servidor (suporta tanto v4 quanto v6)
    
    char *protocol = argv[1];
    char *port = argv[2];

    // Configurar um endereço de servidor para determinada rede
    inicializar_addr_server(protocol, port, &server_addr);

    int sock;
    sock = socket(server_addr.ss_family, SOCK_STREAM, 0); // Criação de um socket do tipo SOCK_STREAM

    printf("Servidor iniciado em modo IP%s na porta %s. Aguardando conexão...\n", protocol, port);

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

    int auxValid = 1; // auxiliar para evitar repetição da tabela de jogadas (explicação mais a frente)
    int vitoria_server = 0, vitoria_client = 0;
    srand(time(NULL));
    
    while (1) {  
        // Loop com toda a lógica do jogo, após conexão com o cliente
        GameMessage msg_solicitacao, msg_ataque_client; 
        memset(&msg_solicitacao, 0, sizeof(GameMessage));

        msg_solicitacao.type = MSG_REQUEST; 

        if(auxValid == 1){
            // na primeira execução do loop, auxValid está com valor 1 e a tabela de jogadas é impressa no terminal do cliente. 
            // em determinadas situações, o loop precisa ocorrer novamente mas sem a necessidade da impressão da tabela, então
            // auxValid recebe valor 0 e if não imprime a tabela novamente
            printf("Apresentando opções para o cliente.\n");
            snprintf(msg_solicitacao.message, MSG_SIZE, 
            "Escolha sua jogada:\n\n%d - Nuclear Attack\n%d - Intercept Attack\n%d - Cyber Attack\n%d - Drone Strike\n%d - Bio Attack\n",
            NUCLEAR_ATTACK, INTERCEPT_ATTACK, CYBER_ATTACK, DRONE_STRIKE, BIO_ATTACK);
            send(client_sock, &msg_solicitacao, sizeof(GameMessage), 0); // Envio da mensagem para o cliente
        }

        recv(client_sock, &msg_ataque_client, sizeof(GameMessage), 0); // Recebimento da mensagem vinda do cliente

        printf("Cliente escolheu %d.\n", msg_ataque_client.client_action);

        if (msg_ataque_client.client_action < NUCLEAR_ATTACK || msg_ataque_client.client_action > BIO_ATTACK) {
            // Se o cliente escolher uma opção de jogada fora do intervalo 0 - 4, uma mensagem de erro é exibida e a auxValid recebe valor 1
            // para que a tabela seja impressa novamente
            printf("Erro: opção inválida de jogada.\n");

            GameMessage msg_erro;
            memset(&msg_erro, 0, sizeof(GameMessage));
            msg_erro.type = MSG_ERROR;
            snprintf(msg_erro.message, MSG_SIZE, "Por favor, selecione um valor de 0 a 4.\n");
            send(client_sock, &msg_erro, sizeof(GameMessage), 0);
            auxValid = 1;
            
        } else{
            // Cliente escolheu uma opção válida de jogada, o jogo segue
            Acao_jogada ataque_client = msg_ataque_client.client_action;
            Acao_jogada ataque_server = Escolha_server();
            printf("Servidor escolheu aleatoriamente %d.\n", ataque_server);

            int resul_ = batalha(ataque_server, ataque_client); // Chama a função batalha, que retorna -1 para empate, 1 para vitória do Cliente e 0 para derrota do cliente (vitória do Servidor)
            char *resultado; // Char que será enviado para o cliente com o resultado da partida

            if(resul_ == 0){
                // Cliente derrotado pelo servidor
                resultado = "Derrota!";
                vitoria_server ++;
                printf("Placar atualizado: Cliente %d x %d Servidor\n", vitoria_client, vitoria_server);
            } else if(resul_ == 1){
                // Vitória do cliente sobre do servidor
                resultado = "Vitória!";
                vitoria_client ++;
                printf("Placar atualizado: Cliente %d x %d Servidor\n", vitoria_client, vitoria_server);
            } else{ // resultado == -1
                // empate
                resultado = "Empate!";
                printf("Jogo empatado.\n");
                printf("Solicitando ao cliente mais uma escolha.\n");

                auxValid = 1;
                continue; // Volta para o início do loop While para pedir novas jogadas ao cliente
            }

            GameMessage msg_resultado;
            memset(&msg_resultado, 0, sizeof(GameMessage));

            msg_resultado.type = MSG_RESULT;
            msg_resultado.client_action = msg_ataque_client.client_action; // Escolha de ataque do cliente
            msg_resultado.server_action = ataque_server; // Escolha de ataque do servidor

            snprintf(msg_resultado.message, MSG_SIZE, "Você escolheu: %s\nServidor escolheu: %s\nResultado: %s\n",
                str_Acao_jogada[msg_ataque_client.client_action],
                str_Acao_jogada[ataque_server], resultado);

            send(client_sock, &msg_resultado, sizeof(GameMessage), 0);

            GameMessage msg_jogar_novamente, msg_escolha_client;
            memset(&msg_jogar_novamente, 0, sizeof(GameMessage));

            while(1){
                // Cliente escolhe se quer jogar novamente
                msg_jogar_novamente.type = MSG_PLAY_AGAIN_REQUEST;

                printf("Perguntando se o cliente deseja jogar novamente.\n");

                snprintf(msg_jogar_novamente.message, MSG_SIZE, "Deseja jogar novamente?\n1 - Sim\n0 - Não\n");
                send(client_sock, &msg_jogar_novamente, sizeof(GameMessage), 0);

                recv(client_sock, &msg_escolha_client, sizeof(GameMessage), 0);

                if(msg_escolha_client.client_action != 0 && msg_escolha_client.client_action != 1){
                    // Se o cliente escolher uma opção diferente entre Sim e Não, exibe uma mensagem de erro em ambos os terminais
                    // e é dado um comando continue; , que retorna para o início do loop (no caso, o loop onde o cliente escolhe se quer jogar novamente) 
                    printf("Erro: resposta inválida para jogar novamente.\n");

                    GameMessage msg_erro_escolha;
                    memset(&msg_erro_escolha, 0, sizeof(GameMessage));
                    msg_erro_escolha.type = MSG_ERROR;
                    snprintf(msg_erro_escolha.message, MSG_SIZE, "Por favor, digite 1 para jogar novamente ou 0 para encerrar.\n");
                    send(client_sock, &msg_erro_escolha, sizeof(GameMessage), 0);

                    continue;
                } 
                
                else if(msg_escolha_client.client_action == 0){
                    // Cliente não quer jogar novamente, é exibida a mensagem final e passados para o cliente os valores finais do placar  
                    // do Cliente e do Servidor , através de client_wins e server_wins. Retonra 0 para finalizar o jogo
                    printf("Cliente não deseja jogar novamente.\n");
                    
                    GameMessage msg_final;
                    memset(&msg_final, 0, sizeof(GameMessage));

                    msg_final.type = MSG_END;
                    msg_final.client_wins = vitoria_client;
                    msg_final.server_wins = vitoria_server;
                    snprintf(msg_final.message, MSG_SIZE, "Fim de jogo!");
                    send(client_sock, &msg_final, sizeof(GameMessage), 0);

                    printf("Encerrando conexão.\n");
                    close(client_sock);
                    printf("Cliente desconectado.\n");
                    close(sock);
                    return 0;
                } 
                
                else{ // Resposta for sim, quer jogar novamente.
                    // Cliente deseja jogar novamente, então foi dado um break para sair do loop se o cliente quer jogar novamente, e
                    // volta para o início do loop principal
                    printf("Cliente deseja jogar novamente.\n");
                    auxValid = 1;
                    break;
                }
            }
        }
    }
}
