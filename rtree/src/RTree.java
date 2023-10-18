import algorithms.RTreeAlgorithm;
import file.utils.FileObjectIO;
import structs.RTreeNode;
import structs.Rectangle;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.ArrayList;

import static java.lang.Math.ceil;

public class RTree {
    private int rectanglesPerBlock;
    private int nodesPerBlock;
    private int maxChildrenPerNode;

    RTreeAlgorithm algorithm;
    private int numberOfRects;
    private int totalSearchIOs = 0;
    RandomAccessFile leavesFile;
    RandomAccessFile treeFile;

    public RTree(int maxChildrenPerNode, RTreeAlgorithm algorithm, int numberOfRects, int rectanglesPerBlock, int nodesPerBlock) {
        this.maxChildrenPerNode = maxChildrenPerNode;
        this.algorithm = algorithm;
        this.numberOfRects = numberOfRects;
        this.rectanglesPerBlock = rectanglesPerBlock;
        this.nodesPerBlock = nodesPerBlock;
    }

    public void setAlgorithm(RTreeAlgorithm algorithm) {
        this.algorithm = algorithm;
    }

    public String getAlgorithmName() {
        return this.algorithm.getAlgorithmName();
    }

    public int getTotalSearchIOs() {
        return totalSearchIOs;
    }

    public void resetTotalSearchIOs() {
        totalSearchIOs = 0;
    }

    public void buildTreeFromFile(String filename) throws IOException {
        this.algorithm.buildTree(filename);
        leavesFile = this.algorithm.getLeavesFile();
        treeFile = this.algorithm.getTreeFile();
    }

    public ArrayList<Rectangle> search(Rectangle query) throws IOException {
        ArrayList<Rectangle> result = new ArrayList<>();
        this.searchRecursive(query, 0, 0, result);
        return result;
    }

    private void searchRecursive(Rectangle query, long firstChildIndex, long lastChildIndex, ArrayList<Rectangle> result) throws IOException {
        // Si los hijos son hojas
        if (firstChildIndex < 0 || lastChildIndex < 0) {
            FileObjectIO<Rectangle> rectIo = new FileObjectIO<>(new Rectangle(0,0,0,0));
            int rectangleByteSize = rectIo.calculateObjectSize();
            long first = -firstChildIndex / rectangleByteSize;
            long last = -lastChildIndex / rectangleByteSize;
            long buffSize = last - first + 1;
            Rectangle[] rectBuffer;
            rectBuffer = rectIo.readObjectArrayFromFile(leavesFile, (int) firstChildIndex);
            this.totalSearchIOs += (int)ceil((double)buffSize / rectanglesPerBlock);
            for (int i = 0; i < buffSize; i++) {
                if (query.intersects(rectBuffer[i])) {
                    result.add(rectBuffer[i]);
                }
            }
        } else {
            FileObjectIO<RTreeNode> nodeIo = new FileObjectIO<>(new RTreeNode(new Rectangle(0,0,0,0),0,0));
            int nodeByteSize = nodeIo.calculateObjectSize();
            long first = firstChildIndex / nodeByteSize;
            long last = lastChildIndex / nodeByteSize;
            long buffSize = last - first + 1;
            RTreeNode[] nodeBuffer = new RTreeNode[(int) buffSize];
            for (int i = 0; i < buffSize ; i++) {
                nodeBuffer[i] = nodeIo.readObjectFromFile(treeFile, (int) firstChildIndex);
            }
            this.totalSearchIOs += (int)ceil((double)buffSize / nodesPerBlock);
            for (int i = 0; i < buffSize; i++) {
                if (query.intersects(nodeBuffer[i].getMBR())) {
                    this.searchRecursive(query, nodeBuffer[i].getFirstChildIndex(), nodeBuffer[i].getLastChildIndex(), result);
                }
            }
        }
    }

    public void cleanup() throws IOException {
        this.algorithm.cleanup();
    }
}
