#pragma once

#ifndef COMMON_MODULE_H
#define COMMON_MODULE_H

#include <stdlib.h>
#include <stdio.h>

#include <arpa/inet.h> 
#include <string.h>

#define MSG_SIZE 256

typedef enum {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_RESULT,
    MSG_PLAY_AGAIN_REQUEST,
    MSG_PLAY_AGAIN_RESPONSE,
    MSG_ERROR,
    MSG_END
} MessageType;

typedef struct {
    int type; // Tipo da mensagem
    int client_action;
    int server_action;
    int result;
    int client_wins;
    int server_wins;
    char message[MSG_SIZE];
} GameMessage;

int configura_addr(const char *str_addr, const char *porta_str, struct sockaddr_storage *server_addr);

int inicializar_addr_server(const char *protocolo, const char* porta_str, struct sockaddr_storage *server_addr);

#endif
