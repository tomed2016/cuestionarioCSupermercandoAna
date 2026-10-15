#include "ventas.h"

#include <limits.h>
#include <stdio.h>

static size_t buscarProducto(const Producto productos[], size_t cantidad,
                             int codigo)
{
    size_t i;

    /* Solo se recorren las posiciones ocupadas, no las 200 posiciones reservadas. */
    for (i = 0; i < cantidad; ++i) {
        if (productos[i].codigo == codigo) {
            return i;
        }
    }

    /* Los indices validos llegan hasta cantidad - 1: cantidad indica "no encontrado". */
    return cantidad;
}

int leerVentas(Producto productos[], size_t *cantidad)
{
    /* El llamador proporciona un arreglo de MAX_PRODUCTOS y una cantidad inicial cero.
       El puntero permite actualizar la cantidad de productos distintos en main. */
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
        posicion = buscarProducto(productos, *cantidad, codigo);
        if (posicion < *cantidad) {
            /* Verificar antes de sumar evita que el total desborde y vuelva a cero. */
            if (productos[posicion].unidades > ULLONG_MAX
                    - (unsigned long long)unidades) {
                fprintf(stderr, "Error: el total de unidades excede el rango permitido.\n");
                return 0;
            }
            productos[posicion].unidades += (unsigned long long)unidades;
        } else {
            /* El limite afecta a productos distintos, no a registros de venta.
               Con el arreglo lleno, todavia se acumulan ventas de productos existentes. */
            if (*cantidad == MAX_PRODUCTOS) {
                fprintf(stderr, "Error: se supero el limite de %d productos distintos.\n",
                        MAX_PRODUCTOS);
                return 0;
            }
            productos[*cantidad].codigo = codigo;
            productos[*cantidad].unidades = (unsigned long long)unidades;
            ++*cantidad;
        }
    }
}

void ordenarProductos(Producto productos[], size_t cantidad)
{
    size_t i;

    /* Insercion: antes de cada paso, las posiciones anteriores a i ya estan ordenadas.
       Se trabaja sobre el mismo arreglo, sin reservar un segundo vector. */
    for (i = 1; i < cantidad; ++i) {
        Producto actual = productos[i];
        size_t j = i;

        /* Se desplazan los productos que deben quedar despues del actual:
           menos unidades o, en empate, mayor codigo. j > 0 evita acceder al indice -1. */
        while (j > 0
                && (productos[j - 1].unidades < actual.unidades
                    || (productos[j - 1].unidades == actual.unidades
                        && productos[j - 1].codigo > actual.codigo))) {
            productos[j] = productos[j - 1];
            --j;
        }
        /* La copia temporal conserva el producto mientras se desplazan los anteriores. */
        productos[j] = actual;
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
