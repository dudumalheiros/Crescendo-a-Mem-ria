// main.c
#include "raylib.h"
#include "memoria.h"
#include "algoritmos.h"
#include "ia.h"
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define LARGURA 900
#define ALTURA  600
#define TAMANHO 5

typedef enum {
    EST_MENU,
    EST_FASE,
    EST_PESOS,
    EST_RESULTADO,
    EST_EXPLICACAO,
    EST_FIM
} EstadoJogo;

typedef struct {
    Rectangle paredes[32];
    int qtd;
} Labirinto;

static void DesenharFundoFase1(void) {
    ClearBackground((Color){135, 206, 235, 255});
    DrawCircle(780, 120, 60, YELLOW);
    DrawCircle(300, 640, 320, (Color){0, 200, 0, 255});
    DrawCircle(700, 640, 260, (Color){34, 139, 34, 255});
}

static void DesenharFundoFase2(void) {
    ClearBackground((Color){255, 165, 90, 255});
    DrawCircle(120, 120, 60, ORANGE);
    DrawCircle(300, 640, 320, (Color){200, 120, 50, 255});
    DrawCircle(700, 640, 260, (Color){160, 82, 45, 255});
}

static void DesenharFundoFase3(void) {
    ClearBackground((Color){10, 10, 30, 255});
    DrawCircle(780, 120, 50, (Color){240, 240, 180, 255});
    for (int i = 0; i < 120; i++)
        DrawPixel(rand() % LARGURA, rand() % ALTURA, WHITE);
}

static const char *NomeAlgoritmoDaFase(int fase) {
    if (fase == 1) return "Bubble Sort";
    if (fase == 2) return "Selection Sort";
    return "Quick Sort";
}

static int TipoAlgoritmoDaFase(int fase) {
    if (fase == 1) return 1; // bubble
    if (fase == 2) return 2; // selection
    return 3;               // quick
}

// -------- LABIRINTOS (os mesmos da sua versão) --------
static Labirinto CriarLabirinto(int fase) {
    Labirinto l = {0};

    if (fase == 1) { // simples, com vão central
        Rectangle tmp[] = {
            {50, 100, 800, 20},  // topo
            {50, 500, 800, 20},  // base
            {50, 100, 20, 420},  // esquerda
            {830, 100, 20, 420}, // direita

            // retângulo interno com vão no meio
            {200, 180, 500, 20},  // topo interno
            {200, 180, 20, 160},  // lateral esquerda
            {680, 180, 20, 160},  // lateral direita
            {200, 320, 190, 20},  // base esquerda
            {510, 320, 190, 20},  // base direita
        };
        l.qtd = sizeof(tmp)/sizeof(tmp[0]);
        for (int i = 0; i < l.qtd; i++) l.paredes[i] = tmp[i];
    }
    else if (fase == 2) { // médio
        Rectangle tmp[] = {
            {50, 100, 800, 20},
            {50, 500, 800, 20},
            {50, 100, 20, 420},
            {830, 100, 20, 420},

            {150, 200, 600, 20},
            {150, 350, 200, 20},
            {550, 350, 200, 20},
            {150, 200, 20, 150},
            {730, 200, 20, 150},
            {430, 260, 40, 20}
        };
        l.qtd = sizeof(tmp)/sizeof(tmp[0]);
        for (int i = 0; i < l.qtd; i++) l.paredes[i] = tmp[i];
    }
    else { // fase 3
        Rectangle tmp[] = {
            // moldura
            {50,  80, 800, 20},
            {50, 520, 800, 20},
            {50,  80,  20, 460},
            {830, 80,  20, 460},

            // interno
            {200, 180, 500, 20},
            {200, 420, 220, 20},
            {480, 420, 220, 20},
            {200, 180,  20, 240},
            {680, 180,  20, 240},
            {420, 280,  60, 20}
        };
        l.qtd = sizeof(tmp)/sizeof(tmp[0]);
        for (int i = 0; i < l.qtd; i++) l.paredes[i] = tmp[i];
    }

    return l;
}

// --------- EXPLICAÇÃO EM UMA TELA SEPARADA ---------
static void DesenharPainelExplicacao(int fase) {
    Rectangle painel = {40, 260, LARGURA - 80, 260};
    DrawRectangleRec(painel, (Color){220, 220, 220, 255});
    DrawRectangleLinesEx(painel, 2, DARKGRAY);

    int x = (int)painel.x + 20;
    int y = (int)painel.y + 20;

    if (fase == 1) {
        DrawText("Bubble Sort: percorre o vetor varias vezes, comparando vizinhos.",
                 x, y, 18, DARKBLUE);
        y += 28;
        DrawText("Se o da esquerda for maior, troca de lugar. As memorias vao 'borbulhando' ate ficarem",
                 x, y, 16, DARKGRAY);
        y += 22;
        DrawText("na ordem dos pesos que voce escolheu.", x, y, 16, DARKGRAY);
        y += 28;
        DrawText("Exemplo (peso usando campo valorAfeto):", x, y, 16, BLUE);
        y += 22;
        DrawText("for (i = 0; i < n-1; i++) {", x, y, 16, BLUE); y += 20;
        DrawText("    for (j = 0; j < n-1-i; j++) {", x, y, 16, BLUE); y += 20;
        DrawText("        if (v[j].valorAfeto > v[j+1].valorAfeto)", x, y, 16, BLUE); y += 20;
        DrawText("            swap(v[j], v[j+1]);", x, y, 16, BLUE); y += 20;
        DrawText("    }", x, y, 16, BLUE); y += 20;
        DrawText("}", x, y, 16, BLUE);
    }
    else if (fase == 2) {
        DrawText("Selection Sort: em cada posicao, procura o menor peso no resto do vetor.",
                 x, y, 18, DARKBLUE);
        y += 28;
        DrawText("Depois coloca esse menor na posicao correta. Poucas trocas, mas ainda percorre",
                 x, y, 16, DARKGRAY);
        y += 22;
        DrawText("bastante o vetor de memorias.", x, y, 16, DARKGRAY);
        y += 28;
        DrawText("Exemplo:", x, y, 16, BLUE); y += 22;
        DrawText("for (i = 0; i < n-1; i++) {", x, y, 16, BLUE); y += 20;
        DrawText("    int min = i;", x, y, 16, BLUE); y += 20;
        DrawText("    for (j = i+1; j < n; j++)", x, y, 16, BLUE); y += 20;
        DrawText("        if (v[j].valorAfeto < v[min].valorAfeto) min = j;", x, y, 16, BLUE); y += 20;
        DrawText("    swap(v[i], v[min]);", x, y, 16, BLUE); y += 20;
        DrawText("}", x, y, 16, BLUE);
    }
    else { // fase 3
        DrawText("Quick Sort: escolhe um pivo e separa memorias com peso menor e maior.",
                 x, y, 18, DARKBLUE);
        y += 28;
        DrawText("Ordena recursivamente cada lado. Em geral, e bem rapido para vetores grandes,",
                 x, y, 16, DARKGRAY);
        y += 22;
        DrawText("por isso e muito usado na pratica.", x, y, 16, DARKGRAY);
        y += 28;
        DrawText("Exemplo simplificado:", x, y, 16, BLUE); y += 22;
        DrawText("int p = particiona(v, esq, dir);", x, y, 16, BLUE); y += 20;
        DrawText("quickSort(v, esq, p-1);",         x, y, 16, BLUE); y += 20;
        DrawText("quickSort(v, p+1, dir);",         x, y, 16, BLUE);
    }
}

// ---------------------------------------------------
//                      MAIN
// ---------------------------------------------------
int main(void) {
    InitWindow(LARGURA, ALTURA, "Crescendo na Memoria - Pique-Esconde dos Algoritmos");
    InitAudioDevice();
    SetTargetFPS(60);

    Music musicaMenu = LoadMusicStream("assets/menu_music.mp3");
    Music musicaJogo = LoadMusicStream("assets/game_music.mp3");
    PlayMusicStream(musicaMenu);

    EstadoJogo estado = EST_MENU;
    int faseAtual = 1;

    Vector2 jogador = {150, 450};
    float velocidade = 3.0f;
    Labirinto lab = CriarLabirinto(faseAtual);

    Memoria vetor[TAMANHO];
    criarMemorias(vetor, TAMANHO);

    // variaveis da tela de PESOS
    int indiceSelecionado = 0;

    while (!WindowShouldClose()) {
        if (estado == EST_MENU || estado == EST_FIM)
            UpdateMusicStream(musicaMenu);
        else
            UpdateMusicStream(musicaJogo);

        BeginDrawing();

        // ---------------- MENU ----------------
        if (estado == EST_MENU) {
            DesenharFundoFase1();
            const char *titulo    = "Crescendo na Memoria";
            const char *subtitulo = "Pique-Esconde dos Algoritmos";
            int tl = MeasureText(titulo, 40);
            int sl = MeasureText(subtitulo, 20);
            DrawText(titulo,    (LARGURA - tl)/2, 120, 40, DARKBLUE);
            DrawText(subtitulo, (LARGURA - sl)/2, 180, 20, DARKGRAY);

            DrawText("Pressione [ENTER] para comecar", 280, 320, 20, DARKGRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                StopMusicStream(musicaMenu);
                PlayMusicStream(musicaJogo);

                estado      = EST_FASE;
                faseAtual   = 1;
                jogador     = (Vector2){150, 450};
                lab         = CriarLabirinto(faseAtual);
                criarMemorias(vetor, TAMANHO);
            }
        }

        // ---------------- FASE (labirinto) ----------------
        else if (estado == EST_FASE) {

            if (faseAtual == 1) DesenharFundoFase1();
            else if (faseAtual == 2) DesenharFundoFase2();
            else DesenharFundoFase3();

            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 22, DARKBLUE);
            DrawText(TextFormat("Algoritmo desta fase: %s", NomeAlgoritmoDaFase(faseAtual)),
                     20, 50, 20, BLUE);
            DrawText("Use as setas para mover e [ESPACO] para restaurar memorias",
                     20, ALTURA - 30, 18, DARKGRAY);

            Vector2 novaPos = jogador;
            if (IsKeyDown(KEY_RIGHT)) novaPos.x += velocidade;
            if (IsKeyDown(KEY_LEFT))  novaPos.x -= velocidade;
            if (IsKeyDown(KEY_UP))    novaPos.y -= velocidade;
            if (IsKeyDown(KEY_DOWN))  novaPos.y += velocidade;

            bool colidiu = false;
            for (int i = 0; i < lab.qtd; i++) {
                if (CheckCollisionCircleRec(novaPos, 12, lab.paredes[i])) {
                    colidiu = true;
                    break;
                }
            }
            if (!colidiu) jogador = novaPos;

            for (int i = 0; i < lab.qtd; i++)
                DrawRectangleRec(lab.paredes[i], DARKGRAY);

            DrawCircleV(jogador, 12, DARKBLUE);

            // memórias
            for (int i = 0; i < TAMANHO; i++) {
                Rectangle r = { vetor[i].coordX, vetor[i].coordY, 40, 40 };
                Color cor = vetor[i].encontrada ? BLUE : SKYBLUE;
                DrawRectangleRec(r, cor);

                // nome só depois de encontrada
                if (vetor[i].encontrada) {
                    DrawText(vetor[i].nome,
                             vetor[i].coordX - 20,
                             vetor[i].coordY - 18,
                             10, RAYWHITE);
                }

                if (!vetor[i].encontrada &&
                    CheckCollisionCircleRec(jogador, 12, r)) {
                    DrawText("Pressione [ESPACO] para restaurar memoria",
                             220, 80, 20, RED);
                    if (IsKeyPressed(KEY_SPACE)) {
                        vetor[i].encontrada = 1;
                    }
                }
            }

            bool todas = true;
            for (int i = 0; i < TAMANHO; i++)
                if (!vetor[i].encontrada) todas = false;

            // terminou de pegar todas -> ir para tela de PESOS
            if (todas) {
                estado = EST_PESOS;
                indiceSelecionado = 0;
                // zera pesos para o jogador começar a atribuir
                for (int i = 0; i < TAMANHO; i++)
                    vetor[i].valorAfeto = 0;
            }
        }

        // ---------------- ATRIBUIR PESOS ----------------
        else if (estado == EST_PESOS) {

            if (faseAtual == 1) DesenharFundoFase1();
            else if (faseAtual == 2) DesenharFundoFase2();
            else DesenharFundoFase3();

            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 22, DARKBLUE);
            DrawText(TextFormat("Algoritmo desta fase: %s",
                                NomeAlgoritmoDaFase(faseAtual)),
                     20, 50, 20, BLUE);

            DrawText("Agora distribua os PESOS (1 = mais importante, 5 = menos).",
                     60, 100, 20, DARKBLUE);
            DrawText("Use SETA CIMA/BAIXO para escolher a memoria, e teclas [1..5] para definir o peso.",
                     60, 130, 18, DARKGRAY);
            DrawText("Voce pode mudar o peso quantas vezes quiser. ENTER confirma.",
                     60, 155, 18, DARKGRAY);

            // navegação
            if (IsKeyPressed(KEY_UP)) {
                indiceSelecionado--;
                if (indiceSelecionado < 0) indiceSelecionado = TAMANHO - 1;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                indiceSelecionado++;
                if (indiceSelecionado >= TAMANHO) indiceSelecionado = 0;
            }

            // atribuição de peso sem repetir
            int pesoEscolhido = 0;
            if (IsKeyPressed(KEY_ONE))   pesoEscolhido = 1;
            if (IsKeyPressed(KEY_TWO))   pesoEscolhido = (TAMANHO >= 2 ? 2 : 0);
            if (IsKeyPressed(KEY_THREE)) pesoEscolhido = (TAMANHO >= 3 ? 3 : 0);
            if (IsKeyPressed(KEY_FOUR))  pesoEscolhido = (TAMANHO >= 4 ? 4 : 0);
            if (IsKeyPressed(KEY_FIVE))  pesoEscolhido = (TAMANHO >= 5 ? 5 : 0);

            if (pesoEscolhido > 0) {
                // remove esse peso de qualquer outro
                for (int i = 0; i < TAMANHO; i++) {
                    if (i != indiceSelecionado &&
                        vetor[i].valorAfeto == pesoEscolhido) {
                        vetor[i].valorAfeto = 0;
                    }
                }
                // aplica na selecionada
                vetor[indiceSelecionado].valorAfeto = pesoEscolhido;
            }

            // desenha lista
            int y0 = 200;
            for (int i = 0; i < TAMANHO; i++) {
                Color corLinha = (i == indiceSelecionado) ? YELLOW : RAYWHITE;
                DrawRectangle(40, y0 - 4, LARGURA - 80, 26, Fade(BLACK, 0.1f));

                DrawText(TextFormat("%d) %s", i+1, vetor[i].nome),
                         60, y0, 18, corLinha);

                const char *pesoTxt =
                    (vetor[i].valorAfeto == 0) ?
                    "(sem peso)" :
                    TextFormat("(peso %d)", vetor[i].valorAfeto);

                DrawText(pesoTxt, 650, y0, 18, corLinha);
                y0 += 30;
            }

            // verifica se todos possuem peso
            bool completo = true;
            for (int i = 0; i < TAMANHO; i++) {
                if (vetor[i].valorAfeto <= 0) {
                    completo = false;
                    break;
                }
            }

            if (!completo)
                DrawText("Defina um peso diferente para cada memoria.",
                         60, ALTURA - 60, 18, RED);
            else
                DrawText("Tudo pronto! Pressione [ENTER] para ordenar as memorias.",
                         60, ALTURA - 60, 18, GREEN);

            if (completo && IsKeyPressed(KEY_ENTER)) {
                // ordena usando o algoritmo da fase
                ordenarMemorias(vetor, TAMANHO, TipoAlgoritmoDaFase(faseAtual));
                estado = EST_RESULTADO;
            }
        }

        // ---------------- RESULTADO ORDENACAO ----------------
        else if (estado == EST_RESULTADO) {

            if (faseAtual == 1) DesenharFundoFase1();
            else if (faseAtual == 2) DesenharFundoFase2();
            else DesenharFundoFase3();

            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 22, DARKBLUE);
            DrawText(TextFormat("Algoritmo desta fase: %s",
                                NomeAlgoritmoDaFase(faseAtual)),
                     20, 50, 20, BLUE);

            DrawText("Resultado da ordenacao pelas suas escolhas:",
                     140, 100, 24, DARKBLUE);

            int y = 150;
            for (int i = 0; i < TAMANHO; i++) {
                DrawText(TextFormat("%d) %s  (peso %d)",
                                    i+1,
                                    vetor[i].nome,
                                    vetor[i].valorAfeto),
                         140, y, 20, DARKBLUE);
                y += 32;
            }

            DrawText(TextFormat("Aqui usamos %s para organizar do peso 1 ao %d.",
                                NomeAlgoritmoDaFase(faseAtual),
                                TAMANHO),
                     140, y + 10, 18, DARKGRAY);

            DrawText("Pressione [ENTER] para ver uma explicacao do algoritmo.",
                     140, ALTURA - 40, 18, RED);

            if (IsKeyPressed(KEY_ENTER)) {
                estado = EST_EXPLICACAO;
            }
        }

        // ---------------- EXPLICACAO ALGORITMO ----------------
        else if (estado == EST_EXPLICACAO) {

            if (faseAtual == 1) DesenharFundoFase1();
            else if (faseAtual == 2) DesenharFundoFase2();
            else DesenharFundoFase3();

            DrawText(TextFormat("Fase %d / 3", faseAtual), 20, 20, 22, DARKBLUE);
            DrawText(TextFormat("Algoritmo desta fase: %s",
                                NomeAlgoritmoDaFase(faseAtual)),
                     20, 50, 20, BLUE);

            DrawText("Como esse algoritmo organiza as memorias?",
                     140, 100, 24, DARKBLUE);

            DesenharPainelExplicacao(faseAtual);

            DrawText("Pressione [ENTER] para seguir.",
                     260, ALTURA - 40, 20, RED);

            if (IsKeyPressed(KEY_ENTER)) {
                if (faseAtual < 3) {
                    faseAtual++;
                    jogador = (Vector2){150, 450};
                    lab     = CriarLabirinto(faseAtual);
                    criarMemorias(vetor, TAMANHO);
                    estado  = EST_FASE;
                } else {
                    StopMusicStream(musicaJogo);
                    PlayMusicStream(musicaMenu);
                    estado = EST_FIM;
                }
            }
        }

        // ---------------- TELA FINAL ----------------
        else if (estado == EST_FIM) {
            DesenharFundoFase3();
            DrawText("Parabens!", 360, 200, 40, SKYBLUE);
            DrawText("Voce organizou todas as memorias com tres algoritmos diferentes!",
                     120, 260, 20, RAYWHITE);
            DrawText("Pressione [ENTER] para voltar ao menu ou [ESC] para sair.",
                     150, 320, 20, LIGHTGRAY);

            if (IsKeyPressed(KEY_ENTER)) {
                estado    = EST_MENU;
                faseAtual = 1;
                jogador   = (Vector2){150, 450};
                lab       = CriarLabirinto(faseAtual);
                criarMemorias(vetor, TAMANHO);
                StopMusicStream(musicaMenu);
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
