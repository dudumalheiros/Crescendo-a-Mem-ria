#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define LARGURA 900
#define ALTURA 600
#define TAMANHO 5

typedef enum { MENU, FASE, FIM } EstadoJogo;

int main(void) {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    InitAudioDevice();
    SetTargetFPS(60);

    // === Trilhas sonoras ===
    Music musicaMenu = LoadMusicStream("assets/menu_music.mp3");
    Music musicaJogo = LoadMusicStream("assets/game_music.mp3");
    PlayMusicStream(musicaMenu);

    EstadoJogo estado = MENU;
    int faseAtual = 1;
    bool jogoAtivo = true;

    Vector2 jogador = {150, 450};
    float velocidade = 3.0f;

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);

    while (!WindowShouldClose() && jogoAtivo) {
        if (estado == MENU) UpdateMusicStream(musicaMenu);
        else UpdateMusicStream(musicaJogo);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // =================== MENU ===================
        if (estado == MENU) {
            DrawText("Crescendo na Memoria", 230, 150, 42, DARKBLUE);
            DrawText("Pique-Esconde dos Algoritmos", 240, 210, 22, GRAY);
            DrawText("Pressione [ENTER] para iniciar", 300, 350, 20, DARKGRAY);
            DrawText("Use as setas para se mover durante o jogo", 260, 390, 18, DARKGRAY);

            float brilho = (sinf(GetTime() * 2) + 1) / 2;
            DrawCircle(450, 450, 15 + brilho * 5, Fade(SKYBLUE, 0.8f));

            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaJogo);
                estado = FASE;
                faseAtual = 1;
                jogador = (Vector2){150, 450};
                criarMemorias(vetor, TAMANHO);
            }
        }

        // =================== FASES ===================
        else if (estado == FASE) {
            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 22, DARKBLUE);
            DrawText("[1] Bubble  [2] Selection  [3] Quick  [I] IA ordena", 20, 550, 18, BLUE);
            DrawText("Use as setas para mover e [ESPACO] para restaurar memorias", 20, 575, 18, DARKGRAY);

            Vector2 anterior = jogador;

            // Movimento do jogador
            if (IsKeyDown(KEY_RIGHT)) jogador.x += velocidade;
            if (IsKeyDown(KEY_LEFT)) jogador.x -= velocidade;
            if (IsKeyDown(KEY_UP)) jogador.y -= velocidade;
            if (IsKeyDown(KEY_DOWN)) jogador.y += velocidade;

            if (jogador.x < 0) jogador.x = 0;
            if (jogador.x > LARGURA) jogador.x = LARGURA;
            if (jogador.y < 0) jogador.y = 0;
            if (jogador.y > ALTURA) jogador.y = ALTURA;

            Rectangle *lab = NULL;
            int qtdLab = 0;

            // ===== FASE 1 =====
            if (faseAtual == 1) {
                static Rectangle lab1[] = {
                    {50, 100, 800, 20}, {50, 500, 300, 20}, {550, 500, 300, 20},
                    {50, 100, 20, 420}, {830, 100, 20, 420},
                    {250, 200, 400, 20}, {250, 350, 180, 20},
                    {470, 350, 180, 20}, {250, 200, 20, 150}, {630, 200, 20, 150}
                };
                lab = lab1; qtdLab = 10;
            }

            // ===== FASE 2 (corrigida com passagens abertas) =====
            else if (faseAtual == 2) {
                static Rectangle lab2[] = {
                    // Contorno externo
                    {60, 100, 780, 20}, {60, 500, 780, 20},
                    {60, 100, 20, 420}, {820, 100, 20, 420},

                    // Estrutura interna (com passagens abertas)
                    {150, 200, 600, 20},   // parede superior interna
                    {150, 350, 600, 20},   // parede inferior interna
                    {150, 200, 20, 150},   // lateral esquerda interna
                    {730, 250, 20, 100},   // lateral direita (passagens superior e inferior)
                    {410, 200, 20, 80},    // pilar superior central
                    {410, 320, 20, 50},    // pilar inferior central (abertura entre 280–320)
                    {300, 350, 300, 20},   // linha horizontal mais abaixo
                    {300, 380, 20, 120},   // coluna esquerda abaixo
                    {580, 380, 20, 120}    // coluna direita abaixo
                };
                lab = lab2; qtdLab = 10;
            }

            // ===== FASE 3 =====
            else if (faseAtual == 3) {
                static Rectangle lab3[] = {
                    {50, 80, 800, 20}, {50, 520, 300, 20}, {550, 520, 300, 20},
                    {50, 80, 20, 460}, {830, 80, 20, 460},
                    {200, 180, 500, 20}, {200, 420, 200, 20}, {500, 420, 200, 20},
                    {200, 180, 20, 240}, {680, 180, 20, 240},
                    {430, 180, 60, 20}, {430, 420, 60, 20},
                    {340, 300, 280, 20}
                };
                lab = lab3; qtdLab = 13;
            }

            // Colisão com paredes
            for (int i = 0; i < qtdLab; i++) {
                DrawRectangleRec(lab[i], DARKGRAY);
                if (CheckCollisionCircleRec(jogador, 12, lab[i])) jogador = anterior;
            }

            // Jogador
            DrawCircleV(jogador, 12, DARKBLUE);

            // Memórias
            desenharMemorias(vetor, TAMANHO);

            // Interação com memórias
            for (int i = 0; i < TAMANHO; i++) {
                Rectangle r = {vetor[i].coordX, vetor[i].coordY, 40, 40};
                if (!vetor[i].encontrada && CheckCollisionCircleRec(jogador, 12, r)) {
                    DrawText("Pressione [ESPACO] para restaurar memoria", 250, 50, 20, DARKBLUE);
                    if (IsKeyPressed(KEY_SPACE)) {
                        vetor[i].encontrada = true;
                        ordenarMemorias(vetor, TAMANHO, 3);
                    }
                }
            }

            // Passar de fase
            bool todas = true;
            for (int i = 0; i < TAMANHO; i++)
                if (!vetor[i].encontrada) todas = false;

            if (todas) {
                if (faseAtual < 3) {
                    faseAtual++;
                    criarMemorias(vetor, TAMANHO);
                    jogador = (Vector2){150, 450};
                } else estado = FIM;
            }
        }

        // =================== FINAL ===================
        else if (estado == FIM) {
            DrawText("Parabens!", 380, 220, 40, DARKBLUE);
            DrawText("Voce restaurou todas as memorias!", 250, 280, 24, DARKGRAY);
            DrawText("Pressione [ENTER] para jogar novamente", 260, 340, 20, GRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(musicaJogo);
                PlayMusicStream(musicaMenu);
                estado = MENU;
                faseAtual = 1;
                criarMemorias(vetor, TAMANHO);
            }
        }

        EndDrawing();
    }

    StopMusicStream(musicaMenu);
    StopMusicStream(musicaJogo);
    UnloadMusicStream(musicaMenu);
    UnloadMusicStream(musicaJogo);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
