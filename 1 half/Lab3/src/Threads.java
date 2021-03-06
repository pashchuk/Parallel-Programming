/**
 * Created by mamaxropela on 02.10.14.
 */
class F1 implements Runnable {
    @Override
    public void run() {
        System.out.println("Task1 started");
        Vector A = new Vector(Main.size),
                B = new Vector(Main.size),
                C = new Vector(Main.size);
        Matrix MA = new Matrix(Main.size),
                MZ = new Matrix(Main.size);
        System.out.println("generating vector A ...");
        A.Generate(Main.fillNumber);
        System.out.println("generating vector B ...");
        B.Generate(Main.fillNumber);
        System.out.println("generating vector C ...");
        C.Generate(Main.fillNumber);
        System.out.println("generating matrix MA ...");
        MA.Generate(Main.fillNumber);
        System.out.println("generating matrix MZ ...");
        MZ.Generate(Main.fillNumber);
        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Calculating F1 ...");
        int d = Vector.Multiply(B,C) + Vector.Multiply(A,B) +
                Vector.Multiply(C,Vector.Multiply(Matrix.Multiply(MA,MZ),B));
        if(Main.PrintMode)
            synchronized (Main.printSynchronization) {
                System.out.printf("d = %d", d);
                System.out.println();
                System.out.println();
            }
        System.out.println("Task1 finished");
    }
}

class F2 implements Runnable {
    @Override
    public void run() {
        System.out.println("Task2 started");
        Matrix MA = new Matrix(Main.size),
                MB = new Matrix(Main.size);
        System.out.println("generating matrix MA ...");
        MA.Generate(Main.fillNumber);
        System.out.println("generating matrix MB ...");
        MB.Generate(Main.fillNumber);
        try {
            Thread.sleep(3000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println("Calculating F2 ...");
        Matrix MC = Matrix.Multiply(MA.Transpose(),MB).Sort();
        if(Main.PrintMode)
            synchronized (Main.printSynchronization) {
                System.out.printf("MC = ");
                System.out.println();
                MC.Print();
            }
        System.out.println("Task2 finished");
    }
}
 class F3 extends Thread {
     @Override
     public void run() {
         super.run();
         System.out.println("Task3 started");
         Vector A = new Vector(Main.size),
                 M = new Vector(Main.size);
         Matrix MC = new Matrix(Main.size),
                 MM = new Matrix(Main.size);
         System.out.println("generating vector A ...");
         A.Generate(Main.fillNumber);
         System.out.println("generating vector M ...");
         M.Generate(Main.fillNumber);
         System.out.println("generating matrix MC ...");
         MC.Generate(Main.fillNumber);
         System.out.println("generating matrix MM ...");
         MM.Generate(Main.fillNumber);
         System.out.println("Calculating F3 ...");
         Vector D = Vector.Multiply(Matrix.Multiply(MC, MM).Transpose(), Vector.Plus(A, M).Sort());
         if(Main.PrintMode)
             synchronized (Main.printSynchronization) {
                 System.out.printf("D = ");
                 System.out.println();
                 D.Print();
             }
         System.out.println("Task3 finished");
     }
 }
