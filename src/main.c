/* ============================================================
 * main.c - Ponto de entrada do Sistema de Roteamento de Pacotes
 *
 * Uso:
 *   ./simulador             → modo interativo (stdin)
 *   ./simulador input.txt   → lê comandos do arquivo
 * ============================================================ */

#include <stdio.h>
#include "simulator.h"

int main(int argc, char *argv[]) {
    Simulator sim;
    sim_init(&sim);

    if (argc >= 2) {
        sim_run_file(&sim, argv[1]);
    } else {
        sim_run_interactive(&sim);
    }

    /* Libera toda a memória dinâmica */
    sim_destroy(&sim);
    return 0;
}
