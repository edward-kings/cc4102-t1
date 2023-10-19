package file.utils;


/**
 * Estadísticas de rendimiento, especificamente tiempo y cantidad de I/Os
 */
public class PerformanceStats {

    private long algorithmTime;
    private long algorithmIOs;

    public void updateDuration(long newDuration) {
        algorithmTime = newDuration;
    }

    public long getDuration() {
        return algorithmTime;
    }

    public void updateIOs(long newIos) {
        algorithmIOs = newIos;
    }

    public long getIos() {
        return algorithmIOs;
    }
}
