package algorithms;

import structs.Rectangle;

import java.io.IOException;
import java.io.RandomAccessFile;

public interface RTreeAlgorithm {
    void buildTree(String filename) throws IOException;
    RandomAccessFile getTreeFile();
    RandomAccessFile getLeavesFile();
    String getAlgorithmName();
    void preProcessing(Rectangle[] rects);

    void cleanup() throws IOException;
}
