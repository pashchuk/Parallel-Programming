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
        System.out.println("Calculating F1 ...");
        int d = Vector.Multiply(B,C) + Vector.Multiply(A,B) +
                Vector.Multiply(C,Vector.Multiply(Matrix.Multiply(MA,MZ),B));
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
        System.out.println("Calculating F2 ...");
        Matrix MC = Matrix.Multiply(MA.Transpose(),MB).Sort();
        System.out.println("Task2 finished");
    }
}
 class F3 extends Thread {
     @Override
     public void run() {
         super.run();

     }
 }
