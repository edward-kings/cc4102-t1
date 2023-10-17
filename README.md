# cc4102-t1
Tarea para el curso de Diseño y Análisis de Algoritmos (CC4102). Universidad de Chile

Este repositorio compara 3 métodos de construcción de R-Trees: Nearest X, Hilbert y Sort Tile Recursive.

El R-Tree esta formado por dos tipos de archivo: 1 archivo de hojas ordenadas según su criterio correspondiente 
y 1 archivo de nodos internos con punteros a otros nodos u hojas.

## Requirements / Requisitos

- Python y librerías pandas, seaborn, matplotlib, scipy, csv
- Jupyter Notebook 
- Compilador de C++ (como g++)
- Comando make

## Instrucciones

Teniendo make correr
```bash
$ make
```
Para crear todos los ejecutables.


Para probar los métodos en conjuntos desde los 2^10 a los 2^25 rectángulos y 100 consultas uniformemente distribuidos con semillas aleatorias ejecutar
```bash
$ sudo ./main.exe`
```
El cual creara los archivos binarios `rects.bin`, `querys.bin`, `sortedRects*.bin` y archivos `*RTree.bin` donde * es el algoritmo con el que se formó el RTree
menos 1, estos ultimos contienen los nodos internos del árbol, y la raíz siempre estará en aquel con el número mayor. \
Una vez creados estos archivos, utilizara el método de búsqueda implementado en `/estructuras/RTree.cpp` para encontrar todos los rectángulos que intersecten con la
consulta y los irá guardando en archivos de resultados y en un csv con los tiempos e IO/s totales de cada query.

También es posible utilizar semillas fijas corriendolo con
```bash
$ sudo ./main.exe <Semilla para R> <Semilla para Q>
```

Notar que sudo es necesario para ejecutar el comando `echo 3 > /proc/sys/vm/drop_caches`, si se está en windows, es necesario comentar las líneas 56, 74 y 92 para ejecutar el programa (no se limpiará la caché de archivos).

Para hacer limpieza de archivos al recompilar en linux ejectuar
```bash
$ make clean
```

Y en windows
```bash
$ make wclean
```

Se pueden borrar por separado los archivos binarios ejecutando
```bash
$ make cleanBin
```
O en windows
```bash
$ make wcleanBin
```
