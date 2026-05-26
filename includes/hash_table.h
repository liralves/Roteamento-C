#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* ============================================================
 * hash_table.h - TAD Tabela de Hashing (Cache de Rede)
 *
 * Chave  : endereço IP de destino (string)
 * Valor  : ID do último pacote processado para esse IP
 * Colisão: Encadeamento Exterior (lista ligada em cada bucket)
 *
 * Função de espalhamento:
 *   h(ip) = soma ponderada dos bytes ASCII do IP  mod  TABLE_SIZE
 *   (djb2-like: hash = hash * 31 + c)
 * ============================================================ */

#include "packet.h"

#define TABLE_SIZE 10   /* tamanho da tabela conforme exemplo do PDF */

/* Nó da lista de colisão em cada bucket */
typedef struct HashNode {
    char           ip[MAX_IP_LEN];
    int            packet_id;
    struct HashNode *next;
} HashNode;

typedef struct {
    HashNode *buckets[TABLE_SIZE];
    int       count;   /* total de entradas na tabela */
} HashTable;

/* Inicializa a tabela (todos os buckets NULL) */
void ht_init(HashTable *ht);

/* Libera toda a memória alocada pelos buckets */
void ht_destroy(HashTable *ht);

/* Função de espalhamento: devolve índice 0..TABLE_SIZE-1 */
int ht_hash(const char *ip);

/* Insere ou atualiza a entrada (ip → packet_id).
 * Retorna 1 em sucesso, 0 em falha de alocação. */
int ht_insert(HashTable *ht, const char *ip, int packet_id);

/* Busca o packet_id associado ao IP.
 * Retorna 1 e preenche *out_id se encontrado; 0 caso contrário. */
int ht_search(const HashTable *ht, const char *ip, int *out_id);

/* Exibe o estado completo da tabela */
void ht_print(const HashTable *ht);

#endif /* HASH_TABLE_H */
