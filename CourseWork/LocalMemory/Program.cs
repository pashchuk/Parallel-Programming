using System;
using System.Diagnostics;

//MA = MB*MC*alpha + (E*T)*MK

namespace LocalMemory
{
	class Program
	{
		static void Main(string[] args)
		{
			var watch = new Stopwatch();
			var worker = new Worker(10);
			watch.Start();
			worker.Start().Wait();
			watch.Stop();
			Console.WriteLine(watch.ElapsedMilliseconds);
		}
	}

	
}
