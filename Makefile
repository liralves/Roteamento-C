# ============================================================
# Makefile - Sistema de Roteamento de Pacotes e Cache de Rede
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c99 -g
IFLAGS  = -Iinclude

SRC_DIR = src
OBJ_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

TARGET  = simulador

# ─── Regra principal ────────────────────────────────────────
all: $(OBJ_DIR) $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)
	@echo "\n✔  Compilação concluída: ./$(TARGET)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

# ─── Teste com o arquivo de exemplo ─────────────────────────
test: $(TARGET)
	@echo "═══════════════════════════════════════"
	@echo "  Executando com tests/input.txt"
	@echo "═══════════════════════════════════════"
	./$(TARGET) tests/input.txt

# ─── Limpeza ────────────────────────────────────────────────
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean test
