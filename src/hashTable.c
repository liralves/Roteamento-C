/* ============================================================
 * hash_table.c - Implementação da Tabela Hash (Cache de Rede)
 *
 * Função de espalhamento (djb2 modificada):
 *   hash = 5381
 *   for each char c in ip:
 *       hash = hash * 31 + c
 *   return hash % TABLE_SIZE
 *
 * Complexidade média:
 *   insert  O(1)
 *   search  O(1)  (O(n) no pior caso com muitas colisões)
 *
 * Tratamento de colisão: Encadeamento Exterior
 *   Cada bucket é o início de uma lista ligada de HashNode.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

/* ---------------------------------------------------------- */
/*  Função de espalhamento                                     */
/* ---------------------------------------------------------- */

int ht_hash(const char *ip) {
    unsigned long hash = 5381;
    int c;
    while ((c = (unsigned char)*ip++) != '\0')
        hash = hash * 31 + c;
    return (int)(hash % TABLE_SIZE);
}

/* ---------------------------------------------------------- */
/*  Interface pública                                          */
/* ---------------------------------------------------------- */

void ht_init(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++)
        ht->buckets[i] = NULL;
    ht->count = 0;
}

void ht_destroy(HashTable *ht) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        HashNode *cur = ht->buckets[i];
        while (cur) {
            HashNode *next = cur->next;
            free(cur);
            cur = next;
        }
        ht->buckets[i] = NULL;
    }
    ht->count = 0;
}

int ht_insert(HashTable *ht, const char *ip, int packet_id) {
    int idx = ht_hash(ip);

    /* Verifica se o IP já existe no bucket (atualiza se sim) */
    HashNode *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->ip, ip) == 0) {
            cur->packet_id = packet_id;   /* atualização */
            return 1;
        }
        cur = cur->next;
    }

    /* Novo nó → insere no início da lista do bucket (O(1)) */
    HashNode *node = (HashNode *)malloc(sizeof(HashNode));
    if (!node) return 0;

    strncpy(node->ip, ip, MAX_IP_LEN - 1);
    node->ip[MAX_IP_LEN - 1] = '\0';
    node->packet_id = packet_id;
    node->next      = ht->buckets[idx];  /* encadeamento exterior */
    ht->buckets[idx] = node;
    ht->count++;
    return 1;
}

int ht_search(const HashTable *ht, const char *ip, int *out_id) {
    int idx      = ht_hash(ip);
    HashNode *cur = ht->buckets[idx];
    while (cur) {
        if (strcmp(cur->ip, ip) == 0) {
            *out_id = cur->packet_id;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

void ht_print(const HashTable *ht) {
    printf("\n--- ESTADO DA TABELA HASH (Cache) ---\n");
    int printed = 0;
    for (int i = 0; i < TABLE_SIZE; i++) {
        if (ht->buckets[i] != NULL) {
            printf("  [Índice %2d]: ", i);
            HashNode *cur = ht->buckets[i];
            while (cur) {
                printf("IP %s (Pacote %d)", cur->ip, cur->packet_id);
                if (cur->next) printf(" -> ");
                cur = cur->next;
            }
            printf("\n");
            printed = 1;
        }
    }
    if (!printed)
        printf("  (cache vazio)\n");
    printf("  Total de entradas: %d\n", ht->count);
    printf("-------------------------------------\n");
}
