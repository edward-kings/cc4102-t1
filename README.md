# cc4102-t1
Tarea para el curso de Diseño y Análisis de Algoritmos (CC4102). Universidad de Chile

Actualmente el repo solo consta de funciones utilitarias encargadas de crear los conjuntos de rectangulos del enunciado R y Q

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
Para crear el archivo `crear.exe`, al ser ejecutado, crea los archivos `rectangulos.bin` y `querys.bin` que contienen los rectangulos de R y Q respectivamente


Para leer todos los rectangulos que caben en el primer bloque de disco ejecutar
```bash
$ make -f Makeleer
```
El cual creara el archivo `leer.exe`.



Para hacer limpieza de archivos al recompilar en linux ejectuar
```bash
$ make clean
$ make -f Makeleer clean
```

Y en windows
```bash
$ make wclean
$ make -f Makeleer wclean
```