#ifndef VENTAS_H
#define VENTAS_H

#include <stddef.h>

#define MAX_PRODUCTOS 1000

typedef struct {
    int codigo;
    unsigned long long unidades;
} Producto;

int leerVentas(Producto productos[], size_t *cantidad);
void ordenarProductos(Producto productos[], size_t cantidad);
void mostrarRanking(const Producto productos[], size_t cantidad);

#endif
