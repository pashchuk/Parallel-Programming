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
    public static final int size = 10, fillNumber = 1,
                            stackSize = 1000000;
    public static final boolean PrintMode = true;
    public static Object printSynchronization = new Object();
    public static void main(String[] args) throws InterruptedException {
        Thread t1 = new Thread(null, new F1(), "Task1", stackSize);
        Thread t2 = new Thread(null, new F2(), "Task2", stackSize);
        Thread t3 = new Thread(null, new F3(), "Task3", stackSize);
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
