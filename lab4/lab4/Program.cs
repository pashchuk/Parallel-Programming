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
using System.Threading.Tasks;

namespace lab4
{
    class Program
    {
	    private static Semaphore pool = new Semaphore(1, 1);
        private const int Size = 10;
        private const int Stack_Size = 1000000;
        private const int Fill_Digit = 1;
        static void Main(string[] args)
        {
	        HelpTask1();
	        Task.Factory.StartNew(Task2);
            Thread t3 = new Thread(Task3, Stack_Size) { Name = "T3", Priority = ThreadPriority.Normal };
            t3.Start();
            t3.Join();
            Console.ReadLine();
        }
        
        private static Task<int> Task1()
        {
			return Task.Run(() =>
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
				Console.WriteLine("Task1 finished");
				return d;
			});
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
	        pool.WaitOne();
                Console.WriteLine("F2 = ");
                MC.Print();
	        pool.Release();
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
	        pool.WaitOne();
                Console.Write("F3 = ");
                D.Print();
	        pool.Release();
#endif
            Console.WriteLine("Task3 finished");
        }

	    private static async void HelpTask1()
	    {
		    int result = await Task1();
		    Console.WriteLine("d = {0}", result);
	    }
    }
}
