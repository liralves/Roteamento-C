#ifndef SIMULATOR_H
#define SIMULATOR_H

/* ============================================================
 * simulator.h - Orquestrador do Sistema de Roteamento
 *
 * Integra Fila de Prioridade, Tabela Hash e Lista Encadeada,
 * expondo operações de alto nível mapeadas aos comandos do
 * arquivo de entrada.
 * ============================================================ */

#include "priority_queue.h"
#include "hash_table.h"
#include "history.h"

typedef struct {
    PriorityQueue queue;
    HashTable     cache;
    History       history;
} Simulator;

/* Inicializa todos os TADs internos */
void sim_init(Simulator *sim);

/* Libera toda a memória alocada pelos TADs */
void sim_destroy(Simulator *sim);

/* ADD: insere pacote na fila de prioridade */
void sim_add_packet(Simulator *sim, int id, const char *ip,
                    Priority priority, const char *content);

/* PROCESS: processa o pacote de maior prioridade */
void sim_process(Simulator *sim);

/* SHOW_CACHE: exibe estado da tabela hash */
void sim_show_cache(const Simulator *sim);

/* SHOW_HISTORY: exibe histórico (lista encadeada) */
void sim_show_history(const Simulator *sim);

/* Lê e executa todos os comandos de um arquivo */
void sim_run_file(Simulator *sim, const char *filename);

/* Lê e executa comandos interativos do stdin */
void sim_run_interactive(Simulator *sim);

#endif /* SIMULATOR_H */
