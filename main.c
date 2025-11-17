#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdio.h>
#include <math.h>

#define LARGURA 900
#define ALTURA 600
#define TAMANHO 8
#define FASES_TOTAIS 3

typedef enum {
    MENU,
    FASE,
    FINAL
} EstadoJogo;

int main() {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    SetTargetFPS(60);

    EstadoJogo estado = MENU;
    int faseAtual = 1;
    int tipoAlgoritmo = 0;
    Vector2 jogador = {100, 100};
    float velocidade = 3.0f;
    bool jogoAtivo = true;

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);
    int memRestauradas = 0;

    int opcaoMenu = 0; // 0 = Jogar, 1 = Sair

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estado == MENU) {
            // ===== MENU PRINCIPAL =====
            const char *titulo = "Crescendo na Memoria";
            const char *subtitulo = "Pique-Esconde dos Algoritmos";
            int tL = MeasureText(titulo, 40);
            int sL = MeasureText(subtitulo, 20);
            DrawText(titulo, (LARGURA - tL)/2, 140, 40, DARKBLUE);
            DrawText(subtitulo, (LARGURA - sL)/2, 190, 20, BLUE);

            DrawText("Pressione ENTER para comecar", (LARGURA - 300)/2, 300, 20, DARKGRAY);
            DrawText("Use as setas para escolher", (LARGURA - 280)/2, 330, 18, GRAY);

            Color corJogar = (opcaoMenu == 0) ? DARKBLUE : GRAY;
            Color corSair = (opcaoMenu == 1) ? DARKBLUE : GRAY;
            DrawText("Jogar", (LARGURA - 60)/2, 400, 24, corJogar);
            DrawText("Sair", (LARGURA - 35)/2, 440, 24, corSair);

            if (IsKeyPressed(KEY_DOWN)) opcaoMenu = (opcaoMenu + 1) % 2;
            if (IsKeyPressed(KEY_UP)) opcaoMenu = (opcaoMenu - 1 + 2) % 2;

            if (IsKeyPressed(KEY_ENTER)) {
                if (opcaoMenu == 0) {
                    estado = FASE;
                    faseAtual = 1;
                    memRestauradas = 0;
                    criarMemorias(vetor, TAMANHO);
                } else {
                    break;
                }
            }
        }

        else if (estado == FASE) {
            // ===== JOGO EM ANDAMENTO =====
            DrawText(TextFormat("Fase %d / %d", faseAtual, FASES_TOTAIS), 20, 20, 20, DARKBLUE);

            // HUD
            DrawRectangle(0, ALTURA - 80, LARGURA, 80, Fade(LIGHTGRAY, 0.3f));
            DrawText("[1] Bubble  [2] Selection  [3] Quick  [I] IA ordena", 20, ALTURA - 65, 18, DARKBLUE);
            DrawText("Use as setas para mover e [ESPACO] para restaurar memorias", 20, ALTURA - 40, 18, DARKGRAY);

            // Movimento do jogador
            if (IsKeyDown(KEY_RIGHT)) jogador.x += velocidade;
            if (IsKeyDown(KEY_LEFT)) jogador.x -= velocidade;
            if (IsKeyDown(KEY_UP)) jogador.y -= velocidade;
            if (IsKeyDown(KEY_DOWN)) jogador.y += velocidade;

            if (jogador.x < 0) jogador.x = 0;
            if (jogador.x > LARGURA) jogador.x = LARGURA;
            if (jogador.y < 0) jogador.y = 0;
            if (jogador.y > ALTURA) jogador.y = ALTURA;

            DrawCircleV(jogador, 12, DARKBLUE);

            // Desenhar memórias
            for (int i = 0; i < TAMANHO; i++) {
                Rectangle r = {vetor[i].coordX, vetor[i].coordY, 40, 40};
                Color cor;

                if (vetor[i].encontrada)
                    cor = GREEN;
                else {
                    float brilho = (sin(GetTime() * 2 + i) + 1) / 2;
                    cor = Fade(SKYBLUE, 0.5f + 0.3f * brilho);
                }

                DrawRectangleRec(r, cor);

                if (vetor[i].encontrada)
                    DrawText(vetor[i].nome, vetor[i].coordX - 10, vetor[i].coordY - 15, 10, DARKGREEN);

                // Interação com memórias
                if (CheckCollisionCircleRec(jogador, 12, r) && !vetor[i].encontrada) {
                    DrawText("Pressione [ESPACO] para restaurar memoria", 220, ALTURA - 100, 18, RED);
                    if (IsKeyPressed(KEY_SPACE)) {
                        vetor[i].encontrada = 1;
                        memRestauradas++;
                    }
                }
            }

            // Algoritmos
            if (IsKeyPressed(KEY_ONE)) tipoAlgoritmo = 1;
            if (IsKeyPressed(KEY_TWO)) tipoAlgoritmo = 2;
            if (IsKeyPressed(KEY_THREE)) tipoAlgoritmo = 3;
            if (IsKeyPressed(KEY_I)) tipoAlgoritmo = 9;

            if (tipoAlgoritmo == 1 || tipoAlgoritmo == 2 || tipoAlgoritmo == 3)
                ordenarMemorias(vetor, TAMANHO, tipoAlgoritmo);
            else if (tipoAlgoritmo == 9)
                ordenarIA(vetor, TAMANHO);

            // Próxima fase
            if (memRestauradas >= TAMANHO) {
                faseAtual++;
                if (faseAtual > FASES_TOTAIS) {
                    estado = FINAL;
                } else {
                    criarMemorias(vetor, TAMANHO);
                    memRestauradas = 0;
                    jogador = (Vector2){100, 100};
                }
            }
        }

        else if (estado == FINAL) {
            const char *msg = "Tesouro Afetivo Encontrado!";
            int l = MeasureText(msg, 30);
            DrawText(msg, (LARGURA - l)/2, 200, 30, DARKBLUE);

            DrawText("Pressione ENTER para jogar novamente ou ESC para sair", 150, 300, 20, DARKGRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                estado = MENU;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
