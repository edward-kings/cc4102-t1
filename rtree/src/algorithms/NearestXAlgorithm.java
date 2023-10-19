package algorithms;

import structs.RTreeNode;
import structs.Rectangle;

import java.util.Arrays;

/**
 * Algoritmo de construcción de RTree que ordena los rectángulos por su centro en X
 */
public class NearestXAlgorithm extends AbstractRTreeAlgorithm {
    public NearestXAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "NearestX";
    }

    @Override
    public void rectPreProcessing(Rectangle[] rects) {
        Arrays.sort(rects,Rectangle.compareByXCenter());
    }

    @Override
    public void nodePreProcessing(RTreeNode[] nodes) {
        Arrays.sort(nodes,(RTreeNode node1, RTreeNode node2) -> Rectangle.compareByXCenter().compare(node1.getMBR(), node2.getMBR()));
    }
}
