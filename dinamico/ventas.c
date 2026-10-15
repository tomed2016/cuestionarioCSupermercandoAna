#include "ventas.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cargarProductosEjemplo(Producto **productos, size_t *cantidad)
{
    /* Los totales ya estan acumulados; el desorden y los empates prueban el ranking. */
    const Producto ejemplos[] = {
        {115, 320},
        {103, 850},
        {120, 75},
        {101, 1200},
        {108, 450},
        {112, 600},
        {105, 980},
        {119, 150},
        {102, 850},
        {110, 720},
        {117, 250},
        {104, 1100},
        {114, 450},
        {107, 900},
        {116, 320},
        {109, 780},
        {113, 500},
        {106, 980},
        {118, 200},
        {111, 650}
    };
    /* El cociente obtiene la cantidad de elementos, no el tamano en bytes. */
    size_t total = sizeof ejemplos / sizeof ejemplos[0];
    Producto *nuevoVector = malloc(sizeof ejemplos);

    if (nuevoVector == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para los productos de ejemplo.\n");
        return 0;
    }
    /* Se copia a memoria dinamica porque el arreglo local deja de existir al retornar.
       El llamador recibe el vector mediante el doble puntero y debe liberarlo. */
    memcpy(nuevoVector, ejemplos, sizeof ejemplos);
    *productos = nuevoVector;
    *cantidad = total;
    return 1;
}

static size_t buscarProducto(const Producto productos[], size_t cantidad,
                             int codigo)
{
    size_t i;

    for (i = 0; i < cantidad; ++i) {
        if (productos[i].codigo == codigo) {
            return i;
        }
    }

    /* Los indices validos llegan hasta cantidad - 1: cantidad indica "no encontrado". */
    return cantidad;
}

int leerVentas(Producto **productos, size_t *cantidad)
{
    /* Esta lectura parte de NULL y cantidad cero; no agrega ventas a una precarga.
       Capacidad cuenta posiciones reservadas; cantidad, productos almacenados. */
    size_t capacidad = 0;

    for (;;) {
        int sucursal;
        int codigo;
        int unidades;
        size_t posicion;

        if (scanf("%d", &sucursal) != 1) {
            fprintf(stderr, "Error: falta el codigo de sucursal o el terminador 0.\n");
            return 0;
        }
        /* El terminador ocupa solo un campo: no se deben leer producto ni unidades. */
        if (sucursal == 0) {
            return 1;
        }
        if (sucursal < 0 || scanf("%d%d", &codigo, &unidades) != 2
                || codigo <= 0 || unidades <= 0) {
            fprintf(stderr, "Error: cada venta debe contener tres enteros positivos.\n");
            return 0;
        }

        /* La sucursal no se almacena: las ventas se agrupan por producto a nivel nacional. */
        posicion = buscarProducto(*productos, *cantidad, codigo);
        if (posicion < *cantidad) {
            /* Verificar antes de sumar evita que el total desborde y vuelva a cero. */
            if ((*productos)[posicion].unidades > ULLONG_MAX
                    - (unsigned long long)unidades) {
                fprintf(stderr, "Error: el total de unidades excede el rango permitido.\n");
                return 0;
            }
            (*productos)[posicion].unidades += (unsigned long long)unidades;
        } else {
            if (*cantidad == capacidad) {
                size_t nuevaCapacidad;
                Producto *nuevoVector;

                /* Ambas comprobaciones evitan desbordamientos al calcular la reserva. */
                if (capacidad > SIZE_MAX / 2) {
                    fprintf(stderr, "Error: demasiados productos.\n");
                    return 0;
                }
                /* Duplicar reduce las reservas frente a crecer de una posicion a la vez. */
                nuevaCapacidad = capacidad == 0 ? 8 : capacidad * 2;
                if (nuevaCapacidad > SIZE_MAX / sizeof **productos) {
                    fprintf(stderr, "Error: el vector excede el tamano permitido.\n");
                    return 0;
                }
                /* El auxiliar conserva el puntero original si realloc falla.
                   realloc puede mover el vector; por eso se actualiza *productos. */
                nuevoVector = realloc(*productos,
                                      nuevaCapacidad * sizeof **productos);
                if (nuevoVector == NULL) {
                    fprintf(stderr, "Error: no se pudo reservar memoria.\n");
                    return 0;
                }
                *productos = nuevoVector;
                capacidad = nuevaCapacidad;
            }
            (*productos)[*cantidad].codigo = codigo;
            (*productos)[*cantidad].unidades = (unsigned long long)unidades;
            ++*cantidad;
        }
    }
}

static int compararProductos(const void *primero, const void *segundo)
{
    const Producto *a = primero;
    const Producto *b = segundo;

    /* qsort ubica primero a cuando el comparador devuelve un valor negativo. */
    if (a->unidades != b->unidades) {
        return a->unidades > b->unidades ? -1 : 1;
    }
    /* En empate, codigo ascendente sin restar codigos que podrian desbordar int. */
    return (a->codigo > b->codigo) - (a->codigo < b->codigo);
}

void ordenarProductos(Producto productos[], size_t cantidad)
{
    /* Evita pasar a qsort el puntero NULL que representa un vector vacio. */
    if (cantidad > 1) {
        qsort(productos, cantidad, sizeof *productos, compararProductos);
    }
}

void mostrarRanking(const Producto productos[], size_t cantidad)
{
    size_t i;

    if (cantidad == 0) {
        puts("No se Ingresaron Datos.");
        return;
    }

    puts("_________________________________________________");
    puts("| RANKING NACIONAL DE VENTAS POR PRODUCTO       |");
    puts("|_______________________________________________|");
    puts("| CODIGO PRODUCTO       |TOTAL UNIDADES VENDIDAS|");
    puts("|_______________________|_______________________|");
    puts("|                       |                       |");
    for (i = 0; i < cantidad; ++i) {
        /* Ancho minimo 23, alineado a la derecha; llu corresponde al total sin signo. */
        printf("|%23d|%23llu|\n", productos[i].codigo, productos[i].unidades);
    }
    puts("|_______________________|_______________________|");
}
