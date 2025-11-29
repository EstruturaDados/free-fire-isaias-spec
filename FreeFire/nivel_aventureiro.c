#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10

// Estrutura básica do item da mochila
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

// Função pra tirar o '\n' que o fgets pega
void tira_enter(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

// Busca sequencial por nome (retorna índice ou -1)
int buscar_item(struct Item mochila[], int total, const char *nome) {
    for (int i = 0; i < total; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            return i;
        }
    }
    return -1;
}

int main() {

    struct Item mochila[MAX_ITENS];
    int total = 0;
    int op;

    printf("=== Nível Aventureiro - Mochila com Busca ===\n");

    do {
        printf("\n1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item por nome\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar(); // limpa o \n do scanf

        switch (op) {

            // ---------------- ADICIONAR ----------------
            case 1: {
                if (total >= MAX_ITENS) {
                    printf("A mochila está cheia.\n");
                    break;
                }

                printf("Nome do item: ");
                fgets(mochila[total].nome, 30, stdin);
                tira_enter(mochila[total].nome);

                printf("Tipo do item (arma, municao, cura...): ");
                fgets(mochila[total].tipo, 20, stdin);
                tira_enter(mochila[total].tipo);

                printf("Quantidade: ");
                scanf("%d", &mochila[total].quantidade);
                getchar();

                if (mochila[total].quantidade <= 0) {
                    printf("Quantidade inválida.\n");
                } else {
                    total++;
                    printf("Item adicionado com sucesso!\n");
                }
                break;
            }

            // ---------------- REMOVER ----------------
            case 2: {
                if (total == 0) {
                    printf("A mochila está vazia.\n");
                    break;
                }

                char nomeRemover[30];
                printf("Nome do item a remover: ");
                fgets(nomeRemover, 30, stdin);
                tira_enter(nomeRemover);

                int pos = buscar_item(mochila, total, nomeRemover);

                if (pos == -1) {
                    printf("Item não encontrado.\n");
                } else {
                    for (int i = pos; i < total - 1; i++) {
                        mochila[i] = mochila[i + 1];
                    }
                    total--;
                    printf("Item removido!\n");
                }
                break;
            }

            // ---------------- LISTAR ----------------
            case 3: {
                if (total == 0) {
                    printf("Nenhum item na mochila.\n");
                } else {
                    printf("\n--- Itens na mochila ---\n");
                    for (int i = 0; i < total; i++) {
                        printf("%d) %s | %s | x%d\n",
                            i + 1,
                            mochila[i].nome,
                            mochila[i].tipo,
                            mochila[i].quantidade);
                    }
                }
                break;
            }

            // ---------------- BUSCAR ----------------
            case 4: {
                if (total == 0) {
                    printf("Mochila vazia, nada para buscar.\n");
                    break;
                }

                char nomeBusca[30];
                printf("Digite o nome do item para buscar: ");
                fgets(nomeBusca, 30, stdin);
                tira_enter(nomeBusca);

                int pos = buscar_item(mochila, total, nomeBusca);

                if (pos == -1) {
                    printf("Item '%s' não foi encontrado.\n", nomeBusca);
                } else {
                    printf("\nItem encontrado:\n");
                    printf("Nome: %s\n", mochila[pos].nome);
                    printf("Tipo: %s\n", mochila[pos].tipo);
                    printf("Quantidade: %d\n", mochila[pos].quantidade);
                }
                break;
            }

            // ---------------- SAIR ----------------
            case 0:
                printf("Saindo do Nível Aventureiro...\n");
                break;

            default:
                printf("Opção inválida.\n");
        }

    } while (op != 0);

    return 0;
}
