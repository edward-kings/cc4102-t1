package algorithms;

import structs.Rectangle;

import java.util.Arrays;

import static java.lang.Math.*;

public class SortTileRecursiveAlgorithm extends AbstractTreeAlgorithm {
    public SortTileRecursiveAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "SortTileRecursive";
    }

    @Override
    public void preProcessing(Rectangle[] rects) {
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
}
