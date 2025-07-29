# 🧠💥 Jokenpoo — Batalha Estratégica em Rede

**Jokenpoo** é um jogo de batalha por turnos no terminal, desenvolvido em linguagem C com arquitetura cliente-servidor usando **sockets POSIX**. Ele funciona com **IPv4 e IPv6** e simula um combate entre dois jogadores por meio de ataques estratégicos.

> Inspirado em pedra-papel-tesoura, mas com **ataques militares cibernéticos** com regras mais complexas.

---

## 🧨 Ataques disponíveis e regras

| Ação             | Vence de…                             | Perde para…                            |
|------------------|----------------------------------------|----------------------------------------|
| **Nuclear Attack** | Cyber Attack, Drone Strike             | Intercept Attack, Bio Attack           |
| **Intercept Attack** | Nuclear Attack, Bio Attack              | Cyber Attack, Drone Strike             |
| **Cyber Attack** | Intercept Attack, Drone Strike         | Nuclear Attack, Bio Attack             |
| **Drone Strike** | Intercept Attack, Bio Attack           | Nuclear Attack, Cyber Attack           |
| **Bio Attack**   | Nuclear Attack, Cyber Attack           | Intercept Attack, Drone Strike         |

---

## 🚀 Funcionalidades

- ✅ Jogo de Jokenpô com lógica de vitória, empate e derrota.
- ✅ Modo cliente-servidor via terminal (sem interface gráfica).
- ✅ Comunicação de rede com `socket()` e `bind()` nativos do C.
- ✅ Suporte a **IPv4 (`AF_INET`)** e **IPv6 (`AF_INET6`)**.
- ✅ Interface de linha de comando simples e interativa.

---

## 🛠️ Tecnologias utilizadas

- Linguagem C
- POSIX sockets
- Protocolo TCP/IP
- IPv4 e IPv6
- Terminal/Linux

---

## 📦 Como compilar e jogar?

### ✅ Compilando

```bash
gcc servidor.c -o servidor
gcc cliente.c -o cliente
```

Ou simplesmente executar o arquivo Makefile, digitando 'make' no Terminal.

### 🚀 Executando

- Conectando Servidor 
```bash
./server v4 51511
```

- Conectando Cliente
```bash
./client 127.0.0.1 51511
```
Conectar-se ao IP 127.0.0.1 (IPv4) ou ::1 (IPv6)!

---

## 🧪 Testes locais
Utilizar `tmux` ou `screen`, ou abrir várias abas de terminal.

