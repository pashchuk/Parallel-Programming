import java.util.Arrays;

public class Monitor {
    private int P;
	private int a;
	private int d = 0;
	private int[] E;
	private int[][] MC;
    private int flagInput;
    private int flagCount1;
    private int flagCount2;

    public Monitor(int threadCount){
        this.P = threadCount;
    }

    public synchronized void writea(int a) {
        this.a = a;
    }

    public synchronized int copya() {
        return a;
    }

    public synchronized void writed(int di) {
        this.d += di;
    }

    public synchronized int copyd() {
        return d;
    }

    public synchronized void writeE(int[] E) {
        this.E = E;
    }

    public synchronized int[] copyE() {
        return Arrays.copyOf(E, E.length);
    }

    public synchronized void writeMT(int[][] MT) {
        this.MT = MT;
    }

    public synchronized int[][] copyMT() {
        return Arrays.copyOf(MT, MT.length);
    }

    public synchronized void signalInput() {
        if(++flagInput == 1)
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
        while (flagInput != 1) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }

    public synchronized void waitCount1() {
        while (flagCount1 != 8) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }

    public synchronized void waitCount2() {
        while (flagCount2 != 8) {
            try {
                wait();
            } catch (InterruptedException ignored) {
            }
        }
    }
}