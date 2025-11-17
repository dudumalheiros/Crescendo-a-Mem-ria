#include "memoria.h"
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

void criarMemorias(Memoria vetor[], int n) {
    const char *nomes[] = {
        "Casa da Vovo", "Primeiro Dia de Aula", "O Gato de Rua",
        "Brincadeira na Chuva", "Aniversario", "Viagem ao Campo",
        "Primeiro Amigo", "Noite das Estrelas", "A Bicicleta Azul", "Tesouro Afetivo"
    };

    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        strcpy(vetor[i].nome, nomes[i % 10]);
        vetor[i].valorAfeto = rand() % 100;
        vetor[i].coordX = rand() % 700 + 80;
        vetor[i].coordY = rand() % 350 + 120;
        vetor[i].encontrada = 0;
    }
}

void desenharMemorias(Memoria vetor[], int n) {
    for (int i = 0; i < n; i++) {
        Color cor = vetor[i].encontrada ? BLUE : SKYBLUE;
        DrawRectangle(vetor[i].coordX, vetor[i].coordY, 40, 40, cor);
        DrawText(vetor[i].nome, vetor[i].coordX - 10, vetor[i].coordY - 20, 10, DARKGRAY);
    }
}
