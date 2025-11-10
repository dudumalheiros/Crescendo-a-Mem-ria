#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef enum GameScreen { MENU, JOGO, VITORIA } GameScreen;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Crescendo na Memória");
    SetTargetFPS(60);

    GameScreen screen = MENU;
    Texture2D bg = LoadTexture("assets/background.png");
    Texture2D playerTex = LoadTexture("assets/player.png");

    // Estrutura do jogo
    Memoria vetor[10];
    int tamanho = 10;
    int algoritmoSelecionado = 0;
    Vector2 posicaoJogador = {100, 100};
    int fase = 1;

    inicializarMemorias(vetor, tamanho);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(bg, 0, 0, WHITE);

        switch (screen) {
            case MENU:
                DrawText("Crescendo na Memória", 200, 100, 40, DARKBLUE);
                DrawText("1 - Iniciar Jogo", 300, 250, 25, BLACK);
                DrawText("2 - Sair", 300, 300, 25, BLACK);

                if (IsKeyPressed(KEY_ONE)) screen = JOGO;
                if (IsKeyPressed(KEY_TWO)) CloseWindow();
                break;

            case JOGO:
                DrawText(TextFormat("Fase: %d", fase), 20, 20, 20, DARKBLUE);
                DrawText("Pressione [A] para ordenar (Bubble Sort)", 20, 60, 20, BLACK);
                DrawTexture(playerTex, posicaoJogador.x, posicaoJogador.y, WHITE);

                // Movimento simples no mapa
                if (IsKeyDown(KEY_RIGHT)) posicaoJogador.x += 4;
                if (IsKeyDown(KEY_LEFT))  posicaoJogador.x -= 4;
                if (IsKeyDown(KEY_UP))    posicaoJogador.y -= 4;
                if (IsKeyDown(KEY_DOWN))  posicaoJogador.y += 4;

                // Ordenação
                if (IsKeyPressed(KEY_A)) {
                    ordenarMemorias(vetor, tamanho, 1); // 1 = Bubble
                    DrawText("Vetor ordenado! Pista desbloqueada!", 200, 500, 20, GREEN);
                    fase++;
                    acaoIA(vetor, tamanho, 1); // IA modifica vetor para próxima fase
                }

                if (fase > 5) screen = VITORIA;
                break;

            case VITORIA:
                DrawText("Você encontrou o Tesouro Afetivo!", 150, 250, 30, GOLD);
                DrawText("Pressione ESC para sair.", 250, 300, 20, DARKGRAY);
                break;
        }

        EndDrawing();
    }

    UnloadTexture(bg);
    UnloadTexture(playerTex);
    CloseWindow();
    return 0;
}
