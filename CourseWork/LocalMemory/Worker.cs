using System;
using System.Threading;
using System.Threading.Tasks;

namespace LocalMemory
{
	public class Worker
	{
		public const int N = 1300;
		public const int P = 1;
		public const int H = N / P;

		private int _processCount;
		private int[,] MA, MB, MC, MK;
		private int[] E, T;
		private int alpha, ET_result;

		private ManualResetEvent _input1, _inputP, _ET_finish;
		private ManualResetEvent[] _vectorMultiply, _finishCalculations;
		private static object _lock = new object();
		public Worker(int procCount)
		{
			_processCount = P;
			MA = new int[N, N];
			MB = new int[N, N];
			MC = new int[N, N];
			MK = new int[N, N];
			E = new int[N];
			T = new int[N];
			alpha = 0;
			ET_result = 0;

			_input1 = new ManualResetEvent(false);
			_inputP = new ManualResetEvent(false);
			_ET_finish = new ManualResetEvent(false);
			_vectorMultiply = new ManualResetEvent[P];
			_finishCalculations = new ManualResetEvent[P];

			for (int i = 0; i < _vectorMultiply.Length; i++)
				_vectorMultiply[i] = new ManualResetEvent(false);
			for (int i = 0; i < _finishCalculations.Length; i++)
				_finishCalculations[i] = new ManualResetEvent(false);
			_vectorMultiply[0].Set();
			_finishCalculations[0].Set();
		}

		public Task Start()
		{
			return Task.Run(() =>
			{
				var threads = new Thread[_processCount];
				for (int i = 0; i < threads.Length; i++)
					threads[i] = new Thread((id) => DoWork((int) id));
				for (int i = 0; i < threads.Length; i++)
					threads[i].Start(i);
				foreach (var thread in threads)
					thread.Join();
			});
		}

		private void DoWork(int taskId)
		{
			if (_processCount <= 1) oneTask();
			else
			{
				if (taskId == 0) { firstTask(); return; }
				if (taskId == _processCount - 1) { lastTask(); return; }
				otherTasks(taskId);
			}
		}
		private void firstTask()
		{
			Console.WriteLine("Task {0,2:D} started!!!", 1);
			fill(MA);
			fill(MB);
			fill(T);
			_input1.Set();
			_inputP.WaitOne();

			int[,] MC_copy;
			int alpha_copy;
			lock (_lock)
			{
				MC_copy = copy(MC);
				alpha_copy = alpha;
			}
			int ET_sum = 0, ET_result_copy = 0;
			for (int i = getStart(1); i < getEnd(1); i++)
				ET_sum += E[i] * T[i];
			Interlocked.Add(ref ET_result, ET_sum);
			WaitHandle.WaitAll(_vectorMultiply);
			_ET_finish.Set();
			Interlocked.Exchange(ref ET_result_copy, ET_result);
			for (int i = getStart(1); i < getEnd(1); i++)
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MB[i, k] * MC_copy[k, j];
					sum *= alpha_copy;

					MA[i, j] = sum + MK[i, j] * ET_result_copy;
				}

			WaitHandle.WaitAll(_finishCalculations);
			Console.WriteLine("Task {0,2:D} finished!!!", 1);
			if(N<=10)
				for (int i = 0; i < MA.GetLength(0); i++)
				{
					for (int j = 0; j < MA.GetLength(1); j++)
						Console.Write("{0}  ", MA[i, j]);
					Console.WriteLine();
				}

		}
		private void lastTask()
		{
			Console.WriteLine("Task {0,2:D} started!!!", _processCount);
			fill(MC);
			fill(MK);
			fill(E);
			alpha = 1;
			_inputP.Set();
			_input1.WaitOne();
			calculation(_processCount - 1);
			Console.WriteLine("Task {0,2:D} finished!!!", _processCount);
		}
		private void otherTasks(int id)
		{
			Console.WriteLine("Task {0,2:D} started!!!", id + 1);
			_input1.WaitOne();
			_inputP.WaitOne();
			calculation(id);
			Console.WriteLine("Task {0,2:D} finished!!!", id + 1);
		}

		private void oneTask()
		{
			Console.WriteLine("Task {0,2:D} started!!!", 1);
			fill(MA);
			fill(MB);
			fill(T);
			fill(MC);
			fill(MK);
			fill(E);

			int ET_sum = 0;
			for (int i = 0; i < N; i++)
				ET_sum += E[i] * T[i];
			for (int i = 0; i < N; i++)
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MB[i, k] * MC[k, j];
					sum *= alpha;

					MA[i, j] = sum + MK[i, j] * ET_sum;
				}
			Console.WriteLine("Task {0,2:D} finished!!!", 1);
			if(N<=10)
				for (int i = 0; i < MA.GetLength(0); i++)
				{
					for (int j = 0; j < MA.GetLength(1); j++)
						Console.Write("{0}  ", MA[i, j]);
					Console.WriteLine();
				}

		}

		//MA = MB*MC*alpha + (E*T)*MK
		private void calculation(int id)
		{
			int[,] MC_copy;
			int alpha_copy;
			lock (_lock)
			{
				MC_copy = copy(MC);
				alpha_copy = alpha;
			}
			int ET_sum = 0, ET_result_copy = 0;
			for (int i = getStart(id + 1); i < getEnd(id + 1); i++)
				ET_sum += E[i] * T[i];
			Interlocked.Add(ref ET_result, ET_sum);
			_vectorMultiply[id].Set();
			_ET_finish.WaitOne();
			Interlocked.Exchange(ref ET_result_copy, ET_result);
			for (int i = getStart(id + 1); i < getEnd(id + 1); i++)
				for (int j = 0; j < N; j++)
				{
					int sum = 0;
					for (int k = 0; k < N; k++)
						sum += MB[i, k] * MC_copy[k, j];
					sum *= alpha_copy;

					MA[i, j] = sum + MK[i, j] * ET_result_copy;
				}
			_finishCalculations[id].Set();
		}

		private int[,] copy(int[,] matrix)
		{
			var result = new int[matrix.GetLength(0), matrix.GetLength(1)];
			for (int i = 0; i < matrix.GetLength(0); i++)
				for (int j = 0; j < matrix.GetLength(1); j++)
					result[i, j] = matrix[i, j];
			return result;
		}
		private void fill(int[,] matrix)
		{
			for (int i = 0; i < matrix.GetLength(0); i++)
				for (int j = 0; j < matrix.GetLength(1); j++)
					matrix[i, j] = 1;
		}
		private void fill(int[] vector)
		{
			for (int i = 0; i < vector.Length; i++)
				vector[i] = 1;
		}

		int getStart(int number)
		{
			int h = N / P;
			int h0 = N % P;
			h0 = number <= h0 ? number - 1 : h0;
			return (number - 1) * h + h0;
		}

		int getEnd(int number)
		{
			int h = N / P;
			int h0 = N % P;
			h0 = number <= h0 ? number : h0;
			return number * h + h0;
		}
	}
}
