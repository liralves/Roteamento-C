/* ============================================================
 * packet.c - Implementação do tipo Pacote
 * ============================================================ */

#include <stdio.h>
#include <string.h>
#include "packet.h"

Packet packet_create(int id, const char *dest_ip, Priority priority, const char *content) {
    Packet p;
    p.id       = id;
    p.priority = priority;
    strncpy(p.dest_ip, dest_ip,  MAX_IP_LEN - 1);
    strncpy(p.content, content,  MAX_CONTENT - 1);
    p.dest_ip[MAX_IP_LEN - 1] = '\0';
    p.content[MAX_CONTENT - 1] = '\0';
    return p;
}

void packet_print(const Packet *p) {
    printf("[ID: %d | IP: %s | Prioridade: %s | Conteúdo: %s]",
           p->id,
           p->dest_ip,
           p->priority == PRIORITY_HIGH ? "ALTA" : "Normal",
           p->content);
}
