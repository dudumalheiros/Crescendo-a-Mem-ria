#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdio.h>
#include <math.h>

#define LARGURA 900
#define ALTURA 600
#define TAMANHO 5
#define FASES_TOTAIS 3

typedef enum {
    MENU,
    FASE,
    FINAL
} EstadoJogo;

// Detecta colisão entre o jogador e paredes
bool colisaoComLabirinto(Vector2 jogador, Rectangle *paredes, int qtd) {
    for (int i = 0; i < qtd; i++) {
        if (CheckCollisionCircleRec(jogador, 12, paredes[i])) return true;
    }
    return false;
}

// Desenha as paredes do labirinto
void desenharLabirinto(Rectangle *paredes, int qtd) {
    for (int i = 0; i < qtd; i++) {
        DrawRectangleRec(paredes[i], Fade(DARKGRAY, 0.95f));
        DrawRectangleLinesEx(paredes[i], 2, BLACK);
    }
}

int main() {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    SetTargetFPS(60);

    EstadoJogo estado = MENU;
    int faseAtual = 1;
    int tipoAlgoritmo = 0;
    Vector2 jogador = {120, 130}; // começa dentro do labirinto
    float velocidade = 3.0f;
    int memRestauradas = 0;

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);

    int opcaoMenu = 0;

    // ===== LABIRINTOS =====

 // ======== FASE 1 - Simples, limpa e com passagem central aberta ========
Rectangle lab1[] = {
    {50, 100, 800, 20},                            // topo
    {50, 500, 300, 20}, {550, 500, 300, 20},       // base com abertura no meio
    {50, 100, 20, 420}, {830, 100, 20, 420},       // laterais

    // paredes internas (retângulo com vão no centro)
    {250, 200, 400, 20},                           // topo interno
    {250, 350, 180, 20},                           // base interna esquerda
    {470, 350, 180, 20},                           // base interna direita
    {250, 200, 20, 150},                           // lateral esquerda interna
    {630, 200, 20, 150}                            // lateral direita interna
};
int qtdLab1 = 10;



 // FASE 2 - revisada com passagens centrais e laterais
Rectangle lab2[] = {
    {50, 100, 800, 20},                           // topo
    {50, 500, 300, 20}, {550, 500, 300, 20},      // base com porta
    {50, 100, 20, 420}, {830, 100, 20, 420},      // laterais

    // paredes internas principais
    {150, 150, 600, 20},                          // faixa superior
    {150, 450, 600, 20},                          // faixa inferior

    // colunas verticais com aberturas no meio
    {150, 150, 20, 120}, {150, 330, 20, 120},     // esquerda com passagem central
    {730, 150, 20, 120}, {730, 330, 20, 120},     // direita com passagem central

    // cruz central em H
    {300, 250, 300, 20},                          // barra superior do H
    {300, 350, 300, 20},                          // barra inferior do H
    {450, 250, 20, 120},                          // linha vertical do H

    // pequenos detalhes para cantos e variação
    {600, 200, 20, 100}, {250, 300, 20, 100}
};
int qtdLab2 = 16;

// ======== FASE 3 - Versão final com entrada central inferior ========
Rectangle lab3[] = {
    // Estrutura externa
    {50, 80, 800, 20},                             // topo
    {50, 520, 300, 20}, {550, 520, 300, 20},       // base com porta central
    {50, 80, 20, 460}, {830, 80, 20, 460},         // laterais externas

    // Estrutura interna (templo)
    {200, 180, 500, 20},                           // topo interno
    {200, 420, 200, 20}, {500, 420, 200, 20},      // base interna com VÃO CENTRAL (porta!)
    {200, 180, 20, 240},                           // lateral esquerda
    {680, 180, 20, 240},                           // lateral direita

    // Aberturas adicionais (fluxo interno)
    {430, 180, 60, 20},                            // abertura no topo central
    {200, 300, 20, 40},                            // abertura esquerda
    {680, 300, 20, 40},                            // abertura direita

    // Passagem interna horizontal
    {340, 300, 280, 20}                            // corredor central
};
int qtdLab3 = 13;




    // ===== LOOP PRINCIPAL =====
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // ===== MENU =====
        if (estado == MENU) {
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
                    jogador = (Vector2){120, 130};
                } else break;
            }
        }

        // ===== FASE =====
        else if (estado == FASE) {
            DrawText(TextFormat("Fase %d / %d", faseAtual, FASES_TOTAIS), 20, 20, 20, DARKBLUE);

            Rectangle *lab;
            int qtdLab = 0;
            if (faseAtual == 1) { lab = lab1; qtdLab = qtdLab1; }
            else if (faseAtual == 2) { lab = lab2; qtdLab = qtdLab2; }
            else { lab = lab3; qtdLab = qtdLab3; }

            desenharLabirinto(lab, qtdLab);

            Vector2 novoPos = jogador;
            if (IsKeyDown(KEY_RIGHT)) novoPos.x += velocidade;
            if (IsKeyDown(KEY_LEFT)) novoPos.x -= velocidade;
            if (IsKeyDown(KEY_UP)) novoPos.y -= velocidade;
            if (IsKeyDown(KEY_DOWN)) novoPos.y += velocidade;

            if (!colisaoComLabirinto(novoPos, lab, qtdLab)) jogador = novoPos;

            DrawCircleV(jogador, 12, DARKBLUE);

            for (int i = 0; i < TAMANHO; i++) {
                Rectangle r = {vetor[i].coordX, vetor[i].coordY, 40, 40};
                Color cor = vetor[i].encontrada ? GREEN : Fade(SKYBLUE, 0.5f + 0.3f * sin(GetTime() + i));
                DrawRectangleRec(r, cor);

                if (CheckCollisionCircleRec(jogador, 12, r) && !vetor[i].encontrada) {
                    DrawText("Pressione [ESPACO] para restaurar memoria", 220, ALTURA - 100, 18, RED);
                    if (IsKeyPressed(KEY_SPACE)) { vetor[i].encontrada = 1; memRestauradas++; }
                }
            }

            if (IsKeyPressed(KEY_ONE)) tipoAlgoritmo = 1;
            if (IsKeyPressed(KEY_TWO)) tipoAlgoritmo = 2;
            if (IsKeyPressed(KEY_THREE)) tipoAlgoritmo = 3;
            if (IsKeyPressed(KEY_I)) tipoAlgoritmo = 9;

            if (tipoAlgoritmo == 1 || tipoAlgoritmo == 2 || tipoAlgoritmo == 3) ordenarMemorias(vetor, TAMANHO, tipoAlgoritmo);
            else if (tipoAlgoritmo == 9) ordenarIA(vetor, TAMANHO);

            if (memRestauradas >= TAMANHO) {
                faseAtual++;
                if (faseAtual > FASES_TOTAIS) estado = FINAL;
                else {
                    criarMemorias(vetor, TAMANHO);
                    memRestauradas = 0;
                    jogador = (Vector2){120, 130};
                }
            }

            DrawText("[1] Bubble  [2] Selection  [3] Quick  [I] IA ordena", 20, ALTURA - 60, 18, DARKBLUE);
        }

        // ===== TELA FINAL =====
        else if (estado == FINAL) {
            const char *msg = "Tesouro Afetivo Encontrado!";
            int l = MeasureText(msg, 30);
            DrawText(msg, (LARGURA - l)/2, 200, 30, DARKBLUE);
            DrawText("Pressione ENTER para jogar novamente ou ESC para sair", 150, 300, 20, DARKGRAY);

            if (IsKeyPressed(KEY_ENTER)) estado = MENU;
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
