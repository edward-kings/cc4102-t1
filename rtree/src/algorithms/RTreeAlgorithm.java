package algorithms;

import structs.RTreeNode;
import structs.Rectangle;

import java.io.IOException;
import java.io.RandomAccessFile;

/**
 * Interfaz para un algoritmo de construcción de RTree
 */
public interface RTreeAlgorithm {
    /**
     * Construye el RTree a partir de un archivo de rectángulos
     * @param filename Nombre del archivo de rectángulos
     * @throws Exception Si ocurre un error al leer el archivo
     */
    void buildTree(String filename) throws Exception;
    /**
     * @return El archivo de nodos internos
     */
    RandomAccessFile getTreeFile();
    /**
     * @return El archivo de hojas
     */
    RandomAccessFile getLeavesFile();
    /**
     * @return El nombre convreto del algoritmo
     */
    String getAlgorithmName();
    /**
     * Pre procesado de los rectángulos según algún criterio de ordenamiento
     * @param rects Arreglo de rectángulos a ordenar
     */
    void rectPreProcessing(Rectangle[] rects);
    /**
     * Pre procesado de los nodos según algún criterio de ordenamiento
     * @param nodes Arreglo de nodos a ordenar
     */
    void nodePreProcessing(RTreeNode[] nodes);
    /**
     * @return La cantidad de rectángulos
     */
    int getNumberOfRects();
    /**
     * Cierra los archivos abiertos
     * @throws IOException Si ocurre un error al cerrar los archivos
     */
    void cleanup() throws IOException;
}
