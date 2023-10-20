# cc4102-t1
Tarea para el curso de Diseño y Análisis de Algoritmos (CC4102). Universidad de Chile

Este repositorio compara 3 métodos de construcción de R-Trees: Nearest X, Hilbert y Sort Tile Recursive.

El R-Tree esta formado por dos tipos de archivo: 1 archivo de hojas ordenadas según su criterio correspondiente 
y 1 archivo de nodos internos con punteros a otros nodos u hojas.

## Requirements / Requisitos

- Python y librerías pandas, seaborn, matplotlib, scipy, csv
- Jupyter Notebook 
- IDE y compilador de Java
- Tener más de 4 GB de RAM

## Instrucciones

Abrir su IDE de Java de preferencia y agregar a los `vmArguments` el valor `-Xmx3G` para tener memoria suficiente en el heap al correr el programa

Al compilar el programa y correr la función Main se crearan archivos que contendran entre los 2^10 a los 2^25 rectángulos y 100 consultas uniformemente distribuidos con semillas aleatorias.
Por defecto se asume un tamaño de disco de 4096 bytes, esto se puede cambiar pasandole argumentos de consola al programa (hacia `argv`). El primer argumento es el tamaño de disco deseado, el segundo, la semilla para el conjunto R y el tercero la semilla para el conjunto Q.
Los rectangulos se guardarán en los archivos binarios `rects.bin`, `querys.bin`, `sortedRects*.bin` y archivos `*RTree.bin` donde * es el algoritmo con el que se formó el RTree.
Una vez creados estos archivos, utilizará el método de búsqueda implementado en `rtree/src/structs/RTree.java` para encontrar todos los rectángulos que intersecten con la consulta y los irá guardando en archivos de resultados y en un csv con los tiempos e IO/s totales de cada query.
