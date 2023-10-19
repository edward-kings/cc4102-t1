package structs;

import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.util.Objects;

/**
 * Clase que representa un nodo del RTree
 */
public class RTreeNode {
    /**
     * Rectángulo que representa el MBR (Minimum Bounding Rectangle) del nodo
     */
    Rectangle MBR;
    /**
     * Índice del primer hijo del nodo en el archivo de nodos u hojas
     */
    private final long firstChildIndex;
    /**
     * Índice del último hijo del nodo en el archivo de nodos u hojas
     */
    private final long lastChildIndex;
    /**
     * Tamaño en bytes de un nodo del RTree
     */
    public static final int SIZE = Rectangle.SIZE + Long.BYTES * 2;

    /**
     * Constructor de un nodo del RTree
     * @param MBR MBR del nodo
     * @param firstChildIndex índice del primer hijo del nodo en el archivo de nodos u hojas
     * @param lastChildIndex índice del último hijo del nodo en el archivo de nodos u hojas
     */
    public RTreeNode(Rectangle MBR, long firstChildIndex, long lastChildIndex) {
        this.MBR = MBR;
        this.firstChildIndex = firstChildIndex;
        this.lastChildIndex = lastChildIndex;
    }

    /**
     * Escribe los 4 enteros que representan el MBR del nodo y los dos índices de los hijos del nodo en el archivo de nodos
     * @param dos DataOutputStream que representa el archivo de nodos
     * @throws Exception si ocurre un error al escribir en el archivo
     */
    public void serialize(DataOutputStream dos) throws Exception {
        this.MBR.serialize(dos);
        dos.writeLong(this.firstChildIndex);
        dos.writeLong(this.lastChildIndex);
    }

    /**
     * Lee los 4 enteros que representan el MBR del nodo y los dos índices de los hijos del nodo en el archivo de nodos
     * @param dis DataInputStream que representa el archivo de nodos
     * @return un objeto RTreeNode con su MBR y los índices de sus hijos
     * @throws Exception si ocurre un error al leer del archivo
     */
    public static RTreeNode deserialize(DataInputStream dis) throws Exception {
        Rectangle MBR = Rectangle.deserialize(dis);
        long firstChildIndex = dis.readLong();
        long lastChildIndex = dis.readLong();
        return new RTreeNode(MBR, firstChildIndex, lastChildIndex);
    }

    public Rectangle getMBR() {
        return MBR;
    }

    public long getFirstChildIndex() {
        return firstChildIndex;
    }

    public long getLastChildIndex() {
        return lastChildIndex;
    }

    @Override
    public String toString() {
        return "RTreeNode{" +
                "MBR=" + MBR +
                ", firstChildIndex=" + firstChildIndex +
                ", lastChildIndex=" + lastChildIndex +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        RTreeNode rTreeNode = (RTreeNode) o;
        return firstChildIndex == rTreeNode.firstChildIndex && lastChildIndex == rTreeNode.lastChildIndex && Objects.equals(MBR, rTreeNode.MBR);
    }

    @Override
    public int hashCode() {
        return Objects.hash(MBR, firstChildIndex, lastChildIndex);
    }
}
