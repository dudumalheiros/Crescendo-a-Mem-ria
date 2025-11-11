#include "memoria.h"
#include "raylib.h"
#include <stdio.h>
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
        vetor[i].coordX = rand() % 700 + 50;
        vetor[i].coordY = rand() % 400 + 100;
        vetor[i].encontrada = 0;
    }
}

void desenharMemorias(Memoria vetor[], int n) {
    for (int i = 0; i < n; i++) {
        if (vetor[i].encontrada) {
            DrawRectangle(vetor[i].coordX, vetor[i].coordY, 40, 40, BLUE);
            DrawText(vetor[i].nome, vetor[i].coordX - 20, vetor[i].coordY - 15, 10, BLACK);
        }
    }
}

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

void atualizarTela() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
}
