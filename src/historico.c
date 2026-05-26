/* ============================================================
 * history.c - Implementação da Lista Duplamente Encadeada
 *
 * Complexidade:
 *   append          O(1)  (mantemos ponteiro tail)
 *   print_forward   O(n)
 *   print_backward  O(n)
 *   destroy         O(n)
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include "history.h"

void history_init(History *h) {
    h->head = NULL;
    h->tail = NULL;
    h->size = 0;
}

void history_destroy(History *h) {
    HistoryNode *cur = h->head;
    while (cur) {
        HistoryNode *next = cur->next;
        free(cur);
        cur = next;
    }
    h->head = NULL;
    h->tail = NULL;
    h->size = 0;
}

int history_append(History *h, Packet packet, int cache_hit) {
    HistoryNode *node = (HistoryNode *)malloc(sizeof(HistoryNode));
    if (!node) return 0;

    node->packet    = packet;
    node->cache_hit = cache_hit;
    node->next      = NULL;
    node->prev      = h->tail;

    if (h->tail)
        h->tail->next = node;
    else
        h->head = node;   /* lista estava vazia */

    h->tail = node;
    h->size++;
    return 1;
}

void history_print_forward(const History *h) {
    printf("\n--- HISTÓRICO DE PROCESSAMENTO (LISTA ENCADEADA) ---\n");
    if (h->size == 0) {
        printf("  (histórico vazio)\n");
        printf("-----------------------------------------------------\n");
        return;
    }

    printf("  [INÍCIO]");
    HistoryNode *cur = h->head;
    while (cur) {
        printf(" -> [ID: %d | IP: %s | %s | Cache: %s]",
               cur->packet.id,
               cur->packet.dest_ip,
               cur->packet.priority == PRIORITY_HIGH ? "ALTA" : "Normal",
               cur->cache_hit ? "HIT" : "MISS");
        cur = cur->next;
    }
    printf(" -> [FIM]\n");
    printf("  Total processados: %d\n", h->size);
    printf("-----------------------------------------------------\n");
}

void history_print_backward(const History *h) {
    printf("\n--- HISTÓRICO REVERSO (mais recente primeiro) ---\n");
    if (h->size == 0) {
        printf("  (histórico vazio)\n");
        printf("-------------------------------------------------\n");
        return;
    }

    printf("  [FIM]");
    HistoryNode *cur = h->tail;
    while (cur) {
        printf(" -> [ID: %d | IP: %s]", cur->packet.id, cur->packet.dest_ip);
        cur = cur->prev;
    }
    printf(" -> [INÍCIO]\n");
    printf("-------------------------------------------------\n");
}
