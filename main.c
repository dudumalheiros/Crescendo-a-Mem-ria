#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdio.h>

#define LARGURA 900
#define ALTURA 600
#define TAMANHO 8

int main() {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    SetTargetFPS(60);

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);

    Vector2 jogador = {100, 100};
    float velocidade = 3.0;
    int tipoAlgoritmo = 0;
    bool jogoAtivo = true;
    int memRestauradas = 0;

    while (!WindowShouldClose()) {
        atualizarTela();

        DrawText("Use as setas para se mover", 20, 20, 20, DARKGRAY);
        DrawText("Pressione ESPACO perto de uma memoria para ordena-la", 20, 50, 18, DARKGRAY);
        DrawText("[1] Bubble  [2] Selection  [3] Quick  [I] IA ordena", 20, 80, 18, DARKGRAY);

        // Movimento
        if (IsKeyDown(KEY_RIGHT)) jogador.x += velocidade;
        if (IsKeyDown(KEY_LEFT)) jogador.x -= velocidade;
        if (IsKeyDown(KEY_UP)) jogador.y -= velocidade;
        if (IsKeyDown(KEY_DOWN)) jogador.y += velocidade;

        DrawCircleV(jogador, 12, DARKBLUE);
        desenharMemorias(vetor, TAMANHO);

        // Interação com memórias
        for (int i = 0; i < TAMANHO; i++) {
            Rectangle memoriaRect = {vetor[i].coordX, vetor[i].coordY, 40, 40};
            if (!vetor[i].encontrada && CheckCollisionCircleRec(jogador, 12, memoriaRect)) {
                DrawText("Pressione [SPACE] para restaurar memoria!", 250, 550, 20, RED);

                if (IsKeyPressed(KEY_SPACE)) {
                    vetor[i].encontrada = 1;
                    tipoAlgoritmo = 0;
                }
            }
        }

        // Quando jogador encontra uma memória
        for (int i = 0; i < TAMANHO; i++) {
            if (vetor[i].encontrada) {
                if (IsKeyPressed(KEY_ONE)) tipoAlgoritmo = 1;
                if (IsKeyPressed(KEY_TWO)) tipoAlgoritmo = 2;
                if (IsKeyPressed(KEY_THREE)) tipoAlgoritmo = 3;
                if (IsKeyPressed(KEY_I)) tipoAlgoritmo = 9;

                if (tipoAlgoritmo == 1 || tipoAlgoritmo == 2 || tipoAlgoritmo == 3) {
                    ordenarMemorias(vetor, TAMANHO, tipoAlgoritmo);
                    memRestauradas++;
                } else if (tipoAlgoritmo == 9) {
                    ordenarIA(vetor, TAMANHO);
                    memRestauradas++;
                }

                verificarOrdem(vetor, TAMANHO);
            }
        }

        if (memRestauradas >= TAMANHO) {
            DrawText("Voce reuniu todas as lembrancas!", 250, 260, 30, GOLD);
            DrawText("Tesouro Afetivo encontrado!", 270, 300, 25, DARKBLUE);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
