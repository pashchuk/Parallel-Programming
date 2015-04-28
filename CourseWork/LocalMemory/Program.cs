/*
---------------------------------------------------------------------------------
--              Course Work. Part #2
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      16.03.2014
--      Task	  MA = MB*MC*alpha + (E*T)*MK
---------------------------------------------------------------------------------
*/

using System;
using System.Diagnostics;

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
