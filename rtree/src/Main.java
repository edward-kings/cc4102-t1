import algorithms.HilbertAlgorithm;
import algorithms.NearestXAlgorithm;
import algorithms.RTreeAlgorithm;
import algorithms.SortTileRecursiveAlgorithm;
import file.generation.RectangleGenerator;
import file.utils.FileObjectIO;
import file.utils.PerformanceStats;
import file.utils.StatsContainer;
import structs.RTreeNode;
import structs.Rectangle;

import java.io.*;
import java.time.Duration;
import java.time.Instant;
import java.util.ArrayList;

public class Main {

    public static ArrayList<Rectangle> executeQuery(RTree tree, Rectangle query, PerformanceStats stats) throws IOException, InterruptedException {
        boolean isWindows = System.getProperty("os.name").toLowerCase().startsWith("windows");
        if (!isWindows) {
            String[] command = new String[2];
            command[0] = "/bin/sh";
            command[1] = "echo 3 > /proc/sys/vm/drop_caches";
            Process process = Runtime.getRuntime().exec(command);
            process.waitFor();
        }
        Instant before = Instant.now();
        ArrayList<Rectangle> queryResults = tree.search(query);
        Instant after = Instant.now();
        Duration total = Duration.between(before,after);
        stats.updateDuration(total);
        stats.updateIOs(tree.getTotalSearchIOs());
        tree.resetTotalSearchIOs();
        return queryResults;
    }

    public static void main(String[] args) throws IOException, InterruptedException {
        int diskBlockSize = Integer.parseInt(args[0]);
        int seedR = 0;
        int seedQ = 0;
        if (args.length == 3) {
            seedR = Integer.parseInt(args[1]);
            seedQ = Integer.parseInt(args[2]);
        }
        FileObjectIO<Rectangle> rectIO = new FileObjectIO<>(new Rectangle(0,0,0,0));
        int rectangleByteSize = rectIO.calculateObjectSize();
        int rectanglesPerBlock = diskBlockSize % rectangleByteSize == 0 ?
                diskBlockSize / rectangleByteSize :
                diskBlockSize / rectangleByteSize - (diskBlockSize % rectangleByteSize);
        FileObjectIO<RTreeNode> nodeIO = new FileObjectIO<>(new RTreeNode(new Rectangle(0,0,0,0),0,0));
        int nodeByteSize = nodeIO.calculateObjectSize();
        int nodesPerBlock = diskBlockSize / nodeByteSize;
        PrintWriter output = new PrintWriter("output.csv");
        for (int k = 10; k < 26; k++) {
            output.println("n = 2^" + k + "; Tiempo NearestX ; IOs NearestX ; Tiempo Hilbert; IOs Hilbert; Tiempo STR; IOS STR;");
            int numberOfRectangles = 1 << k;
            RTreeAlgorithm[] algorithms = new RTreeAlgorithm[3];
            algorithms[0] = new NearestXAlgorithm(numberOfRectangles,nodesPerBlock);
            algorithms[1] = new HilbertAlgorithm(numberOfRectangles,nodesPerBlock);
            algorithms[2] = new SortTileRecursiveAlgorithm(numberOfRectangles,nodesPerBlock);
            RectangleGenerator.generate("rects.bin",true,numberOfRectangles,seedR,false);
            RectangleGenerator.generate("queries.bin",true,100,seedQ,true);
            Rectangle[] queries = new Rectangle[100];
            try (FileInputStream queryFile = new FileInputStream("queries.bin");
                 ObjectInputStream queryStream = new ObjectInputStream(queryFile)) {
                 queries = (Rectangle[]) queryStream.readObject();
            } catch (ClassNotFoundException e) {
                throw new RuntimeException(e);
            }
            StatsContainer[] finalStats = new StatsContainer[3];
            String[] queriesAsString = new String[100];
            for (int i = 0; i < 3; i++) {
                finalStats[i] = new StatsContainer();
                RTree tree = new RTree(nodesPerBlock,algorithms[i],numberOfRectangles,rectanglesPerBlock,nodesPerBlock);

                tree.buildTreeFromFile("rects.bin");
                for (int j = 0; j < 100; j++) {
                    PerformanceStats queryStats = new PerformanceStats();
                    Rectangle query = queries[j];
                    if (queriesAsString[j] != null) {
                        queriesAsString[j] = "Rect(" + query.getX1() + ", " + query.getY1() + ", " + query.getX2() + ", " + query.getY2() + ")";
                    }
                    ArrayList<Rectangle> results = executeQuery(tree,query,queryStats);
                    finalStats[i].addPerformanceStats(queryStats);
                    System.out.print("Query: ");
                    System.out.println(query);
                    for (Rectangle r : results) {
                        System.out.println(r);
                    }
                    if (results.size() == 0) {
                        System.out.println("No results");
                    }
                }
                tree.cleanup();
            }
            String[][] rows = new String[3][100];
            for (int i = 0; i < 3; i++) {
                rows[i] = finalStats[i].generateRows();
            }
            for (int i = 0; i < 100; i++) {
                queriesAsString[i]+= rows[0][i];
                queriesAsString[i]+= rows[1][i];
                queriesAsString[i]+= rows[2][i] + "\n";
                output.println(queriesAsString[i]);
            }
            Duration[] totalDurations = new Duration[3];
            int[] totalIOs = new int[3];
            for (int i = 0; i < 3; i++) {
                PerformanceStats[] allStats = finalStats[i].getStats();
                for (PerformanceStats stat : allStats) {
                    totalDurations[i] = totalDurations[i].plus(stat.getDuration());
                    totalIOs[i] += stat.getIos();
                }
                output.print("; " + totalDurations[i] + "; " + totalIOs[i]);
            }
            output.println("\n");
        }
        output.close();
    }
}