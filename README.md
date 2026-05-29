# UNIVERSIDADE CATÓLICA DE PERNAMBUCO
<p align="center">
   <img src="http://www1.unicap.br/icam/wp-content/uploads/2019/06/marca_nova.svg" />
  </p>

# Sistema de Roteamento de Pacotes e Cache de Rede — Simulador

> Projeto da disciplina **Estrutura de Dados Lineares** — 2° Grau de Qualificação  
> Profa. Monique Soares

> Alunos: Allan Henrique e Luana Larissa

## Descrição

Simulador de tráfego de rede em C que gerencia o fluxo de pacotes de dados entre diferentes "nós". O sistema prioriza mensagens urgentes (Voz/Vídeo) e mantém um histórico e cache de endereços acessados para otimizar buscas.

---

## Arquitetura e Estrutura de Dados

O projeto implementa **três TADs** manualmente, sem uso de bibliotecas externas de estruturas de dados:

### 1. Fila de Prioridade (Max-Heap)
- **Arquivo:** `include/priority_queue.h` / `src/priority_queue.c`
- Implementada como heap binário em array estático
- **Critério de prioridade:** pacotes com `ALTA` (Voz/Vídeo) saem antes dos `NORMAL` (Dados)
- **Desempate FIFO:** dentro da mesma prioridade, usa número de sequência de inserção
- **Complexidade:** `enqueue O(log n)` | `dequeue O(log n)` | `peek O(1)`

### 2. Tabela Hash (Cache de Rede)
- **Arquivo:** `include/hash_table.h` / `src/hash_table.c`
- **Chave:** endereço IP de destino (string)
- **Valor:** ID do último pacote processado para aquele IP
- **Função de espalhamento (djb2 modificada):**
  ```
  hash = 5381
  para cada char c do IP:
      hash = hash * 31 + c
  índice = hash % TABLE_SIZE
  ```
- **Tratamento de colisão:** Encadeamento Exterior (lista ligada em cada bucket)
- **Complexidade média:** `insert O(1)` | `search O(1)`

### 3. Lista Duplamente Encadeada (Histórico)
- **Arquivo:** `include/history.h` / `src/history.c`
- Armazena todos os pacotes processados em ordem cronológica
- Cada nó guarda o pacote e se houve `cache HIT` ou `MISS`
- Permite percorrer o histórico nos dois sentidos (frente e trás)
- **Complexidade:** `append O(1)` | `print O(n)`

---

## Estrutura de Arquivos

```
network-simulator/
├── include/                  # Headers (interfaces dos TADs)
│   ├── packet.h              # Tipo base Packet
│   ├── priority_queue.h      # TAD Fila de Prioridade
│   ├── hash_table.h          # TAD Tabela Hash
│   ├── history.h             # TAD Lista Duplamente Encadeada
│   └── simulator.h           # Orquestrador do sistema
├── src/                      # Implementações (.c)
│   ├── packet.c
│   ├── priority_queue.c
│   ├── hash_table.c
│   ├── history.c
│   ├── simulator.c
│   └── main.c
├── tests/
│   ├── input.txt             # Arquivo de teste do PDF
│   └── input_extended.txt    # Teste estendido com colisões e cache hits
├── Makefile
└── README.md
```

---

## Compilação

### Pré-requisitos
- GCC (versão 9+)
- GNU Make

### Compilar
```bash
make
```

### Compilar + executar o teste padrão
```bash
make test
```

### Limpar binários
```bash
make clean
```

---

## Uso

### Modo arquivo (recomendado)
```bash
./simulador tests/input.txt
```

### Modo interativo
```bash
./simulador
```
Digite os comandos manualmente. Use `EXIT` para encerrar.

---

## Formato dos Comandos

| Comando | Formato | Descrição |
|---|---|---|
| `ADD` | `ADD;ID;IP;PRIORIDADE;CONTEUDO` | Insere pacote na fila |
| `PROCESS` | `PROCESS` | Processa o pacote de maior prioridade |
| `SHOW_CACHE` | `SHOW_CACHE` | Exibe a tabela hash (cache) |
| `SHOW_HISTORY` | `SHOW_HISTORY` | Exibe o histórico (lista encadeada) |

**PRIORIDADE:** `1` = Alta (Voz/Vídeo) | `0` = Normal (Dados)

Linhas iniciadas com `#` são tratadas como comentários.

---

## Exemplo de Arquivo de Entrada

```
ADD;101;192.168.0.1;0;Download_Arquivo_PDF
ADD;102;10.0.0.5;1;Chamada_Video_Teams
ADD;103;192.168.0.1;1;Streaming_Audio
ADD;104;172.16.254.1;0;Email_Trabalho
PROCESS
PROCESS
ADD;105;10.0.0.5;0;Sincronizacao_Nuvem
SHOW_CACHE
PROCESS
SHOW_HISTORY
```

## Saída Esperada

```
[SISTEMA INICIALIZADO - Tabela Hash de tamanho 10]

>> Lendo comando: ADD (Pacote 101 - Prioridade: Normal)
   Status: Pacote 101 inserido na fila.

>> Lendo comando: ADD (Pacote 102 - Prioridade: ALTA)
   Status: Pacote 102 inserido na fila.
...
>> Lendo comando: PROCESS
   Executando: Pacote 102 (Chamada_Video_Teams) - Prioridade ALTA.
   Cache: IP 10.0.0.5 não encontrado. Mapeando novo destino na Hash...
   Histórico: Pacote 102 adicionado à Lista de Eventos.

>> Lendo comando: PROCESS
   Executando: Pacote 103 (Streaming_Audio) - Prioridade ALTA.
   Cache: IP 192.168.0.1 não encontrado. Mapeando novo destino na Hash...
   Histórico: Pacote 103 adicionado à Lista de Eventos.
...
>> Lendo comando: PROCESS
   Executando: Pacote 101 (Download_Arquivo_PDF) - Prioridade Normal.
   Cache: IP 192.168.0.1 ENCONTRADO (Acesso rápido! Referência: Pacote 103).
   Histórico: Pacote 101 adicionado à Lista de Eventos.

>> Lendo comando: SHOW_HISTORY

--- HISTÓRICO DE PROCESSAMENTO (LISTA ENCADEADA) ---
  [INÍCIO] -> [ID: 102 | IP: 10.0.0.5 | ALTA | Cache: MISS]
           -> [ID: 103 | IP: 192.168.0.1 | ALTA | Cache: MISS]
           -> [ID: 101 | IP: 192.168.0.1 | Normal | Cache: HIT] -> [FIM]
  Total processados: 3
```

---

## Análise de Complexidade

| Operação | Estrutura | Complexidade |
|---|---|---|
| Inserir pacote | Heap (Max-Heap) | O(log n) |
| Processar pacote | Heap (Max-Heap) | O(log n) |
| Buscar IP no cache | Hash Table | O(1) médio |
| Inserir IP no cache | Hash Table | O(1) médio |
| Adicionar ao histórico | Lista Dupla (tail) | O(1) |
| Exibir histórico | Lista Dupla | O(n) |

---

## Decisões de Projeto

### Por que Max-Heap?
O heap binário é a escolha natural para filas de prioridade: garante `O(log n)` para inserção e remoção do máximo, sem reordenar o array inteiro a cada operação. O desempate FIFO é implementado com um contador de sequência crescente — dentro da mesma prioridade, o menor número de sequência vence (chegou antes, sai antes).

### Por que Encadeamento Exterior na Hash?
O encadeamento exterior (cada bucket é uma lista ligada) foi escolhido pois:
- Não há limite fixo de colisões por bucket
- A remoção de entradas é mais simples
- O fator de carga pode ultrapassar 1.0 sem degradação abrupta


### Modularização
Cada TAD é completamente encapsulado em seu par `.h`/`.c`. O `simulator.c` atua como camada de integração, e o `main.c` apenas inicializa e delega — facilitando testes unitários futuros por TAD.

### Desenvolvedores 
Luana Lira & Allan Henrique


