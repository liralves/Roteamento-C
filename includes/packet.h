#ifndef PACKET_H
#define PACKET_H

/* ============================================================
 * packet.h - Definição do tipo Pacote (TAD base do sistema)
 * ============================================================ */

#define MAX_IP_LEN    20
#define MAX_CONTENT   64

typedef enum {
    PRIORITY_NORMAL = 0,
    PRIORITY_HIGH   = 1
} Priority;

typedef struct {
    int      id;
    char     dest_ip[MAX_IP_LEN];
    Priority priority;
    char     content[MAX_CONTENT];
} Packet;

/* Cria e retorna um Packet preenchido */
Packet packet_create(int id, const char *dest_ip, Priority priority, const char *content);

/* Exibe os dados de um pacote no terminal */
void packet_print(const Packet *p);

#endif /* PACKET_H */
