package algorithms;

import structs.RTreeNode;
import structs.Rectangle;

import java.util.Arrays;

/**
 * Algoritmo de construcción de RTree que ordena los rectángulos según su valor en la curva de Hilbert
 */
public class HilbertAlgorithm extends AbstractRTreeAlgorithm {
    public HilbertAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "Hilbert";
    }
    @Override
    public void rectPreProcessing(Rectangle[] rects) {
        Arrays.sort(rects,Rectangle.compareByHilbertD());
    }

    public void nodePreProcessing(RTreeNode[] nodes) {
        Arrays.sort(nodes,(RTreeNode node1, RTreeNode node2) -> Rectangle.compareByHilbertD().compare(node1.getMBR(), node2.getMBR()));
    }
}
