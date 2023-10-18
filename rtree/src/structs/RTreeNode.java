package structs;

import java.io.Serializable;

public class RTreeNode implements Serializable {
    Rectangle MBR;
    long firstChildIndex;
    long lastChildIndex;

    public RTreeNode(Rectangle MBR, long firstChildIndex, long lastChildIndex) {
        this.MBR = MBR;
        this.firstChildIndex = firstChildIndex;
        this.lastChildIndex = lastChildIndex;
    }

    public Rectangle getMBR() {
        return MBR;
    }

    public long getFirstChildIndex() {
        return firstChildIndex;
    }

    public long getLastChildIndex() {
        return lastChildIndex;
    }
}
