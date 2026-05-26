#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

/* ============================================================
 * priority_queue.h - TAD Fila de Prioridade (Max-Heap)
 *
 * Pacotes com prioridade ALTA (1) são sempre processados
 * antes dos de prioridade NORMAL (0).  Dentro da mesma
 * prioridade, a ordem de chegada (FIFO) é preservada via
 * número de sequência crescente.
 * ============================================================ */

#include "packet.h"

#define PQ_MAX_SIZE 256

typedef struct {
    Packet packet;
    int    seq;        /* sequência de inserção (desempate FIFO) */
} PQNode;

typedef struct {
    PQNode nodes[PQ_MAX_SIZE];
    int    size;
    int    seq_counter; /* contador global de sequência */
} PriorityQueue;

/* Inicializa a fila vazia */
void pq_init(PriorityQueue *pq);

/* Retorna 1 se vazia, 0 caso contrário */
int pq_is_empty(const PriorityQueue *pq);

/* Retorna 1 se cheia, 0 caso contrário */
int pq_is_full(const PriorityQueue *pq);

/* Insere pacote mantendo propriedade do heap.
 * Retorna 1 em sucesso, 0 se cheia. */
int pq_enqueue(PriorityQueue *pq, Packet packet);

/* Remove e retorna o pacote de maior prioridade.
 * Retorna 1 em sucesso, 0 se vazia. */
int pq_dequeue(PriorityQueue *pq, Packet *out);

/* Espia o topo sem remover.
 * Retorna 1 em sucesso, 0 se vazia. */
int pq_peek(const PriorityQueue *pq, Packet *out);

/* Exibe o estado atual da fila */
void pq_print(const PriorityQueue *pq);

#endif /* PRIORITY_QUEUE_H */
