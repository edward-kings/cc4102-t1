package structs;

import java.io.Serializable;
import java.util.Comparator;

public class Rectangle implements Serializable {
    public Rectangle(int x1, int y1, int x2, int y2) {
        this.x1 = x1;
        this.y1 = y1;
        this.x2 = x2;
        this.y2 = y2;
    }
    private final int x1;
    public int getX1() {
        return x1;
    }
    private final int y1;

    public int getY1() {
        return y1;
    }

    private final int x2;
    public int getX2() {
        return x2;
    }

    private final int y2;

    public int getY2() {
        return y2;
    }

    public int getXCenter() {
        return (x1 + x2) / 2;
    }

    public int getYCenter() {
        return (y1 + y2) / 2;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Rectangle rectangle = (Rectangle) o;
        return x1 == rectangle.x1 && y1 == rectangle.y1 && x2 == rectangle.x2 && y2 == rectangle.y2;
    }

    private long coordsToHilbertD() {
        int n = 524288;
        int rx, ry;
        long d = 0;
        int[] xAsArray = {this.getXCenter()};
        int[] yAsArray = {this.getYCenter()};
        for (long s = n / 2; s > 0; s /= 2) {
            rx = (xAsArray[0] & s) > 0 ? 1 : 0;
            ry = (yAsArray[0] & s) > 0 ? 1 : 0;
            d += s * s * ((3 * rx) ^ ry);
            rotate(s, xAsArray, yAsArray, rx, ry);
        }
        return d;
    }

    private void rotate(long n, int[] x, int[] y, int rx, int ry) {
        int t;
        if (ry == 0) {
            if (rx == 1) {
                x[0] = (int) (n - 1 - x[0]);
                y[0] = (int) (n - 1 - y[0]);
            }
            t = x[0];
            x[0] = y[0];
            y[0] = t;
        }
    }

    public static Comparator<Rectangle> compareByXCenter() {
        return Comparator.comparing(Rectangle::getXCenter);
    }

    public static Comparator<Rectangle> compareByYCenter() {
        return Comparator.comparing(Rectangle::getYCenter);
    }

    public static Comparator<Rectangle> compareByHilbertD() {
        return Comparator.comparing(Rectangle::coordsToHilbertD);
    }

    public boolean intersects(Rectangle smallerRect) {
        boolean xOverlap = this.x2 >= smallerRect.getX1() && this.x1 <= smallerRect.getX2();
        boolean yOverlap = this.y2 >= smallerRect.getY1() && this.y1 <= smallerRect.getY2();

        return xOverlap && yOverlap;
    }

    @Override
    public String toString() {
        return "Rectangle{" +
                "x1=" + x1 +
                ", y1=" + y1 +
                ", x2=" + x2 +
                ", y2=" + y2 +
                '}';
    }
}
