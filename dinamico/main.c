#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ventas.h"

int main(int argc, char *argv[])
{
    Producto *productos = NULL;
    size_t cantidad = 0;
    int lecturaCorrecta;

    if (argc == 1) {
        lecturaCorrecta = leerVentas(&productos, &cantidad);
    } else if (argc == 2 && strcmp(argv[1], "--ejemplo") == 0) {
        lecturaCorrecta = cargarProductosEjemplo(&productos, &cantidad);
    } else {
        fprintf(stderr, "Uso: %s [--ejemplo]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!lecturaCorrecta) {
        free(productos);
        return EXIT_FAILURE;
    }

    ordenarProductos(productos, cantidad);
    mostrarRanking(productos, cantidad);
    free(productos);

    return EXIT_SUCCESS;
}
