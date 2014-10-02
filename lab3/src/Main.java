/*
---------------------------------------------------------------------------------
--              Laboratory Work #3
--        Lab3.Threads in Java programming language
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      01.10.2014
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ)))
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM)
---------------------------------------------------------------------------------
 */

public class Main {
    public static final int size = 2000;
    public static final int fillNumber = 1;
    public static final boolean PrintMode = true;
    public static Object printSynchronization = new Object();
    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(new F1());
        Thread t2 = new Thread(new F2());
        Thread t3 = new F3();
        t1.setPriority(3);
        t2.setPriority(3);
        t3.setPriority(3);
        t1.start();
        t2.start();
        t3.start();
        t1.join();
        t2.join();
        t3.join();
    }
}
