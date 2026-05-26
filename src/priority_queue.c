/* ============================================================
 * priority_queue.c - Implementação da Fila de Prioridade
 *
 * Estrutura: Max-Heap binário armazenado em array.
 *
 * Critério de ordenação (maior = mais prioritário):
 *   1° campo: priority (ALTA=1 > NORMAL=0)
 *   2° campo: seq  — MENOR seq = chegou antes = deve sair antes
 *             (desempate FIFO dentro da mesma prioridade)
 *
 * Complexidade:
 *   enqueue  O(log n)
 *   dequeue  O(log n)
 *   peek     O(1)
 * ============================================================ */

#include <stdio.h>
#include "priority_queue.h"

/* ---------------------------------------------------------- */
/*  Helpers internos                                           */
/* ---------------------------------------------------------- */

/* Retorna 1 se a > b segundo o critério de prioridade */
static int pqnode_has_priority(const PQNode *a, const PQNode *b) {
    if (a->packet.priority != b->packet.priority)
        return a->packet.priority > b->packet.priority;
    /* mesma prioridade: menor seq saiu antes (FIFO) */
    return a->seq < b->seq;
}

static void swap(PQNode *a, PQNode *b) {
    PQNode tmp = *a;
    *a = *b;
    *b = tmp;
}

static int parent(int i)      { return (i - 1) / 2; }
static int left_child(int i)  { return 2 * i + 1;   }
static int right_child(int i) { return 2 * i + 2;   }

/* Sobe o nó na posição i até restaurar a propriedade do heap */
static void heapify_up(PriorityQueue *pq, int i) {
    while (i > 0 && pqnode_has_priority(&pq->nodes[i], &pq->nodes[parent(i)])) {
        swap(&pq->nodes[i], &pq->nodes[parent(i)]);
        i = parent(i);
    }
}

/* Desce o nó na posição i até restaurar a propriedade do heap */
static void heapify_down(PriorityQueue *pq, int i) {
    int best = i;
    int l    = left_child(i);
    int r    = right_child(i);

    if (l < pq->size && pqnode_has_priority(&pq->nodes[l], &pq->nodes[best]))
        best = l;
    if (r < pq->size && pqnode_has_priority(&pq->nodes[r], &pq->nodes[best]))
        best = r;

    if (best != i) {
        swap(&pq->nodes[i], &pq->nodes[best]);
        heapify_down(pq, best);
    }
}

/* ---------------------------------------------------------- */
/*  Interface pública                                          */
/* ---------------------------------------------------------- */

void pq_init(PriorityQueue *pq) {
    pq->size        = 0;
    pq->seq_counter = 0;
}

int pq_is_empty(const PriorityQueue *pq) { return pq->size == 0; }
int pq_is_full (const PriorityQueue *pq) { return pq->size == PQ_MAX_SIZE; }

int pq_enqueue(PriorityQueue *pq, Packet packet) {
    if (pq_is_full(pq)) return 0;

    int idx = pq->size;
    pq->nodes[idx].packet = packet;
    pq->nodes[idx].seq    = pq->seq_counter++;
    pq->size++;

    heapify_up(pq, idx);
    return 1;
}

int pq_dequeue(PriorityQueue *pq, Packet *out) {
    if (pq_is_empty(pq)) return 0;

    *out = pq->nodes[0].packet;

    /* move o último para a raiz e reconstrói o heap */
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;

    if (pq->size > 0)
        heapify_down(pq, 0);

    return 1;
}

int pq_peek(const PriorityQueue *pq, Packet *out) {
    if (pq_is_empty(pq)) return 0;
    *out = pq->nodes[0].packet;
    return 1;
}

void pq_print(const PriorityQueue *pq) {
    if (pq_is_empty(pq)) {
        printf("  (fila vazia)\n");
        return;
    }
    printf("  Pacotes na fila (%d):\n", pq->size);
    for (int i = 0; i < pq->size; i++) {
        printf("    [%d] ID=%-4d | IP=%-16s | %s\n",
               i,
               pq->nodes[i].packet.id,
               pq->nodes[i].packet.dest_ip,
               pq->nodes[i].packet.priority == PRIORITY_HIGH ? "ALTA" : "Normal");
    }
}
