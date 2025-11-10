#ifndef MEMORIA_H
#define MEMORIA_H

typedef struct {
    char nome[50];
    int valorAfeto;
    int coordX, coordY;
} Memoria;

void inicializarMemorias(Memoria vetor[], int tamanho);
void mostrarMemorias(Memoria vetor[], int tamanho);
void embaralharMemorias(Memoria vetor[], int tamanho);
void trocarMemorias(Memoria *a, Memoria *b);

#endif
