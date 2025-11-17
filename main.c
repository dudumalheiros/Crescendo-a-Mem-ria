#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LARGURA 900
#define ALTURA 600
#define TAMANHO 5

typedef enum { MENU, FASE, FIM } EstadoJogo;

typedef struct {
    Rectangle paredes[20];
    int qtd;
} Labirinto;

// ---------- FUNÇÕES DE FUNDO ----------
void desenharFundoFase1(void) {
    ClearBackground((Color){135, 206, 235, 255});
    DrawCircle(780, 100, 60, YELLOW);
    DrawCircle(300, 600, 300, (Color){0, 200, 0, 255});
    DrawCircle(700, 620, 250, (Color){34, 139, 34, 255});
}

void desenharFundoFase2(void) {
    ClearBackground((Color){255, 165, 90, 255});
    DrawCircle(120, 100, 60, ORANGE);
    DrawCircle(300, 600, 300, (Color){200, 120, 50, 255});
    DrawCircle(700, 620, 250, (Color){160, 82, 45, 255});
}

void desenharFundoFase3(void) {
    ClearBackground((Color){10, 10, 30, 255});
    DrawCircle(780, 100, 50, (Color){240, 240, 180, 255});
    for (int i = 0; i < 80; i++)
        DrawPixel(rand() % LARGURA, rand() % ALTURA, WHITE);
}

// ---------- LABIRINTOS ----------
Labirinto criarLabirinto(int fase) {
    Labirinto l = {0};

    if (fase == 1) { // Fase 1 - com passagem central aberta
        Rectangle paredesTemp[] = {
            {50, 100, 800, 20},  // topo
            {50, 500, 800, 20},  // base
            {50, 100, 20, 420},  // esquerda
            {830, 100, 20, 420}, // direita

            // retângulo interno com vão no meio
            {200, 180, 500, 20},  // topo interno
            {200, 180, 20, 160},  // lateral esquerda
            {680, 180, 20, 160},  // lateral direita
            {200, 320, 190, 20},  // base esquerda
            {510, 320, 190, 20},  // base direita (abertura central)
        };
        l.qtd = sizeof(paredesTemp) / sizeof(paredesTemp[0]);
        for (int i = 0; i < l.qtd; i++) l.paredes[i] = paredesTemp[i];
    }

    else if (fase == 2) { // Fase 2 - média, com caminhos livres
        Rectangle paredesTemp[] = {
            {50, 100, 800, 20},  // topo
            {50, 500, 800, 20},  // base
            {50, 100, 20, 420},  // esquerda
            {830, 100, 20, 420}, // direita

            // blocos internos com vão lateral e meio
            {150, 200, 600, 20},
            {150, 350, 200, 20}, // base esquerda
            {550, 350, 200, 20}, // base direita
            {150, 200, 20, 150}, // lateral esquerda
            {730, 200, 20, 150}, // lateral direita
            {430, 260, 40, 20}   // pilar central curto
        };
        l.qtd = sizeof(paredesTemp) / sizeof(paredesTemp[0]);
        for (int i = 0; i < l.qtd; i++) l.paredes[i] = paredesTemp[i];
    }

   else { // Fase 3 - difícil, mas com passagens funcionais
    Rectangle paredesTemp[] = {
        // Moldura externa
        {50, 80, 800, 20},   // topo
        {50, 520, 800, 20},  // base
        {50, 80, 20, 460},   // esquerda
        {830, 80, 20, 460},  // direita

        // Estrutura interna com passagem no centro
        {200, 180, 500, 20}, // topo interno
        {200, 420, 220, 20}, // base esquerda
        {480, 420, 220, 20}, // base direita (vão central)
        {200, 180, 20, 240}, // lateral esquerda
        {680, 180, 20, 240}, // lateral direita
        {420, 280, 60, 20}   // pequena ponte no meio
    };
    l.qtd = sizeof(paredesTemp) / sizeof(paredesTemp[0]);
    for (int i = 0; i < l.qtd; i++) l.paredes[i] = paredesTemp[i];
}


    return l;
}

// ---------- PRINCIPAL ----------
int main(void) {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    InitAudioDevice();
    SetTargetFPS(60);

    Music musicaMenu = LoadMusicStream("assets/menu_music.mp3");
    Music musicaJogo = LoadMusicStream("assets/game_music.mp3");
    PlayMusicStream(musicaMenu);

    EstadoJogo estado = MENU;
    int faseAtual = 1;
    Vector2 jogador = {150, 450};
    float velocidade = 3.0f;

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);
    Labirinto lab = criarLabirinto(faseAtual);

    while (!WindowShouldClose()) {
        if (estado == MENU) UpdateMusicStream(musicaMenu);
        else UpdateMusicStream(musicaJogo);

        BeginDrawing();

        // ---------- MENU ----------
        if (estado == MENU) {
            ClearBackground((Color){135, 206, 235, 255});
            DrawCircle(780, 100, 60, YELLOW);
            DrawCircle(300, 600, 300, (Color){0, 200, 0, 255});
            DrawText("Crescendo na Memoria", 240, 140, 40, DARKBLUE);
            DrawText("Pique-Esconde dos Algoritmos", 250, 200, 20, GRAY);
            DrawText("Pressione [ENTER] para iniciar", 300, 350, 20, DARKGRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaJogo);
                estado = FASE;
                faseAtual = 1;
                jogador = (Vector2){150, 450};
                criarMemorias(vetor, TAMANHO);
                lab = criarLabirinto(faseAtual);
            }
        }

        // ---------- FASES ----------
        else if (estado == FASE) {
            if (faseAtual == 1) desenharFundoFase1();
            else if (faseAtual == 2) desenharFundoFase2();
            else desenharFundoFase3();

            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 20, DARKBLUE);
            const char *algoritmoNome = (faseAtual == 1) ? "Bubble Sort" : (faseAtual == 2) ? "Selection Sort" : "Quick Sort";
            DrawText(TextFormat("Algoritmo desta fase: %s", algoritmoNome), 20, 50, 18, BLUE);
            DrawText("Use as setas para mover e [ESPACO] para restaurar memorias", 20, 575, 18, DARKGRAY);

            Vector2 novaPos = jogador;
            if (IsKeyDown(KEY_RIGHT)) novaPos.x += velocidade;
            if (IsKeyDown(KEY_LEFT)) novaPos.x -= velocidade;
            if (IsKeyDown(KEY_UP)) novaPos.y -= velocidade;
            if (IsKeyDown(KEY_DOWN)) novaPos.y += velocidade;

            bool colidiu = false;
            for (int i = 0; i < lab.qtd; i++)
                if (CheckCollisionCircleRec(novaPos, 12, lab.paredes[i])) colidiu = true;
            if (!colidiu) jogador = novaPos;

            for (int i = 0; i < lab.qtd; i++) DrawRectangleRec(lab.paredes[i], DARKGRAY);
            DrawCircleV(jogador, 12, DARKBLUE);
            desenharMemorias(vetor, TAMANHO);

            for (int i = 0; i < TAMANHO; i++) {
                Rectangle r = {vetor[i].coordX, vetor[i].coordY, 40, 40};
                if (!vetor[i].encontrada && CheckCollisionCircleRec(jogador, 12, r)) {
                    DrawText("Pressione [ESPACO] para restaurar memoria", 250, 50, 20, DARKBLUE);
                    if (IsKeyPressed(KEY_SPACE)) vetor[i].encontrada = true;
                }
            }

            bool todas = true;
            for (int i = 0; i < TAMANHO; i++) if (!vetor[i].encontrada) todas = false;

            if (todas) {
                if (faseAtual < 3) {
                    faseAtual++;
                    criarMemorias(vetor, TAMANHO);
                    lab = criarLabirinto(faseAtual);
                    jogador = (Vector2){150, 450};
                } else estado = FIM;
            }
        }

        // ---------- FIM ----------
        else if (estado == FIM) {
            ClearBackground((Color){10, 10, 30, 255});
            DrawText("Parabens!", 380, 220, 40, SKYBLUE);
            DrawText("Voce restaurou todas as memorias!", 250, 280, 24, LIGHTGRAY);
            DrawText("Pressione [ENTER] para jogar novamente", 260, 340, 20, GRAY);
            if (IsKeyPressed(KEY_ENTER)) {
                estado = MENU;
                faseAtual = 1;
                criarMemorias(vetor, TAMANHO);
                lab = criarLabirinto(faseAtual);
                StopMusicStream(musicaJogo);
                PlayMusicStream(musicaMenu);
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
