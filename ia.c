#include "ia.h"
#include "algoritmos.h"
#include <stdlib.h>
#include <time.h>

void ordenarIA(Memoria vetor[], int n) {
    srand(time(NULL));
    int algoritmo = (rand() % 3) + 1; // Escolhe entre Bubble, Selection e Quick
    ordenarMemorias(vetor, n, algoritmo);
}
