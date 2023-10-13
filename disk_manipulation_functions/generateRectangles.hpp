#ifndef GENERATE_RECTANGLES
#define GENERATE_RECTANGLES

#include "binaryFileFunctions.hpp"
#include <string.h>

/**
 * @brief Genera un archivo binario con rectángulos aleatorios.
 * \param filename Nombre del archivo a generar.
 * \param eraseContents Booleano que indica si se debe sobreescribir el archivo o no.
 * \param amountOfRectangles Cantidad de rectángulos a generar.
 * \param seed Semilla para el generador de números aleatorios.
 * \param isSetQ Booleano que indica si se debe generar un conjunto de Queries o no.
*/
void generateRectanglesFile(std::string filename, bool eraseContents, unsigned int amountOfRectangles, unsigned int seed, bool isSetQ);

#endif
