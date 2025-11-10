#ifndef MEMORIA_H
#define MEMORIA_H

#define MEM_MAX 16

typedef struct {
    int x, y; // reservado para futuro (mapa 2D)
} Coordenada;

typedef struct {
    char nome[64];
    int  valorAfeto;   // 0..100
    Coordenada coord;
} Memoria;

// Carrega conjunto de memorias adequado para a fase (e tamanho n)
void carregarFase(int fase, Memoria vetor[], int *n);

#endif
