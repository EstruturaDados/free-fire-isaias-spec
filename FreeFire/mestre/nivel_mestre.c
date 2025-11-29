#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_ITENS 10

// Struct do item com prioridade (1 a 5)
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // 1 (baixa) .. 5 (alta)
};

// Enum para critérios de ordenação
typedef enum {
    NONE,
    NAME,
    TYPE,
    PRIORITY
} Ordenacao;

// Remove '\n' do fgets
void tira_enter(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

/* Funções de comparação:
   Cada comparação incrementa o contador passado por ponteiro
   (queremos contar quantas comparações foram feitas durante a ordenação).
*/
int cmp_nome(const struct Item *a, const struct Item *b, int *contador) {
    (*contador)++;
    return strcmp(a->nome, b->nome);
}

int cmp_tipo(const struct Item *a, const struct Item *b, int *contador) {
    (*contador)++;
    int r = strcmp(a->tipo, b->tipo);
    if (r == 0) {
        // se o tipo for igual, desempata por nome (opcional)
        (*contador)++;
        return strcmp(a->nome, b->nome);
    }
    return r;
}

int cmp_prioridade(const struct Item *a, const struct Item *b, int *contador) {
    (*contador)++;
    // queremos ordem decrescente de prioridade (5 primeiro)
    if (a->prioridade == b->prioridade) return 0;
    return (b->prioridade - a->prioridade); // positivo se a < b
}

/* Insertion sort geral que usa um critério passado.
   O campo 'criterio' determina qual função de comparação é usada.
   Retorna o número de comparações feitas. */
int insertion_sort(struct Item mochila[], int total, Ordenacao criterio) {
    int comparacoes = 0;

    for (int i = 1; i < total; i++) {
        struct Item chave = mochila[i];
        int j = i - 1;

        // dependendo do critério, usamos a comparação correta
        if (criterio == NAME) {
            while (j >= 0 && cmp_nome(&mochila[j], &chave, &comparacoes) > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        } else if (criterio == TYPE) {
            while (j >= 0 && cmp_tipo(&mochila[j], &chave, &comparacoes) > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        } else if (criterio == PRIORITY) {
            while (j >= 0 && cmp_prioridade(&mochila[j], &chave, &comparacoes) > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        }
        mochila[j + 1] = chave;
    }

    return comparacoes;
}

/* Busca binária por nome — retorna índice ou -1.
   Usa strcmp diretamente (não conta comparações aqui além do que precisar). */
int busca_binaria_nome(struct Item mochila[], int total, const char *nome) {
    int left = 0, right = total - 1;
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strcmp(nome, mochila[mid].nome);
        if (cmp == 0) return mid;
        else if (cmp < 0) right = mid - 1;
        else left = mid + 1;
    }
    return -1;
}

int main() {
    struct Item mochila[MAX_ITENS];
    int total = 0;
    Ordenacao ordenadoPor = NONE; // estado atual da ordenação

    int opcao;

    printf("=== Nível Mestre - Ordenação e Busca Binária ===\n");

    do {
        printf("\n1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Ordenar itens\n");
        printf("5 - Buscar item por nome (busca binária)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            opcao = -1;
        }
        getchar(); // limpa '\n'

        switch (opcao) {

            case 1: { // Adicionar item (pede prioridade)
                if (total >= MAX_ITENS) {
                    printf("Mochila lotada. Remove algo antes.\n");
                    break;
                }
                printf("Nome do item: ");
                fgets(mochila[total].nome, sizeof(mochila[total].nome), stdin);
                tira_enter(mochila[total].nome);

                printf("Tipo do item (arma, municao, cura...): ");
                fgets(mochila[total].tipo, sizeof(mochila[total].tipo), stdin);
                tira_enter(mochila[total].tipo);

                printf("Quantidade: ");
                if (scanf("%d", &mochila[total].quantidade) != 1) {
                    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Quantidade inválida.\n");
                    break;
                }

                printf("Prioridade (1-baixa .. 5-alta): ");
                if (scanf("%d", &mochila[total].prioridade) != 1) {
                    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Prioridade inválida.\n");
                    break;
                }
                getchar(); // limpa '\n'

                if (mochila[total].prioridade < 1 || mochila[total].prioridade > 5) {
                    printf("Prioridade precisa estar entre 1 e 5. Item não adicionado.\n");
                } else {
                    total++;
                    // qualquer modificação quebra a ordenação atual
                    ordenadoPor = NONE;
                    printf("Item adicionado! (prioridade x%d)\n", mochila[total-1].prioridade);
                }
                break;
            }

            case 2: { // Remover item (primeira ocorrência por nome)
                if (total == 0) {
                    printf("Mochila vazia.\n");
                    break;
                }
                char nomeRemover[30];
                printf("Nome do item a remover: ");
                fgets(nomeRemover, sizeof(nomeRemover), stdin);
                tira_enter(nomeRemover);

                int pos = -1;
                for (int i = 0; i < total; i++) {
                    if (strcmp(mochila[i].nome, nomeRemover) == 0) {
                        pos = i;
                        break;
                    }
                }

                if (pos == -1) {
                    printf("Item '%s' não encontrado.\n", nomeRemover);
                } else {
                    for (int i = pos; i < total - 1; i++) mochila[i] = mochila[i + 1];
                    total--;
                    ordenadoPor = NONE; // remoção quebra a ordenação
                    printf("Item removido.\n");
                }
                break;
            }

            case 3: { // Listar
                if (total == 0) {
                    printf("Mochila vazia.\n");
                } else {
                    printf("\n--- Itens na mochila (%d/%d) ---\n", total, MAX_ITENS);
                    for (int i = 0; i < total; i++) {
                        printf("%d) %s | %s | x%d | prioridade: %d\n",
                               i + 1,
                               mochila[i].nome,
                               mochila[i].tipo,
                               mochila[i].quantidade,
                               mochila[i].prioridade);
                    }
                    printf("---------------------------------\n");
                    // informa se está ordenado e por qual critério
                    if (ordenadoPor == NAME) printf("Observação: atualmente ordenado por NOME.\n");
                    else if (ordenadoPor == TYPE) printf("Observação: atualmente ordenado por TIPO.\n");
                    else if (ordenadoPor == PRIORITY) printf("Observação: atualmente ordenado por PRIORIDADE.\n");
                    else printf("Observação: não está ordenado.\n");
                }
                break;
            }

            case 4: { // Ordenar itens (menu de critérios)
                if (total <= 1) {
                    printf("Não há itens suficientes para ordenar.\n");
                    break;
                }

                printf("\nEscolha o critério de ordenação:\n");
                printf("1 - Nome (crescente)\n");
                printf("2 - Tipo (crescente)\n");
                printf("3 - Prioridade (decrescente)\n");
                printf("Critério: ");
                int c;
                if (scanf("%d", &c) != 1) {
                    int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                    printf("Escolha inválida.\n");
                    break;
                }
                getchar(); // limpa '\n'

                Ordenacao criterio = NONE;
                if (c == 1) criterio = NAME;
                else if (c == 2) criterio = TYPE;
                else if (c == 3) criterio = PRIORITY;
                else {
                    printf("Opção inválida.\n");
                    break;
                }

                // roda insertion sort e conta comparações
                int comparacoes = insertion_sort(mochila, total, criterio);
                ordenadoPor = criterio;
                printf("Ordenação concluída. Comparações realizadas: %d\n", comparacoes);
                break;
            }

            case 5: { // Busca binária por nome
                if (total == 0) {
                    printf("Mochila vazia.\n");
                    break;
                }
                if (ordenadoPor != NAME) {
                    printf("Atenção: para usar busca binária a lista precisa estar ordenada por NOME.\n");
                    printf("Ordene antes (menu 4 -> critério Nome) e tente de novo.\n");
                    break;
                }
                char nomeBusca[30];
                printf("Digite o NOME a buscar (busca binária): ");
                fgets(nomeBusca, sizeof(nomeBusca), stdin);
                tira_enter(nomeBusca);

                int pos = busca_binaria_nome(mochila, total, nomeBusca);
                if (pos == -1) {
                    printf("Item '%s' não encontrado (busca binária).\n", nomeBusca);
                } else {
                    printf("\nItem encontrado (posição %d):\n", pos + 1);
                    printf("Nome: %s\n", mochila[pos].nome);
                    printf("Tipo: %s\n", mochila[pos].tipo);
                    printf("Quantidade: %d\n", mochila[pos].quantidade);
                    printf("Prioridade: %d\n", mochila[pos].prioridade);
                }
                break;
            }

            case 0:
                printf("Saindo do Nível Mestre. Bom jogo!\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
