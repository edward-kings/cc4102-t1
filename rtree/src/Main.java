import algorithms.HilbertAlgorithm;
import algorithms.NearestXAlgorithm;
import algorithms.RTreeAlgorithm;
import algorithms.SortTileRecursiveAlgorithm;
import file.generation.RectangleGenerator;
import file.utils.PerformanceStats;
import file.utils.StatsContainer;
import structs.RTreeNode;
import structs.Rectangle;

import java.io.*;
import java.util.ArrayList;

public class Main {
    /**
     * Elimina los archivos generados por la ejecución del programa anteriormente
     */
    public static void preCleanup() {
        File file = new File("rects.bin");
        file.delete();
        file = new File("queries.bin");
        file.delete();
        file = new File("sortedRectsHilbert.bin");
        file.delete();
        file = new File("sortedRectsNearestX.bin");
        file.delete();
        file = new File("sortedRectsSTR.bin");
        file.delete();
        file = new File("NearestXRTree.bin");
        file.delete();
        file = new File("HilbertRTree.bin");
        file.delete();
        file = new File("STRRTree.bin");
        file.delete();
        file = new File("output.csv");
        file.delete();
        for (int i = 10; i < 26; i++) {
            file = new File("results" + i + "NearestX.txt");
            file.close();
            file = new File("results" + i + "Hilbert.txt");
            file.close();
            file = new File("results" + i + "SortTileRecursive.txt");
            file.close();
        }
    }
    /**
     * Ejecuta una consulta sobre un RTree
     * @param tree RTree sobre el cual se ejecuta la consulta
     * @param query Rectángulo de consulta
     * @param stats Estadísticas de la consulta
     * @return Lista de rectángulos que intersectan con el rectángulo de consulta
     * @throws Exception Si ocurre un error al leer el archivo
     */
    public static ArrayList<Rectangle> executeQuery(RTree tree, Rectangle query, PerformanceStats stats) throws Exception {
        boolean isWindows = System.getProperty("os.name").toLowerCase().startsWith("windows");
        if (!isWindows) {
            String[] command = new String[2];
            command[0] = "/bin/sh";
            command[1] = "echo 3 > /proc/sys/vm/drop_caches";
            Process process = Runtime.getRuntime().exec(command);
            process.waitFor();
        }
        long before = System.nanoTime();
        ArrayList<Rectangle> queryResults = tree.search(query);
        long after = System.nanoTime();
        long total = after - before;
        stats.updateDuration(total);
        stats.updateIOs(tree.getTotalIOs());
        tree.resetTotalReads();
        return queryResults;
    }

    public static void main(String[] args) throws Exception {
        preCleanup();
        int diskBlockSize = args.length != 0 ? Integer.parseInt(args[0]) : 4096;
        int seedR = 0;
        int seedQ = 0;
        if (args.length == 3) {
            seedR = Integer.parseInt(args[1]);
            seedQ = Integer.parseInt(args[2]);
        }

        int nodeByteSize = RTreeNode.SIZE;
        int nodesPerBlock = diskBlockSize % nodeByteSize == 0 ?
                diskBlockSize / nodeByteSize :
                diskBlockSize / nodeByteSize - (diskBlockSize % nodeByteSize);
        // Archivo con las estadísticas de las consultas
        PrintWriter output = new PrintWriter("output.csv");
        for (int k = 10; k < 26; k++) {
            output.println("n = 2^" + k + "; Tiempo NearestX ; IOs NearestX ; Tiempo Hilbert; IOs Hilbert; Tiempo STR; IOS STR;");
            int numberOfRectangles = 1 << k;
            //System.out.println("====================================");
            //System.out.println("n = 2^"+ k + " = " + numberOfRectangles);
            //System.out.println("====================================");
            RTreeAlgorithm[] algorithms = new RTreeAlgorithm[3];
            algorithms[0] = new NearestXAlgorithm(numberOfRectangles,nodesPerBlock);
            algorithms[1] = new HilbertAlgorithm(numberOfRectangles,nodesPerBlock);
            algorithms[2] = new SortTileRecursiveAlgorithm(numberOfRectangles,nodesPerBlock);
            RectangleGenerator.generate("rects.bin",true,numberOfRectangles,seedR,false);
            RectangleGenerator.generate("queries.bin",true,100,seedQ,true);
            Rectangle[] queries = new Rectangle[100];
            try (FileInputStream queryFile = new FileInputStream("queries.bin");
                 DataInputStream queryStream = new DataInputStream(queryFile)) {
                 for (int i = 0; i < 100; i++) {
                        Rectangle query = Rectangle.deserialize(queryStream);
                        queries[i] = query;
                 }
            }
            StatsContainer[] finalStats = new StatsContainer[3];
            String[] queriesAsString = new String[100];
            for (int i = 0; i < 3; i++) {
                finalStats[i] = new StatsContainer();
                RTree tree = new RTree(algorithms[i]);
                PrintWriter resultsTxt = new PrintWriter("results" + k + algorithms[i].getAlgorithmName() +".txt");
                tree.buildTreeFromFile("rects.bin");
                for (int j = 0; j < 100; j++) {
                    PerformanceStats queryStats = new PerformanceStats();
                    Rectangle query = queries[j];
                    if (queriesAsString[j] == null) {
                        queriesAsString[j] = "Rect(" + query.x1() + ", " + query.y1() + ", " + query.x2() + ", " + query.y2() + ")";
                    }
                    ArrayList<Rectangle> results = executeQuery(tree,query,queryStats);
                    finalStats[i].addPerformanceStats(queryStats);
                    //System.out.print("Query: ");
                    //System.out.println(query);
                    resultsTxt.println("Query: " + query);
                    for (Rectangle r : results) {
                        //System.out.println(r);
                        resultsTxt.println(r);
                    }
                    if (results.size() == 0) {
                        //System.out.println("No results");
                        resultsTxt.println("No results");
                    }
                }
                resultsTxt.flush();
                resultsTxt.close();
                tree.cleanup();
            }
            String[][] rows = new String[3][100];
            for (int i = 0; i < 3; i++) {
                rows[i] = finalStats[i].generateRows();
            }
            for (int i = 0; i < 100; i++) {
                queriesAsString[i]+= rows[0][i];
                queriesAsString[i]+= rows[1][i];
                queriesAsString[i]+= rows[2][i];
                output.println(queriesAsString[i]);
            }
            long[] totalDurations = new long[3];
            int[] totalIOs = new int[3];
            for (int i = 0; i < 3; i++) {
                totalDurations[i] = 0;
                totalIOs[i] = 0;
                PerformanceStats[] allStats = finalStats[i].getStats();
                for (PerformanceStats stat : allStats) {
                    totalDurations[i] += stat.getDuration();
                    totalIOs[i] += stat.getIos();
                }
                output.print("; " + totalDurations[i] + "; " + totalIOs[i]);
            }
            output.println("\n");
            output.flush();
        }
        output.close();
    }
}