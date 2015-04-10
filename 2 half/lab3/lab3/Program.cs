#define PRINT

/*
----------------------------------------
-- Lab3                               --
-- Pashchuk Eduard Fedorovich         --
-- Group IP-22                        --
-- A = sort(B)*alpha + beta*C*(MO*MK) --
-- Data : 10.04.2015                  --
----------------------------------------
*/

using System;
using System.Threading;

namespace lab3
{

	internal class Program
	{
		private const int N = 12, P = 6, H = N/P;

		private Vector<int> A, B, C, Z;
		private Matrix<int> MO, MK;
		private volatile int alpha, beta;

		private static object _lock = new object();
		private Semaphore S1, S2;
		private Mutex M;
		private EventWaitHandle E7, E8, E9, E10, E11;
		private AutoResetEvent E1, E2, E3;
		private ManualResetEvent E4, E5, E6;

		public Program()
		{
			A = new Vector<int>(N);
			B = new Vector<int>(N);
			C = new Vector<int>(N);
			Z = new Vector<int>(N);

			MO = new Matrix<int>(N);
			MK = new Matrix<int>(N);

			S1 = new Semaphore(0, 5);
			S2 = new Semaphore(0, 5);
			M = new Mutex();

			E1 = new AutoResetEvent(false);
			E2 = new AutoResetEvent(false);
			E3 = new AutoResetEvent(false);

			E4 = new ManualResetEvent(false);
			E5 = new ManualResetEvent(false);
			E6 = new ManualResetEvent(false);

			E7 = new EventWaitHandle(false, EventResetMode.ManualReset);
			E8 = new EventWaitHandle(false, EventResetMode.ManualReset);
			E9 = new EventWaitHandle(false, EventResetMode.ManualReset);
			E10 = new EventWaitHandle(false, EventResetMode.ManualReset);
			E11 = new EventWaitHandle(false, EventResetMode.ManualReset);
		}

		public async void Start()
		{
			var t1 = new Thread(this.t1);
			var t2 = new Thread(this.t2);
			var t3 = new Thread(this.t3);
			var t4 = new Thread(this.t4);
			var t5 = new Thread(this.t5);
			var t6 = new Thread(this.t6);

			t1.Start();
			t2.Start();
			t3.Start();
			t4.Start();
			t5.Start();
			t6.Start();

			t1.Join();
		}

		public static void Merge(Vector<int> source,ref Vector<int> dest ,int firstIndex, int secondIndex, int firstLength, int secondLength)
		{
			var size = firstLength + secondLength;
			for (int i = firstIndex, j = secondIndex, k = firstIndex, n = 0; n < size; n++)
			{
				if (i == firstIndex + firstLength) { dest.CopyTo(source, j, k, size - n); break; }
				if (j == secondIndex + secondLength) { dest.CopyTo(source, i, k, size - n); break; }
				dest[k++] = source[i] < source[j] ? source[i++] : source[j++];
			}
		}

		public static void BubbleSort(Vector<int> vector, int startIndex, int size)
		{
			for(int i = startIndex, counter = 0; i < startIndex + size; i++, counter++)
				for(int j = startIndex; j < startIndex + size - counter - 1; j++)
					if (vector[j] > vector[j + 1])
					{
						var temp = vector[j];
						vector[j] = vector[j + 1];
						vector[j + 1] = temp;
					}
		}

		public static void Main(string[] args)
		{
			var program = new Program();
			program.Start();
		}

		void t1()
		{
			int processID = 0;
			Console.WriteLine("T{0} started!!!", processID + 1);
			//ввід B, MO
			B.Fill(1);
			MO.Fill(1);
			//Сигнал Т2,Т3,Т4,T5,T6 про завершення вводу.
			S1.Release(5);
			//Чекати на закінчення вводу в потоку Т4
			S2.WaitOne();
			//Копіювання аlpha, beta, МO
			int a1 = 0, b1 = 0;
			Interlocked.Exchange(ref a1, alpha);
			Interlocked.Exchange(ref b1, alpha);
			M.WaitOne();
			var MO1 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(1) = sort(Bh)
			BubbleSort(B, processID * H, H);

			//Очікування завершення сортування Zh(2) у потоці Т2
			E1.WaitOne();

			//Знаходження Z2h(1) = merge(Zh(1), Zh(2))
			Merge(B, ref Z, processID*H, processID*H + H, H, H);
			B.CopyTo(Z, processID * H, processID * H, 2*H);

			//Очікування завершення сортування Z4h(1) у потоці Т3
			E5.WaitOne();

			//Знаходження Z = merge(Z2h(1), Z4h(1))
			Merge(B, ref Z, processID*H, processID*H*2, 2*H, 4*H);
			B.CopyTo(Z, 0, 0, 6*H);

			//Сигнал про завершення сортування потокам Т2, Т3, Т4, T5, T6
			E6.Set();

			//Копіювання C1 = C
			Vector<int> C1;
			lock (_lock)
			{
				C1 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha1 + beta1 * C1 * (MO1 * MKh)
			for (int i = processID*H; i < processID*H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO1[j, k]*MK[k, i];
					currElement += C1[j]*sum;
				}
				A[i] = B[i]*a1 + b1*currElement;
			}
			//Очікування завершення обчислень у потоках T2, T3, T4, T5, T6
			WaitHandle.WaitAll(new WaitHandle[] {E7, E8, E9, E10, E11});
#if PRINT
			A.Print(Console.Out);
#endif
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
		void t2()
		{
			int processID = 1;
			Console.WriteLine("T{0} started!!!", processID + 1);

			//Чекати на закінчення вводу в потоках Т1, Т4
			S1.WaitOne();
			S2.WaitOne();

			//Копіювання аlpha, beta, МO
			int a2 = 0, b2 = 0;
			Interlocked.Exchange(ref a2, alpha);
			Interlocked.Exchange(ref b2, alpha);
			M.WaitOne();
			var MO2 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(2) = sort(Bh)
			BubbleSort(B, processID * H, H);

			//Сигнал про завершення сортування Zh(2) потоку Т1
			E1.Set();

			//Очікування завершення сортування у потоці Т1
			E6.WaitOne();

			//Копіювання C2 = C
			Vector<int> C2;
			lock (_lock)
			{
				C2 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha2 + beta2 * C2 * (MO2 * MKh)
			for (int i = processID * H; i < processID * H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO2[j, k] * MK[k, i];
					currElement += C2[j] * sum;
				}
				A[i] = B[i] * a2 + b2 * currElement;
			}

			//Сигнал про завершення обчислень потоку Т1
			E7.Set();
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
		void t3()
		{
			int processID = 2;
			Console.WriteLine("T{0} started!!!", processID + 1);

			//Чекати на закінчення вводу в потоках Т1, Т4
			S1.WaitOne();
			S2.WaitOne();

			//Копіювання аlpha, beta, МO
			int a3 = 0, b3 = 0;
			Interlocked.Exchange(ref a3, alpha);
			Interlocked.Exchange(ref b3, alpha);
			M.WaitOne();
			var MO3 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(3) = sort(Bh)
			BubbleSort(B, processID * H, H);

			//Очікування завершення сортування Zh(4) у потоці Т4
			E2.WaitOne();

			//Знаходження Z2h(2) = merge(Zh(3), Zh(4))
			Merge(B, ref Z, processID*H, processID*H + H, H, H);
			B.CopyTo(Z, processID*H, processID*H, 2*H);

			//Очікування завершення сортування Z2h(3) у потоці Т5
			E4.WaitOne();

			//Знаходження Z4h(1) = merge(Z2h(2), Z2h(3))
			Merge(B, ref Z, processID*H, processID*H*2, 2*H, 2*H);
			B.CopyTo(Z, processID*H, processID*H, 4*H);

			//Сигнал про завершення сортування Z4h(1) потоку Т1
			E5.Set();

			//Очікування завершення сортування у потоці Т1
			E6.WaitOne();

			//Копіювання C3 = C
			Vector<int> C3;
			lock (_lock)
			{
				C3 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha3 + beta3 * C3 * (MO3 * MKh)
			for (int i = processID * H; i < processID * H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO3[j, k] * MK[k, i];
					currElement += C3[j] * sum;
				}
				A[i] = B[i] * a3 + b3 * currElement;
			}

			//Сигнал про завершення обчислень потоку Т1
			E8.Set();
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
		void t4()
		{
			int processID = 3;
			Console.WriteLine("T{0} started!!!", processID + 1);
			//ввід C, alpha, beta, MK
			C.Fill(1);
			alpha = 1;
			beta = 1;
			MK.Fill(1);
			//Сигнал Т1,Т2,Т3,T5,T6 про завершення вводу.
			S2.Release(5);
			//Чекати на закінчення вводу в потоку Т1
			S1.WaitOne();
			//Копіювання аlpha, beta, МO
			int a4 = 0, b4 = 0;
			Interlocked.Exchange(ref a4, alpha);
			Interlocked.Exchange(ref b4, alpha);
			M.WaitOne();
			var MO4 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(4) = sort(Bh)
			BubbleSort(B, processID*H, H);

			//Сигнал про завершення сортування Zh(4) потоку Т3
			E2.Set();

			//Очікування завершення сортування у потоці Т1
			E6.WaitOne();

			//Копіювання C4 = C
			Vector<int> C4;
			lock (_lock)
			{
				C4 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha4 + beta4 * C4 * (MO4 * MKh)
			for (int i = processID * H; i < processID * H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO4[j, k] * MK[k, i];
					currElement += C4[j] * sum;
				}
				A[i] = B[i] * a4 + b4 * currElement;
			}

			//Сигнал про завершення обчислень потоку Т1
			E9.Set();
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
		void t5()
		{
			int processID = 4;
			Console.WriteLine("T{0} started!!!", processID + 1);

			//Чекати на закінчення вводу в потоках Т1, Т4
			S1.WaitOne();
			S2.WaitOne();

			//Копіювання аlpha, beta, МO
			int a5 = 0, b5 = 0;
			Interlocked.Exchange(ref a5, alpha);
			Interlocked.Exchange(ref b5, alpha);
			M.WaitOne();
			var MO5 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(5) = sort(Bh)
			BubbleSort(B, processID * H, H);

			//Очікування завершення сортування Zh(6) у потоці Т6
			E3.WaitOne();

			//Знаходження Z2h(3) = merge(Zh(5), Zh(6))
			Merge(B, ref Z, processID*H, processID*H + H, H, H);
			B.CopyTo(Z, processID*H, processID*H, 2*H);

			//Сигнал про завершення сортування Z2h(3) потоку Т3
			E4.Set();

			//Очікування завершення сортування у потоці Т1
			E6.WaitOne();

			//Копіювання C5 = C
			Vector<int> C5;
			lock (_lock)
			{
				C5 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha5 + beta5 * C5 * (MO5 * MKh)
			for (int i = processID * H; i < processID * H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO5[j, k] * MK[k, i];
					currElement += C5[j] * sum;
				}
				A[i] = B[i] * a5 + b5 * currElement;
			}

			//Сигнал про завершення обчислень потоку Т1
			E10.Set();
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
		void t6()
		{
			int processID = 5;
			Console.WriteLine("T{0} started!!!", processID + 1);

			//Чекати на закінчення вводу в потоках Т1, Т4
			S1.WaitOne();
			S2.WaitOne();

			//Копіювання аlpha, beta, МO
			int a6 = 0, b6 = 0;
			Interlocked.Exchange(ref a6, alpha);
			Interlocked.Exchange(ref b6, alpha);
			M.WaitOne();
			var MO6 = MO.Copy();
			M.ReleaseMutex();

			//Знаходження Zh(6) = sort(Bh)
			BubbleSort(B, processID * H, H);

			//Сигнал про завершення сортування Zh(6) потоку Т5
			E3.Set();

			//Очікування завершення сортування у потоці Т1
			E6.WaitOne();

			//Копіювання C6 = C
			Vector<int> C6;
			lock (_lock)
			{
				C6 = C.Copy();
			}

			//Знаходження Ah = Zh * alpha6 + beta6 * C6 * (MO6 * MKh)
			for (int i = processID * H; i < processID * H + H; i++)
			{
				int currElement = 0;
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MO6[j, k] * MK[k, i];
					currElement += C6[j] * sum;
				}
				A[i] = B[i] * a6 + b6 * currElement;
			}

			//Сигнал про завершення обчислень потоку Т1
			E11.Set();
			Console.WriteLine("T{0} finished!!!", processID + 1);
		}
	}
}
