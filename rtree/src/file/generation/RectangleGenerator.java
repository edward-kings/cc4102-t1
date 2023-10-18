package file.generation;

import structs.Rectangle;

import java.io.*;
import java.util.Random;

import static java.lang.Integer.min;

public class RectangleGenerator {
    public static void generate(String filename, boolean eraseContents, int amountOfRectangles, int seed, boolean isSetQ) throws IOException {
        FileOutputStream fileStream = new FileOutputStream(filename, false);
        ObjectOutputStream rectStream = new ObjectOutputStream(fileStream);
        int upperBound = isSetQ? 100001 : 101;
        Random rand = new Random();
        if (seed != 0) rand.setSeed(seed);
        Rectangle[] rectangleArray = new Rectangle[amountOfRectangles];
        for (int i = 0; i < amountOfRectangles; i++) {
            int x1 = rand.nextInt(0,500001);
            int y1 = rand.nextInt(0,500001);
            int x2 = x1 + rand.nextInt(0,upperBound);
            x2 = min(x2, 500000);
            int y2 = y1 + rand.nextInt(0, upperBound);
            y2 = min(y2,500000);
            Rectangle rect = new Rectangle(x1,y1,x2,y2);
            rectangleArray[i] = rect;
        }
        rectStream.writeObject(rectangleArray);
        fileStream.close();
        rectStream.close();
    }
}
