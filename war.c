#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_MISSOES 5

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Remove o ENTER do final da string
void removerEnter(char *texto) {
    texto[strcspn(texto, "\n")] = '\0';
}

// Limpa o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Cadastro dos territórios
void cadastrarTerritorios(Territorio *mapa, int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        printf("\n=== Cadastro do territorio %d ===\n", i + 1);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        removerEnter(mapa[i].nome);

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        removerEnter(mapa[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBuffer();

        if (mapa[i].tropas < 1)
            mapa[i].tropas = 1;
    }
}

// Exibe o mapa atual
void exibirMapa(Territorio *mapa, int quantidade) {
    printf("\n=========== MAPA ===========\n");

    for (int i = 0; i < quantidade; i++) {
        printf("%d - %s | Cor: %s | Tropas: %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }

    printf("============================\n");
}

// Sorteia e armazena uma missão dinamicamente
void atribuirMissao(char **destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;

    *destino = malloc(strlen(missoes[indice]) + 1);

    if (*destino == NULL) {
        printf("Erro ao alocar memoria da missao.\n");
        exit(1);
    }

    strcpy(*destino, missoes[indice]);
}

// Exibe a missão do jogador
void exibirMissao(char *missao) {
    printf("\n======= SUA MISSAO =======\n");
    printf("%s\n", missao);
    printf("==========================\n");
}

// Função de ataque
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("\n%s atacou %s\n", atacante->nome, defensor->nome);
    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    int diferenca = abs(dadoAtacante - dadoDefensor);

    if (diferenca == 0)
        diferenca = 1;

    // Atacante venceu
    if (dadoAtacante > dadoDefensor) {
        printf("%s venceu o combate!\n", atacante->nome);
        printf("%s perdeu %d tropas.\n", defensor->nome, diferenca);

        defensor->tropas -= diferenca;

        // Se o defensor ficou sem tropas, o território é conquistado
        if (defensor->tropas <= 0) {
            printf("%s foi conquistado!\n", defensor->nome);

            strcpy(defensor->cor, atacante->cor);

            int tropasTransferidas = atacante->tropas / 2;

            if (tropasTransferidas < 1)
                tropasTransferidas = 1;

            // Garante que o atacante fique com ao menos 1 tropa
            if (tropasTransferidas >= atacante->tropas)
                tropasTransferidas = atacante->tropas - 1;

            atacante->tropas -= tropasTransferidas;
            defensor->tropas = tropasTransferidas;

            printf("%d tropas foram movidas para o territorio conquistado.\n",
                   tropasTransferidas);
        }
    }
    // Defensor venceu
    else if (dadoDefensor > dadoAtacante) {
        printf("%s venceu a defesa!\n", defensor->nome);
        printf("%s perdeu %d tropas.\n", atacante->nome, diferenca);

        atacante->tropas -= diferenca;

        // O atacante sempre deve ficar com pelo menos 1 tropa
        if (atacante->tropas < 1)
            atacante->tropas = 1;
    }
    // Empate
    else {
        printf("Empate! Nenhum territorio perdeu tropas.\n");
    }
}

// Verifica se a missão foi concluída
int verificarMissao(char *missao, Territorio *mapa, int tamanho) {

    if (strcmp(missao, "Conquistar 3 territorios seguidos") == 0) {

        if (tamanho < 3)
            return 0;

        for (int i = 0; i < tamanho - 2; i++) {
            if (strcmp(mapa[i].cor, mapa[i + 1].cor) == 0 &&
                strcmp(mapa[i].cor, mapa[i + 2].cor) == 0) {
                return 1;
            }
        }
    }

    if (strcmp(missao, "Eliminar todas as tropas da cor vermelha") == 0) {

        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelha") == 0 &&
                mapa[i].tropas > 0) {
                return 0;
            }
        }

        return 1;
    }

    if (strcmp(missao, "Controlar todos os territorios azuis") == 0) {

        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") != 0)
                return 0;
        }

        return 1;
    }

    if (strcmp(missao, "Conquistar pelo menos 5 territorios") == 0) {

        char corJogador[10];
        int contador = 0;

        strcpy(corJogador, mapa[0].cor);

        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0)
                contador++;
        }

        return contador >= 5;
    }

    if (strcmp(missao, "Ficar com o maior numero de tropas no mapa") == 0) {

        int total = 0;

        for (int i = 0; i < tamanho; i++) {
            total += mapa[i].tropas;
        }

        return total >= 20;
    }

    return 0;
}

// Libera memória
void liberarMemoria(Territorio *mapa, char *missaoJogador) {
    free(mapa);
    free(missaoJogador);
}

int main() {
    srand(time(NULL));

    int quantidade;

    do {
        printf("Quantos territorios deseja cadastrar? ");
        scanf("%d", &quantidade);
        limparBuffer();

    } while (quantidade < 2);

    Territorio *mapa = calloc(quantidade, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar memoria do mapa.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, quantidade);

    char *missoes[MAX_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar todos os territorios azuis",
        "Conquistar pelo menos 5 territorios",
        "Ficar com o maior numero de tropas no mapa"
    };

    char *missaoJogador = NULL;

    atribuirMissao(&missaoJogador, missoes, MAX_MISSOES);

    // Mostra a missão apenas uma vez no início
    exibirMissao(missaoJogador);

    int venceu = 0;
    int opcao = 1;

    while (!venceu && opcao != 0) {

        exibirMapa(mapa, quantidade);

        int atacante, defensor;

        printf("\nEscolha o territorio atacante: ");
        scanf("%d", &atacante);

        printf("Escolha o territorio defensor: ");
        scanf("%d", &defensor);
        limparBuffer();

        atacante--;
        defensor--;

        // Validações
        if (atacante < 0 || atacante >= quantidade ||
            defensor < 0 || defensor >= quantidade) {
            printf("Territorio invalido.\n");
            continue;
        }

        if (atacante == defensor) {
            printf("Um territorio nao pode atacar a si mesmo.\n");
            continue;
        }

        if (mapa[atacante].tropas <= 1) {
            printf("O territorio atacante precisa ter mais de 1 tropa.\n");
            continue;
        }

        if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {
            printf("Voce nao pode atacar um territorio da mesma cor.\n");
            continue;
        }

        atacar(&mapa[atacante], &mapa[defensor]);

        // Mostra o mapa atualizado
        printf("\n===== MAPA ATUALIZADO =====\n");
        exibirMapa(mapa, quantidade);

        // Verifica silenciosamente a missão
        if (verificarMissao(missaoJogador, mapa, quantidade)) {
            printf("\n====================================\n");
            printf("MISSAO CUMPRIDA!\n");
            printf("Voce venceu o jogo!\n");
            printf("Missao: %s\n", missaoJogador);
            printf("====================================\n");

            venceu = 1;
            break;
        }

        // Menu após cada ataque
        while (1) {
            printf("\nO que deseja fazer agora?\n");
            printf("1 - Atacar novamente\n");
            printf("2 - Ver minha missao\n");
            printf("0 - Sair\n");
            printf("Opcao: ");
            scanf("%d", &opcao);
            limparBuffer();

            if (opcao == 1) {
                break;
            }
            else if (opcao == 2) {
                exibirMissao(missaoJogador);

                printf("\n===== MAPA ATUAL =====\n");
                exibirMapa(mapa, quantidade);
            }
            else if (opcao == 0) {
                break;
            }
            else {
                printf("Opcao invalida!\n");
            }
        }
    }

    printf("\nEncerrando o jogo...\n");

    liberarMemoria(mapa, missaoJogador);

    return 0;
}
