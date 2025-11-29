#include <stdio.h>
#include <string.h>

#define MAX_ITENS 10

// Estrutura que representa cada item da mochila
struct Item {
    char nome[30];
    char tipo[20];
    int quantidade;
};

// Função rápida pra remover o \n do fgets
void tira_enter(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

int main() {

    struct Item mochila[MAX_ITENS];
    int total = 0;  // quantidade atual de itens
    int op;

    printf("=== Mochila do Sobrevivente - Nivel Novato ===\n");

    do {
        printf("\n1 - Adicionar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);
        getchar();   // limpa o \n do scanf

        switch(op) {

            // ------------------- ADICIONAR -------------------
            case 1: {
                if (total >= MAX_ITENS) {
                    printf("A mochila ja esta cheia.\n");
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
                getchar(); // limpa \n

                if (mochila[total].quantidade <= 0) {
                    printf("Quantidade invalida.\n");
                } else {
                    total++;
                    printf("Item adicionado!\n");
                }
                break;
            }

            // ------------------- REMOVER -------------------
            case 2: {
                if (total == 0) {
                    printf("Mochila vazia.\n");
                    break;
                }

                char nomeRemover[30];
                int pos = -1;

                printf("Nome do item a remover: ");
                fgets(nomeRemover, 30, stdin);
                tira_enter(nomeRemover);

                // busca simples
                for (int i = 0; i < total; i++) {
                    if (strcmp(mochila[i].nome, nomeRemover) == 0) {
                        pos = i;
                        break;
                    }
                }

                if (pos == -1) {
                    printf("Item nao encontrado.\n");
                } else {
                    // desloca tudo para esquerda
                    for (int i = pos; i < total - 1; i++) {
                        mochila[i] = mochila[i + 1];
                    }
                    total--;
                    printf("Item removido.\n");
                }
                break;
            }

            // ------------------- LISTAR -------------------
            case 3:
                if (total == 0) {
                    printf("Nenhum item na mochila.\n");
                } else {
                    printf("\n--- Itens na mochila (%d/%d) ---\n", total, MAX_ITENS);
                    for (int i = 0; i < total; i++) {
                        printf("%d) %s | %s | x%d\n",
                            i + 1,
                            mochila[i].nome,
                            mochila[i].tipo,
                            mochila[i].quantidade);
                    }
                }
                break;

            // ------------------- SAIR -------------------
            case 0:
                printf("Saindo... boa sorte na ilha!\n");
                break;

            default:
                printf("Opcao invalida.\n");
        }

    } while (op != 0);

    return 0;
}

