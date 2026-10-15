#ifndef VENTAS_H
#define VENTAS_H

#include <stddef.h>

typedef struct {
    int codigo;
    unsigned long long unidades;
} Producto;

/* Recibe un vector inicialmente NULL y una cantidad inicialmente cero. */
int leerVentas(Producto **productos, size_t *cantidad);
/* Recibe NULL y cantidad cero; crea un vector nuevo de 20 productos de ejemplo. */
int cargarProductosEjemplo(Producto **productos, size_t *cantidad);
void ordenarProductos(Producto productos[], size_t cantidad);
void mostrarRanking(const Producto productos[], size_t cantidad);

#endif
