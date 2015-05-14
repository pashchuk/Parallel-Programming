/*
 -------------------------------------
 -- Lab6                            --
 -- Pashchuk Eduard Fedorovich      --
 -- Group IP-22                     --
 -- A =(B*C)*R + a*E*(MT*MK)        --
 -- MA = MB*(MC*MK)+(B*R)*MU        --
 -- Data : 14.05.2015               --
 -------------------------------------
*/

public class Lab6 {

    private static final int N = 10;
    private static final int P = 8;
    private static final int H = N / P;
    private static final Monitor MONITOR = new Monitor(P);
    private static final Thread[] THREADS = new Thread[P];


    private static int[][] MA = inputMatrix();
    private static int[][] MB;
    private static int[][] MK;
    private static int[][] MU;
    private static int[] B;
    private static int[] R;

    public static void calculate(int number) {
        int start = (number - 1) * H;
        int end = number * H;

        MONITOR.waitInput();

        int di = 0;
        for (int i = start; i < end; i++) {
            di += B[i] * R[i];
        }
        MONITOR.writed(di);
        MONITOR.signalCount1();
        MONITOR.waitCount1();

        int ai = MONITOR.copya();
        int dl = MONITOR.copyd();
        int[] Ei = MONITOR.copyE();
        int[][] MTi = MONITOR.copyMT();

        int[][] MM = new int[N][N];
        for (int i = 0; i < N; i++) {
            for (int j = start; j < end; j++) {
                MM[i][j] = 0;
                for (int k = 0; k < N; k++) {
                    MM[i][j] += MTi[i][k] * MK[k][j];
                }
            }
        }
        for (int i = start; i < end; i++) {
            A[i] = dl * R[i];
            for (int j = 0; j < N; j++) {
                A[i] += + ai * Ei[i] * MM[j][i];
            }
        }
        MONITOR.signalCount2();
    }

    public static int[] inputVector() {
        int[] A = new int[N];
        for (int i = 0; i < A.length; i++) {
            A[i] = 1;
        }
        return A;
    }

    public static int[][] inputMatrix() {
        int[][] MA = new int[N][N];
        for (int i = 0; i < MA.length; i++) {
            MA[i] = inputVector();
        }
        return MA;
    }

    public static void outputVector(int[] A) {
        System.out.println("A:");
        for (int a : A) {
            System.out.print(String.format("%6s", a));
        }
    }

    public static void input(int number) {
        if (number == 1) {
            B = inputVector();
            MK = inputMatrix();
            MONITOR.signalInput();
        } else if (number == 3) {
            C = inputVector();
            MONITOR.writeMT(inputMatrix());
            MONITOR.signalInput();
        } else if (number == 6) {
            R = inputVector();
            MONITOR.writea(1);
            MONITOR.writeE(inputVector());
            MONITOR.signalInput();
        }
    }

    public static void output(int number) {
        if (number == 1) {
            MONITOR.waitCount2();
            outputVector(A);
        }
    }

    public static void perform(int number) {
        System.out.println(String.format("Thread%s started!!!", number));
        input(number);
        calculate(number);
        output(number);
        System.out.println(String.format("Thread%s finished!!!", number));
    }

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < P; i++) {
            final int finalI = i;
            THREADS[i] = new Thread() {
                @Override
                public void run() {
                    perform(finalI + 1);
                }
            };
        }
        for (int i = 0; i < P; i++) {
            THREADS[i].start();
        }
        THREADS[0].join();
    }
}