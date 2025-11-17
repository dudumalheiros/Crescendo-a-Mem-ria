#include "memoria.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void criarMemorias(Memoria vetor[], int n) {
    // lista de memórias, sem repetir dentro da fase
    const char *nomes[] = {
        "A primeira montanha-russa ou brinquedo radical",
        "Jogar videogame ate de madrugada com um(a) amigo(a)",
        "Brincar com o primeiro bichinho de estimacao",
        "Presentes de Natal",
        "Ficar acordado ate tarde vendo filme com a familia",
        "Esconder-se em uma cabana de lencois na sala",
        "Interclasse do colegio",
        "O primeiro dia de aula",
        "A primeira vez em um show",
        "A formatura do ABC",
        "A formatura do Ensino Medio",
        "A festa de aniversario com o tema favorito",
        "A primeira vez que dormiu na casa de um(a) amigo(a)",
        "Primeira queda de bicicleta",
        "Primeiro filme de terror",
        "Parque aquatico na infancia",
        "Primeiro beijo",
        "Primeira viagem com os(as) amigos(as)"
    };

    int totalNomes = (int)(sizeof(nomes) / sizeof(nomes[0]));
    if (n > totalNomes) n = totalNomes; // seguranca

    // inicializa RNG uma vez por chamada
    srand((unsigned)time(NULL));

    // embaralha índices (Fisher–Yates)
    int indices[32];
    for (int i = 0; i < totalNomes; i++) indices[i] = i;

    for (int i = totalNomes - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    // escolhe os n primeiros índices embaralhados (sem repeticao)
    for (int i = 0; i < n; i++) {
        int idx = indices[i];

        // copia com limite pra nao estourar o vetor
        strncpy(vetor[i].nome, nomes[idx], sizeof(vetor[i].nome) - 1);
        vetor[i].nome[sizeof(vetor[i].nome) - 1] = '\0';

        vetor[i].valorAfeto = 0;              // o jogador define depois
        vetor[i].coordX = rand() % 700 + 80;
        vetor[i].coordY = rand() % 380 + 120;
        vetor[i].encontrada = 0;
    }
}

void desenharMemorias(Memoria vetor[], int n) {
    for (int i = 0; i < n; i++) {
        Color cor = vetor[i].encontrada ? BLUE : SKYBLUE;
        DrawRectangle(vetor[i].coordX, vetor[i].coordY, 40, 40, cor);

        // **NOME SÓ APARECE DEPOIS QUE A MEMÓRIA FOI ENCONTRADA**
        if (vetor[i].encontrada) {
            DrawText(vetor[i].nome,
                     vetor[i].coordX - 20, vetor[i].coordY - 15,
                     10, RAYWHITE);
        }
    }
}

// mantida para compatibilidade, se ainda for usada em algum lugar
void verificarOrdem(Memoria vetor[], int n) {
    int ordenado = 1;
    for (int i = 0; i < n - 1; i++) {
        if (vetor[i].valorAfeto > vetor[i + 1].valorAfeto) {
            ordenado = 0;
            break;
        }
    }

    if (ordenado)
        DrawText("Memorias ordenadas! Tesouro desbloqueado!", 150, 40, 20, GREEN);
    else
        DrawText("Ainda baguncadas... continue tentando!", 200, 40, 20, RED);
}
