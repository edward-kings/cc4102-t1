# cc4102-t1
Tarea para el curso de Diseño y Análisis de Algoritmos (CC4102). Universidad de Chile

Actualmente el repo solo consta de funciones utilitarias encargadas de crear los conjuntos de rectangulos del enunciado R y Q.

Se inició el desarrollo del algoritmo Nearest X, pero no está testeado.

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

El archivo `crear.exe`, al ser ejecutado, crea los archivos `rectangulos.bin` y `querys.bin` que contienen los rectangulos de R y Q respectivamente.
Por defecto se crean 1024 rectangulos usando como semilla el numero 3 y las querys con semilla 4, para usar semillas y cantidades personalizadas, ejecutar `crear.exe` de la siguiente manera
```bash
$ ./crear.exe <Semilla R> <Semilla Q> <Cantidad de rectangulos> 
```


Para leer todos los rectangulos que caben en el primer bloque de disco ejecutar
```bash
$ ./leer.exe`
```
El cual imprimira el primer bloque de disco del archivo que contiene los rectangulos.



Para hacer limpieza de archivos al recompilar en linux ejectuar
```bash
$ make clean
```

Y en windows
```bash
$ make wclean
```

Cada ejecutable puede compilarse por separado con su regla de make respectiva
```bash
$ make crear.exe
$ make leer.exe
$ make main.exe
```