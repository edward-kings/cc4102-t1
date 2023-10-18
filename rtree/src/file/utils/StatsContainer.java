package file.utils;

public class StatsContainer {
    private final PerformanceStats[] stats = new PerformanceStats[100];
    private int index = 0;

    public void addPerformanceStats(PerformanceStats stat) {
        stats[index] = stat;
        index++;
    }

    public PerformanceStats[] getStats() {
        return stats;
    }
    public String[] generateRows() {
        String[] result = new String[100];
        for (int i = 0; i < 100; i++) {
            PerformanceStats stat = stats[i];
            result[i] = "; " + stat.getDuration().toString() + "; " + stat.getIos();
        }
        return result;
    }
}
