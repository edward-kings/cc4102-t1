import algorithms.RTreeAlgorithm;
import structs.RTreeNode;
import structs.Rectangle;

import java.io.*;
import java.util.ArrayList;


/**
 * Clase que representa un RTree
 */
public class RTree {
    private RTreeAlgorithm algorithm;
    private int totalRectBytesRead = 0;
    private int totalNodeBytesRead = 0;
    private int blockSize;
    private RandomAccessFile rectIO;
    private RandomAccessFile nodeIO;

    private DataInputStream rectInputStream;
    private DataInputStream nodeInputStream;

    /**
     * Constructor de la clase RTree
     * @param algorithm Algoritmo de construcción de RTree
     * @param rectanglesPerBlock Cantidad de rectángulos por bloque de disco
     * @param nodesPerBlock Cantidad de nodos por bloque de disco
     */
    public RTree(RTreeAlgorithm algorithm, int diskBlockSize) {
        this.algorithm = algorithm;
        this.blockSize = diskBlockSize;
    }

    public int getTotalBytesRead() {
        return totalNodeBytesRead + totalRectBytesRead;
    }

    public void resetTotalBytesRead() {
        totalRectBytesRead = 0;
        totalNodeBytesRead = 0;
    }
    public int getTotalIOs() {
        int nodeIos = (int) Math.ceil((double) totalNodeBytesRead / (double) blockSize);
        int rectIos = (int) Math.ceil((double) totalRectBytesRead / (double) blockSize);
        return nodeIos + rectIos;
    }
    /**
     * Construye el RTree a partir de un archivo de rectángulos
     * @param filename Nombre del archivo de rectángulos
     * @throws Exception Si ocurre un error al leer el archivo
     */
    public void buildTreeFromFile(String filename) throws Exception {
        this.algorithm.buildTree(filename);
        RandomAccessFile leavesFile = this.algorithm.getLeavesFile();
        RandomAccessFile treeFile = this.algorithm.getTreeFile();
        this.rectIO = leavesFile;
        this.rectInputStream = new DataInputStream(new FileInputStream(leavesFile.getFD()));
        this.nodeIO = treeFile;
        this.nodeInputStream = new DataInputStream(new FileInputStream(treeFile.getFD()));
    }

    /**
     * Busca los rectángulos que intersectan con el rectángulo query
     * @param query Área sobre la cual se realiza la búsqueda
     * @return Lista de rectángulos que intersectan con query
     * @throws Exception Si ocurre un error al leer el archivo
     */
    public ArrayList<Rectangle> search(Rectangle query) throws Exception {
        ArrayList<Rectangle> result = new ArrayList<>();
        this.searchRecursive(query, 0, 0, result);
        return result;
    }

    /**
     * Realiza la búsqueda recursiva de los rectángulos que intersectan con el rectángulo query
     * @param query Área sobre la cual se realiza la búsqueda
     * @param firstChildIndex Índice del primer hijo del nodo actual
     * @param lastChildIndex Índice del último hijo del nodo actual
     * @param result Lista de rectángulos que intersectan con query
     * @throws Exception Si ocurre un error al leer el archivo
     */
    private void searchRecursive(Rectangle query, long firstChildIndex, long lastChildIndex, ArrayList<Rectangle> result) throws Exception {
        // Si los hijos son hojas
        if (firstChildIndex < 0 || lastChildIndex < 0) {
            int rectangleByteSize = Rectangle.SIZE;
            long first = -firstChildIndex / rectangleByteSize;
            long last = -lastChildIndex / rectangleByteSize;
            long buffSize = last - first + 1;
            this.rectIO.seek(-firstChildIndex);
            for (int i = 0; i < buffSize; i++) {
                Rectangle rect = Rectangle.deserialize(rectInputStream);
                if (query.intersects(rect) || rect.intersects(query)) {
                    result.add(rect);
                }
            }
            this.totalRectBytesRead += Math.max(-lastChildIndex - -firstChildIndex, Rectangle.SIZE);
        } else {
            int nodeByteSize = RTreeNode.SIZE;
            long first = firstChildIndex / nodeByteSize;
            long last = lastChildIndex / nodeByteSize;
            long buffSize = last - first + 1;
            this.nodeIO.seek(firstChildIndex);
            for (int i = 0; i < buffSize ; i++) {
                RTreeNode node = RTreeNode.deserialize(nodeInputStream);
                if (query.intersects(node.getMBR()) || node.getMBR().intersects(query)) {
                    this.searchRecursive(query, node.getFirstChildIndex(), node.getLastChildIndex(), result);
                }
            }
            this.totalNodeBytesRead += Math.max(lastChildIndex - firstChildIndex, RTreeNode.SIZE);
        }
    }

    /**
     * Cierra los archivos abiertos
     * @throws IOException Si ocurre un error al cerrar los archivos
     */
    public void cleanup() throws IOException {
        this.algorithm.cleanup();
        this.nodeIO.close();
        this.rectIO.close();
        this.nodeInputStream.close();
        this.rectInputStream.close();
    }
}
