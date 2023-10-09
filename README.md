# cc4102-t1
Tarea para el curso de Diseño y Análisis de Algoritmos (CC4102). Universidad de Chile

Actualmente el repo consta de funciones utilitarias encargadas de crear los conjuntos de rectangulos del enunciado R y Q.

También está lista la implementación del algoritmo de construcción Nearest X con ordenamiento a través de merge sort.

El R-Tree esta formado por dos tipos de archivo: 1 archivo de hojas ordenadas según su criterio correspondiente 
y k archivos de nodos internos según la altura del árbol.

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


Para probar el método Nearest X en un conjunto de 1024 rectángulos y 100 consultas uniformemente distribuidos con semillas fijas ejecutar
```bash
$ ./main.exe`
```
El cual creara los archivos binarios `rects.bin`, `querys.bin`, `sortedRects.bin` y archivos `NearestXRTree*.bin` donde * es un número desde 0 hasta la altura del árbol 
menos 1, estos ultimos contienen los nodos internos del árbol, y la raíz siempre estará en aquel con el número mayor. \
Una vez creados estos archivos, utilizara el método de búsqueda implementado en `/estructuras/RTree.cpp` para encontrar todos los rectángulos que intersecten con la
consulta y los ira guardando en archivos de resultados y en un csv con los tiempos e IO/s totales de cada query.



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
