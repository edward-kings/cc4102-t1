package file.generation;

import structs.Rectangle;

import java.io.*;
import java.util.Random;

import static java.lang.Integer.min;

/**
 * Clase para generar un archivo de rectángulos
 */
public class RectangleGenerator {
    /**
     * Genera un archivo de rectángulos según las restricciones del enunciado
     * @param filename Nombre del archivo
     * @param eraseContents Si se debe sobreescribir el archivo
     * @param amountOfRectangles Cantidad de rectángulos a generar
     * @param seed Semilla para el generador de números aleatorios
     * @param isSetQ Si se debe generar un set Q de consultas o un set R de rectángulos
     * @throws IOException Si ocurre un error al escribir el archivo
     */
    public static void generate(String filename, boolean eraseContents, int amountOfRectangles, int seed, boolean isSetQ) throws IOException {
        try(FileOutputStream fileStream = new FileOutputStream(filename, !eraseContents);
        DataOutputStream rectStream = new DataOutputStream(fileStream)) {
            int upperBound = isSetQ ? 100001 : 101;
            Random rand = new Random();
            if (seed != 0) rand.setSeed(seed);
            for (int i = 0; i < amountOfRectangles; i++) {
                int x1 = rand.nextInt(0, 500001);
                int y1 = rand.nextInt(0, 500001);
                int x2 = x1 + rand.nextInt(0, upperBound);
                x2 = min(x2, 500000);
                int y2 = y1 + rand.nextInt(0, upperBound);
                y2 = min(y2, 500000);
                Rectangle rect = new Rectangle(x1, y1, x2, y2);
                rect.serialize(rectStream);
            }
        }
    }
}
