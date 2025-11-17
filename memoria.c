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
        Rectangle r = {vetor[i].coordX, vetor[i].coordY, 40, 40};

        if (vetor[i].encontrada) {
            DrawRectangleRec(r, BLUE);
            DrawRectangleLinesEx(r, 2, DARKBLUE);
            int fs = 10;
            int w = MeasureText(vetor[i].nome, fs);
            int nx = (int)(r.x + (r.width - w)/2);
            int ny = (int)(r.y - 14);
            DrawText(vetor[i].nome, nx, ny, fs, BLACK);
        } else {
            DrawRectangleRec(r, (Color){100, 180, 255, 255}); // azul fixo
            DrawRectangleLinesEx(r, 2, DARKBLUE);
            int fs = 22;
            int w = MeasureText("?", fs);
            int qx = (int)(r.x + (r.width - w)/2);
            int qy = (int)(r.y + (r.height - fs)/2);
            DrawText("?", qx, qy, fs, DARKBLUE);
        }
    }
}
