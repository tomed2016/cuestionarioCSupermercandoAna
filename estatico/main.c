#include <stdlib.h>

#include "ventas.h"

int main(void)
{
    Producto productos[MAX_PRODUCTOS];
    size_t cantidad = 0;

    if (!leerVentas(productos, &cantidad)) {
        return EXIT_FAILURE;
    }

    ordenarProductos(productos, cantidad);
    mostrarRanking(productos, cantidad);

    return EXIT_SUCCESS;
}
