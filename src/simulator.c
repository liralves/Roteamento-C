/* ============================================================
 * simulator.c - Orquestrador do Sistema de Roteamento
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "simulator.h"

/* ---------------------------------------------------------- */
/*  Helpers internos                                           */
/* ---------------------------------------------------------- */

/* Remove espaços e \r do início/fim da string */
static void trim(char *s) {
    /* direita */
    int n = (int)strlen(s) - 1;
    while (n >= 0 && (s[n] == '\n' || s[n] == '\r' || s[n] == ' '))
        s[n--] = '\0';
    /* esquerda */
    int start = 0;
    while (s[start] && (s[start] == ' ' || s[start] == '\t'))
        start++;
    if (start > 0)
        memmove(s, s + start, strlen(s + start) + 1);
}

/* Processa uma única linha de comando */
static void process_line(Simulator *sim, const char *raw_line) {
    char line[256];
    strncpy(line, raw_line, sizeof(line) - 1);
    line[sizeof(line) - 1] = '\0';
    trim(line);

    if (line[0] == '\0' || line[0] == '#')
        return;  /* linha vazia ou comentário */

    /* --- ADD;ID;IP;PRIORIDADE;CONTEUDO --- */
    if (strncmp(line, "ADD", 3) == 0) {
        /* skip "ADD" */
        strtok(line, ";");               /* "ADD" */
        char *s_id   = strtok(NULL, ";");
        char *s_ip   = strtok(NULL, ";");
        char *s_pri  = strtok(NULL, ";");
        char *s_cont = strtok(NULL, ";");

        if (!s_id || !s_ip || !s_pri || !s_cont) {
            printf("[ERRO] Formato inválido para ADD: use ADD;ID;IP;PRIORIDADE;CONTEUDO\n");
            return;
        }

        int      id       = atoi(s_id);
        Priority priority = (atoi(s_pri) == 1) ? PRIORITY_HIGH : PRIORITY_NORMAL;
        sim_add_packet(sim, id, s_ip, priority, s_cont);
        return;
    }

    if (strcmp(line, "PROCESS") == 0)      { sim_process(sim);       return; }
    if (strcmp(line, "SHOW_CACHE") == 0)   { sim_show_cache(sim);    return; }
    if (strcmp(line, "SHOW_HISTORY") == 0) { sim_show_history(sim);  return; }

    printf("[AVISO] Comando desconhecido: \"%s\"\n", line);
}

/* ---------------------------------------------------------- */
/*  Interface pública                                          */
/* ---------------------------------------------------------- */

void sim_init(Simulator *sim) {
    pq_init(&sim->queue);
    ht_init(&sim->cache);
    history_init(&sim->history);
    printf("[SISTEMA INICIALIZADO - Tabela Hash de tamanho %d]\n\n", TABLE_SIZE);
}

void sim_destroy(Simulator *sim) {
    ht_destroy(&sim->cache);
    history_destroy(&sim->history);
}

void sim_add_packet(Simulator *sim, int id, const char *ip,
                    Priority priority, const char *content) {
    Packet p = packet_create(id, ip, priority, content);
    printf(">> Lendo comando: ADD (Pacote %d - Prioridade: %s)\n",
           id, priority == PRIORITY_HIGH ? "ALTA" : "Normal");

    if (pq_enqueue(&sim->queue, p))
        printf("   Status: Pacote %d inserido na fila.\n\n", id);
    else
        printf("   [ERRO] Fila cheia! Pacote %d descartado.\n\n", id);
}

void sim_process(Simulator *sim) {
    printf(">> Lendo comando: PROCESS\n");

    if (pq_is_empty(&sim->queue)) {
        printf("   [AVISO] Nenhum pacote na fila.\n\n");
        return;
    }

    Packet p;
    pq_dequeue(&sim->queue, &p);

    printf("   Executando: Pacote %d (%s) - Prioridade %s.\n",
           p.id, p.content,
           p.priority == PRIORITY_HIGH ? "ALTA" : "Normal");

    /* Consulta cache */
    int cached_id = 0;
    int hit       = ht_search(&sim->cache, p.dest_ip, &cached_id);

    if (hit) {
        printf("   Cache: IP %s ENCONTRADO (Acesso rápido! Referência: Pacote %d).\n",
               p.dest_ip, cached_id);
    } else {
        printf("   Cache: IP %s não encontrado. Mapeando novo destino na Hash...\n",
               p.dest_ip);
        ht_insert(&sim->cache, p.dest_ip, p.id);
    }

    /* Registra no histórico */
    history_append(&sim->history, p, hit);
    printf("   Histórico: Pacote %d adicionado à Lista de Eventos.\n\n", p.id);
}

void sim_show_cache(const Simulator *sim) {
    printf(">> Lendo comando: SHOW_CACHE\n");
    ht_print(&sim->cache);
    printf("\n");
}

void sim_show_history(const Simulator *sim) {
    printf(">> Lendo comando: SHOW_HISTORY\n");
    history_print_forward(&sim->history);
    printf("\n");
}

void sim_run_file(Simulator *sim, const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[ERRO] Não foi possível abrir o arquivo: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp))
        process_line(sim, line);

    fclose(fp);
}

void sim_run_interactive(Simulator *sim) {
    char line[256];
    printf("Modo interativo. Digite os comandos (CTRL+D ou 'EXIT' para sair):\n\n");
    while (1) {
        printf("> ");
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        trim(line);
        if (strcmp(line, "EXIT") == 0 || strcmp(line, "exit") == 0) break;
        process_line(sim, line);
    }
}
