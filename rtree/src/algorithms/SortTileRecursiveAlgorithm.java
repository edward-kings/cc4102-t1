package algorithms;

import structs.RTreeNode;
import structs.Rectangle;

import java.util.Arrays;

import static java.lang.Math.*;

/**
 * Algoritmo de construcción de RTree que ordena los rectángulos por su centro en X y luego por su centro en Y
 */
public class SortTileRecursiveAlgorithm extends AbstractRTreeAlgorithm {
    public SortTileRecursiveAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "SortTileRecursive";
    }

    @Override
    public void rectPreProcessing(Rectangle[] rects) {
        Arrays.sort(rects, Rectangle.compareByXCenter());
        int subSetAmount = (int) floor(sqrt((double)numberOfRects / maxChildrenPerNode));
        int subSetSize = subSetAmount * maxChildrenPerNode;
        Arrays.sort(rects, 0, subSetSize, Rectangle.compareByYCenter());
        for (int i = 1; i < subSetAmount; i++) {
            int subsetStart = subSetSize * i + 1;
            int subsetEnd = min(subSetSize * (i+1), numberOfRects);
            Arrays.sort(rects,subsetStart, subsetEnd, Rectangle.compareByYCenter());
        }
    }

    @Override
    public void nodePreProcessing(RTreeNode[] nodes) {
        Arrays.sort(nodes,(RTreeNode node1, RTreeNode node2) -> Rectangle.compareByXCenter().compare(node1.getMBR(), node2.getMBR()));
        int subSetAmount = (int) floor(sqrt((double)nodes.length / maxChildrenPerNode));
        int subSetSize = subSetAmount * maxChildrenPerNode;
        Arrays.sort(nodes, 0, subSetSize, (RTreeNode node1, RTreeNode node2) -> Rectangle.compareByYCenter().compare(node1.getMBR(), node2.getMBR()));
        for (int i = 1; i < subSetAmount; i++) {
            int subsetStart = subSetSize * i + 1;
            int subsetEnd = min(subSetSize * (i+1), nodes.length);
            Arrays.sort(nodes,subsetStart, subsetEnd, (RTreeNode node1, RTreeNode node2) -> Rectangle.compareByYCenter().compare(node1.getMBR(), node2.getMBR()));
        }
    }
}
