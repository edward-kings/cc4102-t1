package structs;


import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.util.Comparator;
import java.util.Objects;

/**
 * Clase que representa un rectángulo en el plano cartesiano
 * @param x1 coordenada x del vértice inferior izquierdo
 * @param y1 coordenada y del vértice inferior izquierdo
 * @param x2 coordenada x del vértice superior derecho
 * @param y2 coordenada y del vértice superior derecho
 */
public record Rectangle(int x1, int y1, int x2, int y2) {
    /**
     * @return Largo del rectángulo
     */
    public int width() { return x2 - x1;}

    /**
     * @return Ancho del rectángulo
     */
    public int height() { return y2 - y1;}

    /**
     * Tamaño en bytes de un rectángulo
     */
    public static final int SIZE = Integer.BYTES * 4;

    /**
     * @return coordenada x del centro del rectángulo
     */
    public int getXCenter() {
        return (x1 + x2) / 2;
    }

    /**
     * @return coordenada y del centro del rectángulo
     */
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

    @Override
    public int hashCode() {
        return Objects.hash(x1, y1, x2, y2);
    }

    /**
     * Hace la transformación de coordenadas cartesianas al valor "d" de la curva de hilbert con un
     * cuadrado de dimension n = 2^19
     * @return valor "d" de la curva de hilbert de este rectángulo
     */
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

    /**
     * Hace la rotación de las coordenadas de acuerdo a la curva de hilbert
     * @param n tamaño del cuadrado que contiene al fractal
     * @param x coordenada x del centro del rectángulo
     * @param y coordenada y del centro del rectángulo
     * @param rx deterimina si se debe rotar o no la coordenada x
     * @param ry deterimina si se debe rotar o no la coordenada y
     */
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

    /**
     * Compara dos rectángulos por su coordenada x del centro
     * @return un comparador de rectángulos por su coordenada x del centro
     */
    public static Comparator<Rectangle> compareByXCenter() {
        return Comparator.comparing(Rectangle::getXCenter);
    }

    /**
     * Compara dos rectángulos por su coordenada y del centro
     * @return un comparador de rectángulos por su coordenada y del centro
     */
    public static Comparator<Rectangle> compareByYCenter() {
        return Comparator.comparing(Rectangle::getYCenter);
    }

    /**
     * Compara dos rectángulos por su valor "d" de la curva de hilbert
     * @return un comparador de rectángulos por su valor "d" de la curva de hilbert
     */
    public static Comparator<Rectangle> compareByHilbertD() {
        return Comparator.comparing(Rectangle::coordsToHilbertD);
    }

    /**
     * @param smallerRect Rectangulo con el que se quiere saber si hay interseccion
     * @return true si hay interseccion, false en caso contrario
     * @see java.awt.Rectangle#intersects(java.awt.Rectangle)
     */
    public boolean intersects(Rectangle smallerRect) {
        // Interseccion
        boolean xOverlap = this.x2 >= smallerRect.x1() && this.x1 <= smallerRect.x2();
        boolean yOverlap = this.y2 >= smallerRect.y1() && this.y1 <= smallerRect.y2();

        boolean inside = this.x1 < smallerRect.x1() && smallerRect.x2() < this.x2 && this.y1 < smallerRect.y1() && smallerRect.y2() < this.y2;

        return (xOverlap && yOverlap) || inside;
    }

    /**
     * Escribe en un DataOutputStream los 4 enteros que representan a un rectángulo
     * @param dos DataOutputStream en el que se escribirán los 4 enteros
     * @throws IOException si ocurre un error al escribir en el DataOutputStream
     */
    public void serialize(DataOutputStream dos) throws IOException {
        dos.writeInt(x1);
        dos.writeInt(y1);
        dos.writeInt(x2);
        dos.writeInt(y2);
    }

    /**
     * Lee de un DataInputStream los 4 enteros que representan a un rectángulo
     * @param dis DataInputStream del que se leerán los 4 enteros
     * @return un objeto Rectangle con los 4 enteros leídos
     * @throws IOException si ocurre un error al leer del DataInputStream
     */
    public static Rectangle deserialize(DataInputStream dis) throws IOException {
        int x1 = dis.readInt();
        int y1 = dis.readInt();
        int x2 = dis.readInt();
        int y2 = dis.readInt();
        return new Rectangle(x1, y1, x2, y2);
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
