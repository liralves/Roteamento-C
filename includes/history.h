#ifndef HISTORY_H
#define HISTORY_H

/* ============================================================
 * history.h - TAD Lista Duplamente Encadeada
 *
 * Mantém o histórico (log) de todos os pacotes processados.
 * Permite percorrer os eventos em ordem cronológica (frente)
 * e em ordem reversa (trás).
 * ============================================================ */

#include "packet.h"

typedef struct HistoryNode {
    Packet            packet;
    int               cache_hit;   /* 1 = hit no cache, 0 = miss */
    struct HistoryNode *prev;
    struct HistoryNode *next;
} HistoryNode;

typedef struct {
    HistoryNode *head;
    HistoryNode *tail;
    int          size;
} History;

/* Inicializa lista vazia */
void history_init(History *h);

/* Libera toda a memória da lista */
void history_destroy(History *h);

/* Adiciona pacote ao final da lista (append).
 * Retorna 1 em sucesso, 0 em falha. */
int history_append(History *h, Packet packet, int cache_hit);

/* Exibe lista do início ao fim */
void history_print_forward(const History *h);

/* Exibe lista do fim ao início (ordem reversa) */
void history_print_backward(const History *h);

#endif /* HISTORY_H */
