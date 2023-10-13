#ifndef BINARY_FILE_FUNCTIONS
#define BINARY_FILE_FUNCTIONS

#include <cstdio>
#include <string>
#include "../estructuras/Rect.hpp"
#include "../estructuras/RTreeNode.hpp"
#include "../algoritmos/RTreeAlgorithm.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/statvfs.h>
#endif


/**
 * @brief Contiene el tamaño de bloque del disco.
*/
extern unsigned int diskBlockSize;

/**
 * @brief Contiene la cantidad de rectángulos que caben en un bloque del disco.
*/
extern unsigned int rectanglesPerBlock;

/**
 * @brief Contiene la cantidad de nodos internos que caben en un bloque del disco.
*/
extern unsigned int nodesPerBlock;

/**
 * @brief Escribe la cantidad de rectángulos que caben en un bloque del disco desde la variable buffer.
 * \param file Archivo en el que se escribirá.
 * \param buffer Variable en la que se almacenan los rectángulos.
 * \param rectanglesToWrite Cantidad de rectángulos a escribir si no es una página completa.
 * \return Cantidad de bytes escritos.
*/
unsigned int binRectPageWrite(std::ofstream& file, Rect* buffer, unsigned int rectanglesToWrite = rectanglesPerBlock);

/**
 * @brief Lee la cantidad de rectángulos que caben en un bloque del disco en la variable buffer.
 * \param file Archivo del que se leerá.
 * \param buffer Variable en la que se almacenan los rectángulos.
 * \param rectanglesToRead Cantidad de rectángulos a leer si no es una página completa.
 * \return Cantidad de bytes leídos.
*/
unsigned int binRectPageRead(std::ifstream& file, Rect* buffer, unsigned int rectanglesToRead = rectanglesPerBlock);

/**
 * @brief Escribe la cantidad de nodos internos que caben en un bloque del disco desde la variable buffer.
 * \param file Archivo en el que se escribirá.
 * \param buffer Variable en la que se almacenan los nodos internos.
 * \param nodesToWrite Cantidad de nodos internos a escribir si no es una página completa.
 * \return Cantidad de bytes escritos.
*/
unsigned int binNodePageWrite(std::ofstream& file, RTreeNode* buffer, unsigned int nodesToWrite = nodesPerBlock);

/**
 * @brief Lee la cantidad de nodos internos que caben en un bloque del disco en la variable buffer.
 * \param file Archivo del que se leerá.
 * \param buffer Variable en la que se almacenan los nodos internos.
 * \param nodesToRead Cantidad de nodos internos a leer si no es una página completa.
 * \return Cantidad de bytes leídos.
*/
unsigned int binNodePageRead(std::ifstream& file, RTreeNode* buffer, unsigned int nodesToRead = nodesPerBlock);


#endif
