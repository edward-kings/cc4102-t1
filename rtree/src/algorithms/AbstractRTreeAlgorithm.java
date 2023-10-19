package algorithms;


import structs.RTreeNode;
import structs.Rectangle;

import java.io.*;
import java.util.ArrayList;

import static java.lang.Integer.min;
import static java.lang.Math.ceil;

/**
 * Clase abstracta para un algoritmo de construcción de RTree
 */
public abstract class AbstractRTreeAlgorithm implements RTreeAlgorithm {
    private RandomAccessFile leavesFileIn = null;
    private RandomAccessFile treeFileIn = null;

    protected int numberOfRects;
    protected int maxChildrenPerNode;
    /**
     * Constructor de la clase AbstractRTreeAlgorithm
     * @param numberOfRects Cantidad de rectángulos
     * @param maxChildrenPerNode Cantidad máxima de hijos por nodo
     */
    public AbstractRTreeAlgorithm(int numberOfRects, int maxChildrenPerNode) {
        this.numberOfRects = numberOfRects;
        this.maxChildrenPerNode = maxChildrenPerNode;
    }
    @Override
    public void buildTree(String filename) throws Exception {
        Rectangle[] rects = new Rectangle[numberOfRects];
        try(FileInputStream fileInputStream = new FileInputStream(filename);
            DataInputStream rectInputStream = new DataInputStream(fileInputStream)) {
            for (int i = 0; i < numberOfRects; i++) {
                Rectangle rect = Rectangle.deserialize(rectInputStream);
                rects[i] = rect;
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        this.rectPreProcessing(rects);
        String leavesFileName = "sortedRects" + this.getAlgorithmName() + ".bin";
        try (FileOutputStream fileOutput = new FileOutputStream(leavesFileName);
            DataOutputStream rectOutput = new DataOutputStream(fileOutput)) {
            for(Rectangle rect: rects) {
                rect.serialize(rectOutput);
            }
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
        int rectByteSize = Rectangle.SIZE;
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
        int nodeByteSize = RTreeNode.SIZE;
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
            this.nodePreProcessing(nodeBuffer.get(currentLevel));
            currentLevel++;
            currentNodeIndex = 0;
        }
        try (FileOutputStream algorithmTreeFile = new FileOutputStream(this.getAlgorithmName() + "RTree.bin");
             DataOutputStream nodeObjectIO = new DataOutputStream(algorithmTreeFile)) {
            for( int i = nodeBuffer.size() - 1; i >=0; i--) {
                for (RTreeNode node : nodeBuffer.get(i)) {
                    node.serialize(nodeObjectIO);
                }
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
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
    public abstract void rectPreProcessing(Rectangle[] rects);

    @Override
    public abstract void nodePreProcessing(RTreeNode[] rects);

    @Override
    public void cleanup() throws IOException {
        leavesFileIn.close();
        treeFileIn.close();
    }

    @Override
    public int getNumberOfRects() {
        return numberOfRects;
    }
}
