package file.utils;

/**
 * Contenedor de estadísticas de rendimiento
 */
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
    /**
     * Genera un arreglo de strings con los datos de las estadísticas para un archivo CSV
     * @return Arreglo de strings con los datos de las estadísticas
     */
    public String[] generateRows() {
        String[] result = new String[100];
        for (int i = 0; i < 100; i++) {
            PerformanceStats stat = stats[i];
            result[i] = "; " + stat.getDuration() + "; " + stat.getIos();
        }
        return result;
    }
}
