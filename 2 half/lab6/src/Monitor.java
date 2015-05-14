import java.util.Arrays;

public class Monitor {
    private int P;
	private int a;
	private int d = 0;
	private int[] E;
	private int[][] MC;
    private boolean flagInput;
    private int flagCount1;
    private int flagCount2;

    public Monitor(int threadCount){
        this.P = threadCount;
    }

    public synchronized void writed(int di) {
        this.d += di;
    }

    public synchronized int copyd() {
        return d;
    }

    public synchronized void writeMC(int[][] MC) {
        this.MC = MC;
    }

    public synchronized int[][] copyMC() {
        return Arrays.copyOf(MC, MC.length);
    }

    public synchronized void signalInput() {
        flagInput = true;
        notifyAll();
    }

    public synchronized void signalCount1() {
        if(++flagCount1 == P)
            notifyAll();
    }

    public synchronized void signalCount2() {
        if(++flagCount2 == P)
            notifyAll();
    }

    public synchronized void waitInput() {
        while (!flagInput) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }

    public synchronized void waitCount1() {
        while (flagCount1 != P) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }

    public synchronized void waitCount2() {
        while (flagCount2 != P) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }
}