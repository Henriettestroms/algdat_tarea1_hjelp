## Estructura del Repositorio `INF-2025-1-TAREA-1`

Este repositorio contiene la documentación, el código fuente y las instrucciones necesarias para la realización de la Tarea 1 de la asignatura *INF221 Algoritmos y Complejidad*.

A continuación, se describe la estructura del repositorio:

```bash
├── assignment_statement
├── code
├── report
└── README.md
```

### `assignment_statement`
Contiene el enunciado de la tarea, así como los archivos fuente del enunciado en formato LaTeX (.tex).

### `code`
Contiene la plantilla de los archivos que deberán estar presentes en la entrega de la tarea. En ella, se deben implementar los algoritmos solicitados, los cuales se detallan a continuación:
- `matrix_multiplication`: Algoritmo de Strassen y versión Naive.
- `sorting`: std::sort, merge sort, quick sort, insertion sort y panda sort.

### `report`
En esta carpeta se encuentra la plantilla en LaTeX para la elaboración del informe correspondiente a la tarea.

***Para obtener información más detallada sobre el contenido de cada carpeta, se recomienda revisar los archivos README ubicados dentro de ellas.***

# Documentación

## Multiplicación de matrices

### Programa principal
`matrix_multiplication.cpp` implementa el flujo principal para la multiplicación de matrices. El programa carga las matrices de entrada y decide qué algoritmo utilizar para el cálculo. Para el enfoque tradicional, se apoya en `naive.cpp`, mientras que para mejoras de rendimiento emplea la implementación de Strassen ubicada en `strassen.cpp`.

### Scripts
`matrix_generator.py` genera matrices de distintos tamaños y densidades para las pruebas. Otros scripts permiten automatizar la creación de casos de prueba y medir los tiempos de ejecución de cada algoritmo.

## Ordenamiento de arreglo unidimensional

### Programa principal
`sorting.cpp` coordina la ejecución de los distintos algoritmos de ordenamiento, delegando el trabajo en módulos específicos como `insertionsort.cpp`, `mergesort.cpp`, `quicksort.cpp` y `pandasort.cpp`.

### Scripts
`array_generator.py` crea arreglos de prueba con diversas configuraciones. Adicionalmente, hay scripts que recogen los tiempos de ejecución de cada método y generan gráficos comparativos a partir de esos datos.


