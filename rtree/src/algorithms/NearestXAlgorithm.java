package algorithms;

import structs.Rectangle;

import java.util.Arrays;

public class NearestXAlgorithm extends AbstractTreeAlgorithm {
    public NearestXAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        super(numberOfRects, maxChildrenPerNode);
    }

    @Override
    public String getAlgorithmName() {
        return "NearestX";
    }

    @Override
    public void preProcessing(Rectangle[] rects) {
        Arrays.sort(rects,Rectangle.compareByXCenter());
    }
}
