#ifndef MEMORIA_H
#define MEMORIA_H

typedef struct {
    char nome[100];
    int valorAfeto;
    int coordX;
    int coordY;
    int encontrada;
} Memoria;

void criarMemorias(Memoria vetor[], int n);
void desenharMemorias(Memoria vetor[], int n);
void verificarOrdem(Memoria vetor[], int n);

#endif
