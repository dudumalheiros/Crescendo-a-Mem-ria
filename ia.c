#include "ia.h"
#include "algoritmos.h"

void iaOrdena(Memoria vetor[], int n) {
    quickSort(vetor, 0, n - 1);
}
