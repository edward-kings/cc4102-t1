package algorithms;

import structs.Rectangle;

import java.util.Arrays;
import java.util.Comparator;

public class HilbertAlgorithm extends AbstractTreeAlgorithm {
    public HilbertAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "Hilbert";
    }
    @Override
    public void preProcessing(Rectangle[] rects) {
        Arrays.sort(rects,Rectangle.compareByHilbertD());
    }
}
