#include "ia.h"
#include <stdlib.h>
#include <time.h>

static void swapM(Memoria *a, Memoria *b) {
    Memoria t = *a; *a = *b; *b = t;
}

void embaralharPiorCaso(int faseAtual, Memoria v[], int n) {
    // ideia: preparar o vetor para dificultar o algoritmo da PROXIMA fase
    // (faseAtual+1). Simplificação: reverte, quase ordena, ou embaralha forte.
    srand((unsigned)time(NULL));

    int prox = faseAtual + 1;
    if (prox == 2) {
        // selection -> muita repeticao e dispersao
        for (int i = 0; i < n; ++i) v[i].valorAfeto = (i*13 + rand()%5) % 100;
    } else if (prox == 3) {
        // insertion -> vetor quase ordenado (desafio pequeno)
        for (int i = 0; i < n; ++i) v[i].valorAfeto = 5 + i*5;
        // insere um erro leve
        if (n >= 2) {
            int a = rand()%n, b = rand()%n;
            swapM(&v[a], &v[b]);
        }
    } else if (prox == 4) {
        // quick -> bastante aleatorio (para pivot ruim ocasional)
        for (int i = 0; i < n; ++i) v[i].valorAfeto = rand()%100;
    } else if (prox == 5) {
        // merge -> mistura moderada
        for (int i = 0; i < n; ++i) v[i].valorAfeto = (i*7 + rand()%10) % 100;
    } else {
        // caso geral: reverso
        for (int i = 0; i < n/2; ++i) swapM(&v[i], &v[n-1-i]);
    }
}
