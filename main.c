#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_W 1280
#define SCREEN_H 720

typedef enum {
    FASE_1_BUBBLE = 1,
    FASE_2_SELECTION,
    FASE_3_INSERTION,
    FASE_4_QUICK,
    FASE_5_MERGE,
    FIM
} Fase;

static Color FundoDaFase(int fase) {
    switch (fase) {
        case FASE_1_BUBBLE:    return (Color){240,240,240,255}; // cinza claro
        case FASE_2_SELECTION: return (Color){220,235,255,255}; // azul claro
        case FASE_3_INSERTION: return (Color){220,255,230,255}; // verde claro
        case FASE_4_QUICK:     return (Color){255,235,210,255}; // laranja claro
        case FASE_5_MERGE:     return (Color){240,225,255,255}; // roxo claro
        default:               return RAYWHITE;
    }
}

static const char* NomeAlgoritmoDaFase(int fase) {
    switch (fase) {
        case FASE_1_BUBBLE:    return "Bubble Sort";
        case FASE_2_SELECTION: return "Selection Sort";
        case FASE_3_INSERTION: return "Insertion Sort";
        case FASE_4_QUICK:     return "Quick Sort";
        case FASE_5_MERGE:     return "Merge Sort";
        default:               return "—";
    }
}

static KeyboardKey TeclaDaFase(int fase) {
    switch (fase) {
        case FASE_1_BUBBLE:    return KEY_A;
        case FASE_2_SELECTION: return KEY_S;
        case FASE_3_INSERTION: return KEY_D;
        case FASE_4_QUICK:     return KEY_F;
        case FASE_5_MERGE:     return KEY_G;
        default:               return KEY_NULL;
    }
}

static const char* TeclaNome(int fase) {
    switch (fase) {
        case FASE_1_BUBBLE:    return "[A]";
        case FASE_2_SELECTION: return "[S]";
        case FASE_3_INSERTION: return "[D]";
        case FASE_4_QUICK:     return "[F]";
        case FASE_5_MERGE:     return "[G]";
        default:               return "[]";
    }
}

static void DesenhaBarras(Memoria v[], int n, int fase) {
    const int marginX = 80;
    const int baseline = SCREEN_H - 120; // "chão" das barras
    const int dispW = SCREEN_W - 2*marginX;
    const float barW = (float)dispW / (float)n * 0.8f;
    const float slotW = (float)dispW / (float)n;
    // escala altura (valorAfeto de 0..100 -> 40..(SCREEN_H-260))
    const float minH = 40.f;
    const float maxH = (float)(SCREEN_H - 260);
    const Color cores[5] = {
        (Color){ 50, 120, 220, 255}, // azul
        (Color){  0, 170,  90, 255}, // verde
        (Color){230, 150,  40, 255}, // laranja
        (Color){160, 70,  200, 255}, // roxo
        (Color){220, 40,   70, 255}  // vermelho
    };

    for (int i = 0; i < n; i++) {
        float t = (float)v[i].valorAfeto / 100.0f;
        float h = minH + t * (maxH - minH);
        float x = (float)marginX + i * slotW + (slotW - barW) * 0.5f;
        float y = (float)baseline - h;
        Color c = cores[i % 5];
        DrawRectangleRec((Rectangle){x, y, barW, h}, c);
        DrawText(TextFormat("%d", v[i].valorAfeto), (int)(x + barW*0.3f), (int)(y - 18), 18, DARKGRAY);
    }
}

int main(void) {
    InitWindow(SCREEN_W, SCREEN_H, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    SetTargetFPS(60);

    // estado do jogo
    Fase fase = FASE_1_BUBBLE;
    Memoria vetor[MEM_MAX];
    int n = 0;
    bool ordenado = false;
    bool aguardandoEnter = false;

    // carrega primeira fase
    carregarFase(fase, vetor, &n);

    while (!WindowShouldClose()) {
        // input
        if (!aguardandoEnter) {
            // tecla da fase atual
            if (IsKeyPressed(TeclaDaFase(fase))) {
                // executa o algoritmo correspondente
                switch (fase) {
                    case FASE_1_BUBBLE:    bubbleSort(vetor, n);   break;
                    case FASE_2_SELECTION: selectionSort(vetor, n);break;
                    case FASE_3_INSERTION: insertionSort(vetor, n);break;
                    case FASE_4_QUICK:     quickSort(vetor, 0, n-1);break;
                    case FASE_5_MERGE:     mergeSort(vetor, 0, n-1);break;
                    default: break;
                }
                ordenado = isSorted(vetor, n);
                aguardandoEnter = true; // fase concluída, esperar ENTER
            }
        } else {
            if (IsKeyPressed(KEY_ENTER)) {
                // próxima fase
                if (fase == FASE_5_MERGE) {
                    fase = FIM;
                } else {
                    // IA bagunça para a próxima fase
                    embaralharPiorCaso((int)fase, vetor, n);
                    // carrega nova fase (novo conjunto)
                    fase = (Fase)((int)fase + 1);
                    carregarFase(fase, vetor, &n);
                    ordenado = false;
                    aguardandoEnter = false;
                }
            }
        }

        BeginDrawing();
        ClearBackground(FundoDaFase(fase));

        DrawText("Crescendo na Memoria", 40, 20, 36, (Color){40,40,40,255});
        DrawText(TextFormat("Fase %d/5  -  Algoritmo: %s", (fase<=5?fase:5), NomeAlgoritmoDaFase(fase)), 40, 70, 24, (Color){40,40,40,255});
        DrawLine(40, 104, SCREEN_W-40, 104, (Color){120,120,120,255});

        if (fase != FIM) {
            // instrucoes
            DrawText(TextFormat("Pressione %s para ordenar com %s",
                                TeclaNome(fase), NomeAlgoritmoDaFase(fase)),
                     40, 118, 22, DARKGRAY);

            if (!aguardandoEnter) {
                DrawText("Dica: observe os valores de afeto nas barras. Ordene em ordem crescente.", 40, 148, 20, GRAY);
            }

            // barras
            DesenhaBarras(vetor, n, fase);

            // feedback
            if (aguardandoEnter) {
                if (ordenado) {
                    DrawText("Memorias ordenadas com sucesso!", 40, SCREEN_H-100, 24, DARKGREEN);
                } else {
                    DrawText("Algo deu errado… (nao esta ordenado)", 40, SCREEN_H-100, 24, RED);
                }
                DrawText("Pressione [ENTER] para continuar", 40, SCREEN_H-70, 22, (Color){50,50,50,255});
            }
        } else {
            // tela final
            ClearBackground((Color){245, 230, 200, 255});
            DrawText("Voce encontrou o Tesouro Afetivo!", SCREEN_W/2 - 360, SCREEN_H/2 - 30, 36, GOLD);
            DrawText("Parabens! (ESC para sair)", SCREEN_W/2 - 200, SCREEN_H/2 + 20, 24, DARKBROWN);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
