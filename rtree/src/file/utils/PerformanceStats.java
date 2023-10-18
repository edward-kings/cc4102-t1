package file.utils;

import java.time.Duration;

public class PerformanceStats {

    private Duration algorithmTimes;
    private long algorithmIOs;

    public void updateDuration(Duration newDuration) {
        algorithmTimes = newDuration;
    }

    public Duration getDuration() {
        return algorithmTimes;
    }

    public void updateIOs(long newIos) {
        algorithmIOs = newIos;
    }

    public long getIos() {
        return algorithmIOs;
    }
}
