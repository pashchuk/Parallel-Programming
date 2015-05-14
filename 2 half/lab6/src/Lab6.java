/*
 -------------------------------------
 -- Lab6                            --
 -- Pashchuk Eduard Fedorovich      --
 -- Group IP-22                     --
 -- MA = MB*(MC*MK)+(B*R)*MU        --
 -- Data : 14.05.2015               --
 -------------------------------------
*/

public class Lab6 {

    private static final int N = 6;
    private static final int P = 6;
    private static final int H = N / P;
    private static final Monitor MONITOR = new Monitor(P);
    private static final Thread[] THREADS = new Thread[P];


    private static int[][] MA = new int[N][];
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

        int dl = MONITOR.copyd();
        int[][] MCi = MONITOR.copyMC();
        int[][] MB = MONITOR.copyMB();
        int[][] temp = new int[N][];
        for(int i = 0; i < N; i++)
            temp[i] = new int[N];

        for(int i = start; i < end; i++)
            for(int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                    temp[j][i] += MCi[j][k] * MK[k][i];

        for(int i = start; i < end; i++)
            for(int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++)
                    MA[j][i] += MB[j][k] * temp[k][i];
                MA[j][i] += dl * MU[j][i];
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

    public static void outputMatrix(int[][] MA) {
        System.out.println("MA:");
        for (int[] line : MA) {
            for(int x : line) {
                System.out.print(String.format("%6s", x));
            }
            System.out.println();
        }
    }

    public static void input() {
        for(int i = 0; i < N; i++)
            MA[i] = new int[N];
        MONITOR.writeMC(inputMatrix());
        MONITOR.writeMB(inputMatrix());
        MK = inputMatrix();
        MU = inputMatrix();
        B = inputVector();
        R = inputVector();
        MONITOR.signalInput();
    }

    public static void output() {
        MONITOR.waitCount2();
        outputMatrix(MA);
    }

    public static void perform(int number) {
        System.out.println(String.format("Thread%s started!!!", number));
        if(number==6)
            input();
        calculate(number);
        if(number==6)
            output();
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