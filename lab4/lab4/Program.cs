/*
---------------------------------------------------------------------------------
--              Laboratory Work #4
--        Lab3.Threads in C# programming language
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      03.10.2014
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ)))
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM)
---------------------------------------------------------------------------------
 */
#define Print

using System;
using System.Threading;

namespace lab4
{
    class Program
    {
        private const int Size = 10;
        private const int Stack_Size = 1000000;
        private const int Fill_Digit = 1;
        private static readonly object syncronized_object = new Object();
        static void Main(string[] args)
        {
            Thread t1 = new Thread(Task1, Stack_Size) { Name = "T1", Priority = ThreadPriority.Normal };
            Thread t2 = new Thread(Task2, Stack_Size) { Name = "T2", Priority = ThreadPriority.Normal };
            Thread t3 = new Thread(Task3, Stack_Size) { Name = "T3", Priority = ThreadPriority.Normal };
            t1.Start();
            t2.Start();
            t3.Start();
            t1.Join();
            t2.Join();
            t3.Join();
            Console.ReadLine();
        }
        
        private static void Task1()
        {
            Console.WriteLine("Task1 started");
            Vector A = new Vector(Size),
                B = new Vector(Size),
                C = new Vector(Size);
            Matrix MA = new Matrix(Size),
                MZ = new Matrix(Size);
            Console.WriteLine("Generating vector A ...");
            A.Generate(Fill_Digit);
            Console.WriteLine("Generating vector B ...");
            B.Generate(Fill_Digit);
            Console.WriteLine("Generating vector C ...");
            C.Generate(Fill_Digit);
            Console.WriteLine("Generating matrix MA ...");
            MA.Generate(Fill_Digit);
            Console.WriteLine("Generating matrix MZ ...");
            MZ.Generate(Fill_Digit);
            Console.WriteLine("----------Calculating F1----------");
            int d = (B*C) + (A*B) + (C*(B*(MA*MZ)));
#if Print
            lock (syncronized_object)
            {
                Console.WriteLine("F1 = {0}",d);   
            }
#endif

            Console.WriteLine("Task1 finished");
        }

        private static void Task2()
        {
            Console.WriteLine("Task2 started");
            Matrix MA = new Matrix(Size),
                MB = new Matrix(Size), MC;
            Console.WriteLine("Generating matrix MA ...");
            MA.Generate(Fill_Digit);
            Console.WriteLine("Generating matrix MB ...");
            MB.Generate(Fill_Digit);
            Console.WriteLine("----------Calculating F2----------");
            MC = (MA.Transpose()*MB).Sort();
#if Print
            lock (syncronized_object)
            {
                Console.WriteLine("F2 = ");
                MC.Print();   
            }
#endif
            Console.WriteLine("Task2 finished");
        }

        private static void Task3()
        {
            Console.WriteLine("Task3 started");
            Vector A = new Vector(Size),
                M = new Vector(Size), D;
            Matrix MC = new Matrix(Size),
                MM = new Matrix(Size);
            Console.WriteLine("Generating vector A ...");
            A.Generate(Fill_Digit);
            Console.WriteLine("Generating vector M ...");
            M.Generate(Fill_Digit);
            Console.WriteLine("Generating matrix MC ...");
            MC.Generate(Fill_Digit);
            Console.WriteLine("Generating matrix MM ...");
            MM.Generate(Fill_Digit);
            Console.WriteLine("----------Calculating F3----------");
            D = (A + M).Sort()*(MC*MM).Transpose();
#if Print
            lock (syncronized_object)
            {
                Console.Write("F3 = ");
                D.Print();   
            }
#endif
            Console.WriteLine("Task3 finished");
        }
    }
}
