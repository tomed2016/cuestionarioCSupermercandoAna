# Ranking nacional de ventas por producto

Dos soluciones independientes en C11 para el enunciado del supermercado SANTA ANA.
Cada solucion es multimodulo:

- `main.c`: coordina la lectura, el ordenamiento y el reporte.
- `ventas.h`: declara la estructura `Producto` y las funciones publicas.
- `ventas.c`: implementa la acumulacion de ventas y el ranking.

## Algoritmo

1. Leer el codigo de sucursal. Si es `0`, terminar sin leer otros campos.
2. Leer el codigo de producto y las unidades vendidas.
3. Buscar el producto en el vector: si existe, sumar las unidades; si no,
   incorporarlo con su primera venta.
4. Ordenar por unidades totales de mayor a menor. Si los totales coinciden,
   ordenar por codigo de producto de menor a mayor.
5. Mostrar la tabla. Si el primer codigo de sucursal es `0`, imprimir
   exactamente `No se Ingresaron Datos.`.

No se almacena la sucursal porque el reporte acumula las ventas nacionales.
Cada posicion representa un producto distinto, no una linea de entrada.
Los codigos y las unidades de cada registro deben ser enteros positivos
representables mediante `int`; los totales se acumulan en
`unsigned long long`, verificando que no desborden.
Las entradas incompletas, los datos invalidos y los errores de recursos se
informan en la salida de errores y terminan el programa con un codigo no cero,
sin imprimir un ranking parcial.

## Version con vector dinamico

La carpeta `dinamico` contiene un vector de estructuras que comienza vacio.
Se reserva espacio para 8 productos y se duplica la capacidad cuando se llena.
No tiene un limite fijo de productos: depende de la memoria disponible y de
los limites de representacion de la plataforma.

Se utiliza `realloc` mediante un puntero auxiliar para conservar el vector
original si falla la reserva. `main` libera la memoria tanto al finalizar
normalmente como ante un error. La lectura recibe un puntero inicialmente
`NULL` y una cantidad inicialmente cero.

El ordenamiento utiliza `qsort`, con un comparador que evita restas susceptibles
de desbordamiento.

Desde la carpeta del proyecto, compilar y ejecutar en PowerShell:

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic .\dinamico\main.c .\dinamico\ventas.c -o .\dinamico\ranking.exe
.\dinamico\ranking.exe
```

### Precarga de 20 productos de ejemplo

Para probar el ranking sin ingresar datos por teclado:

```powershell
make -C .\dinamico
.\dinamico\ranking.exe --ejemplo
```

La funcion `cargarProductosEjemplo`, declarada en `ventas.h` e implementada
en `ventas.c`, reserva con `malloc` un vector de 20 estructuras `Producto`
y copia los datos de ejemplo. Se utiliza con un puntero inicialmente `NULL`
y una cantidad inicialmente cero, como alternativa a `leerVentas`.
`main` ordena el vector, imprime el ranking y libera su memoria.

Los productos tienen codigos del 101 al 120 y totales de ventas ya acumulados.
Estan cargados sin orden e incluyen empates para comprobar el desempate por
codigo ascendente. El ranking comienza con los productos 101 (1200 unidades)
y 104 (1100 unidades); los productos 105 y 106 empatan con 980 unidades,
por lo que aparece primero el 105.

Sin `--ejemplo`, el programa sigue leyendo las ventas por teclado hasta la
sucursal `0`, sin precargar ningun producto. La opcion de ejemplo no agrega
ventas por teclado ni modifica la version estatica.

## Version con vector estatico

La carpeta `estatico` utiliza `Producto productos[MAX_PRODUCTOS]`, con
`MAX_PRODUCTOS` definido como `200` en `ventas.h`.
No utiliza `malloc`, `realloc` ni memoria dinamica; el ordenamiento por insercion
trabaja sobre el mismo vector.

Se admiten hasta **200 productos distintos** y cualquier cantidad de registros
de venta de esos productos, dentro del rango de los totales.
Aunque el vector este lleno, se siguen acumulando las ventas de productos
existentes. Un producto distinto numero 201 se informa como error, sin escribir
fuera del arreglo.

```powershell
gcc -std=c11 -Wall -Wextra -Wpedantic .\estatico\main.c .\estatico\ventas.c -o .\estatico\ranking.exe
.\estatico\ranking.exe
```

Compilar cada version por separado: ambas tienen su propio `main`.
En Code::Blocks u otro IDE, crear un proyecto de consola en C para cada version
y agregar sus tres archivos al proyecto correspondiente.

## Compilar con Makefile

Cada carpeta incluye su propio `Makefile`. Se requiere GCC y GNU Make.
Desde la carpeta del proyecto, en PowerShell:

```powershell
make -C .\dinamico
.\dinamico\ranking.exe

make -C .\estatico
.\estatico\ranking.exe
```

Si GNU Make esta instalado como `mingw32-make`, usar ese comando en lugar
de `make`.

La regla predeterminada `all` compila los modulos por separado y genera
`ranking.exe` en la carpeta de cada version. Los cambios en `ventas.h`
recompilan ambos modulos; si no hay cambios, no se recompila.

Para eliminar solamente los objetos y el ejecutable generados:

```powershell
make -C .\dinamico clean
make -C .\estatico clean
```

Las opciones se pueden personalizar desde la linea de comandos, por ejemplo:

```powershell
make -C .\dinamico CFLAGS="-std=c11 -Wall -Wextra -Wpedantic -O2"
```

Si se cambian las opciones de compilacion, ejecutar `clean` antes de volver
a compilar para regenerar los objetos con las nuevas opciones.

## Ejemplo

Entrada:

```text
3 3 1
3 2 1
1 2 1
2 2 2
1 2 1
1 4 5
0
```

Salida de ambas versiones:

```text
_________________________________________________
| RANKING NACIONAL DE VENTAS POR PRODUCTO       |
|_______________________________________________|
| CODIGO PRODUCTO       |TOTAL UNIDADES VENDIDAS|
|_______________________|_______________________|
|                       |                       |
|                      2|                      5|
|                      4|                      5|
|                      3|                      1|
|_______________________|_______________________|
```

Para `N` registros y `P` productos distintos, la busqueda lineal durante la
lectura cuesta O(N * P). La version estatica ordena en O(P^2) y ocupa un arreglo
fijo de 200 posiciones. El vector dinamico ocupa O(P) memoria; el costo de
`qsort` depende de la implementacion de la biblioteca de C.
