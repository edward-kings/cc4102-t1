package algorithms;

import file.utils.FileObjectIO;
import structs.RTreeNode;
import structs.Rectangle;

import java.io.*;
import java.util.ArrayList;
import java.util.Arrays;

import static java.lang.Integer.min;
import static java.lang.Math.ceil;

public abstract class AbstractTreeAlgorithm implements RTreeAlgorithm {
    private RandomAccessFile leavesFileIn = null;
    private RandomAccessFile treeFileIn = null;

    protected int numberOfRects;
    protected int maxChildrenPerNode;

    public AbstractTreeAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        this.numberOfRects = numberOfRects;
        this.maxChildrenPerNode = maxChildrenPerNode;
    }
    @Override
    public void buildTree(String filename) throws IOException {
        Rectangle[] rects;
        try(FileInputStream fileInputStream = new FileInputStream(filename);
            ObjectInputStream rectInputStream = new ObjectInputStream(fileInputStream)) {
            rects = (Rectangle[]) rectInputStream.readObject();
        } catch (IOException | ClassNotFoundException e) {
            throw new RuntimeException(e);
        }
        this.preProcessing(rects);
        String leavesFileName = "sortedRects" + this.getAlgorithmName() + ".bin";
        try (FileOutputStream fileOutput = new FileOutputStream(leavesFileName);
            ObjectOutputStream rectOutput = new ObjectOutputStream(fileOutput)) {
            rectOutput.writeObject(rects);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        leavesFileIn = new RandomAccessFile(leavesFileName, "r");
        // amountOfNodes = n/M: cantidad de nodos internos
        int amountOfNodes = (int) ceil((double)numberOfRects / maxChildrenPerNode);
        int nodeNumber = 0;
        int currentRect = 0;
        // Indica la altura actual del árbol
        int currentLevel = 0;
        ArrayList<RTreeNode[]> nodeBuffer = new ArrayList<>();
        nodeBuffer.add(new RTreeNode[amountOfNodes]);
        FileObjectIO<Rectangle> objectIO = new FileObjectIO<>(new Rectangle(0,0,0,0));
        int rectByteSize = objectIO.calculateObjectSize();
        while (nodeNumber < amountOfNodes) {
            long firstChildIndex =  - ((long) currentRect * rectByteSize);
            currentRect = min(currentRect + maxChildrenPerNode - 1, numberOfRects - 1);
            long lastChildIndex =  - ((long) currentRect * rectByteSize);
            RTreeNode node = new RTreeNode(rects[currentRect],firstChildIndex,lastChildIndex);
            nodeBuffer.get(currentLevel)[nodeNumber] = node;
            currentRect++;
            nodeNumber++;
        }
        currentLevel++;
        int currentNodeIndex = 0;
        // Mientras queden nodos por crear
        FileObjectIO<RTreeNode> nodeObjectIO = new FileObjectIO<>(new RTreeNode(new Rectangle(0,0,0,0),0,0));
        int nodeByteSize = nodeObjectIO.calculateObjectSize();
        while (amountOfNodes > 1) {
            amountOfNodes = (int) ceil((double)amountOfNodes / maxChildrenPerNode);
            nodeBuffer.add(new RTreeNode[amountOfNodes]);
            for (int i = 0; i < amountOfNodes; i++) {
                long firstChildIndex = (long) (currentNodeIndex + 1) * nodeByteSize * currentLevel;
                currentNodeIndex = min((currentNodeIndex + maxChildrenPerNode - 1),  nodeBuffer.get(currentLevel -1).length - 1);
                long lastChildIndex = (long) (currentNodeIndex + 1) * nodeByteSize * currentLevel;
                RTreeNode node = new RTreeNode(nodeBuffer.get(currentLevel - 1)[currentNodeIndex].getMBR(),firstChildIndex,lastChildIndex);
                nodeBuffer.get(currentLevel)[i] = node;
                currentNodeIndex ++;
            }
            currentLevel++;
            currentNodeIndex = 0;
        }
        FileOutputStream algorithmTreeFile = new FileOutputStream(this.getAlgorithmName() + "RTree.bin", false);
        for (int i = nodeBuffer.size() - 1; i >= 0; i--) {
            RTreeNode[] currentArray = Arrays.copyOf(nodeBuffer.get(i), nodeBuffer.get(i).length);
            nodeObjectIO.writeObjectsToFile(algorithmTreeFile, currentArray);
        }
        algorithmTreeFile.close();
        this.treeFileIn = new RandomAccessFile(this.getAlgorithmName() + "RTree.bin","r");

    }

    @Override
    public RandomAccessFile getTreeFile() {
        return treeFileIn;
    }

    @Override
    public RandomAccessFile getLeavesFile() {
        return leavesFileIn;
    }

    @Override
    public abstract String getAlgorithmName();

    @Override
    public abstract void preProcessing(Rectangle[] rects);

    @Override
    public void cleanup() throws IOException {
        leavesFileIn.close();
        treeFileIn.close();
    }
}
