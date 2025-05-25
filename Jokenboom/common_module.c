#include "common_module.h"

/* Alguns trechos de código foram inspirados nas aplicações da Playlist de Programação de Sockets
do Prof. Ítalo Cunha, e serão devidamente sinalizados. 
Vídeo-aula disponível em: https://www.youtube.com/watch?v=tJ3qNtv0HVs&list=PLyrH0CFXIM5Wzmbv-lC-qvoBejsa803Qk&ab_channel=%C3%8DtaloCunha*/

// Função para configurar o endereço de rede (v4 ou v6) em uma estrutura do tipo sockaddre_storage
/* Trecho inspirado na aula do Prof. Ítalo Cunha */
int configura_addr(const char *str_addr, const char *porta_str, struct sockaddr_storage *server_addr){

    uint16_t porta_int = htons((uint16_t)atoi(porta_str)); // Converte a string da porta para um número intero
    // htons converte o número inteiro da porta para big-endian (usado em rede)

    struct in_addr endereco_v4; 
    struct in6_addr endereco_v6; 

    if(inet_pton(AF_INET, str_addr, &endereco_v4)){ // inet_pton converte str_addr para binário, e testa se é v4
        struct sockaddr_in *addr_v4 = (struct sockaddr_in *)server_addr; // conversão de tipo, para struct sockaddr_in especifica para IPv4
        addr_v4 -> sin_family = AF_INET; // Definição da família
        addr_v4 -> sin_port = porta_int; // sin_port armazena a porta em formato big-endian
        addr_v4 -> sin_addr = endereco_v4; // sin_addr armazena o endereço em binário
        return 0;
    }

    if(inet_pton(AF_INET6, str_addr, &endereco_v6)){
        struct sockaddr_in6 *addr_v6 = (struct sockaddr_in6 *)server_addr;
        addr_v6 -> sin6_family = AF_INET6;
        addr_v6 -> sin6_port = porta_int;
        memcpy(&(addr_v6 -> sin6_addr), &endereco_v6, sizeof(endereco_v6));
        /* memcpy foi utilizado para copiar os 16 bytes do endereço IPv6 (endereco_v6) para o campo sin6_addr, 
        já que sin6_addr é um array de bytes e endereco_v6 é uma estrutura in6_addr. Isso garante que todos
        os bytes do endereço sejam copiados da maneira correta */
        return 0;
    }

    return -1;
}

// Função para configurar um endereço de servidor para determinada rede 
/* Trecho inspirado na aula do Prof. Ítalo Cunha */
int inicializar_addr_server(const char *protocolo, const char* porta_str, struct sockaddr_storage *server_addr){
    
    uint16_t porta_int = htons((uint16_t)atoi(porta_str)); 

    memset(server_addr, 0, sizeof(*server_addr)); // Zera estrutura de armazenamento evitar lixo de memória

    // Configura endereço para IPv4
    if(strcmp(protocolo, "v4") == 0){ // compara se as strings são iguais
        struct sockaddr_in *addr_v4 = (struct sockaddr_in *)server_addr; 
        addr_v4 -> sin_family = AF_INET; // Definição da família do protocolo (AF_INET)
        addr_v4 -> sin_addr.s_addr = INADDR_ANY; // Configura o endereço IP como qualquer
        addr_v4 -> sin_port = porta_int; // Definição da porta do servidor 
        return 0;    
    } 
    
    // Configura endereço para IPv6
    else if (strcmp(protocolo, "v6") == 0){
        struct sockaddr_in6 *addr_v6 = (struct sockaddr_in6 *)server_addr;
        addr_v6 -> sin6_family = AF_INET6;
        addr_v6 -> sin6_addr = in6addr_any;
        addr_v6 -> sin6_port = porta_int;
        return 0;
    } 

    return -1; // Protocolo inválido
}
