using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab3
{
	public class Vector<T> where T : struct
	{
		#region Fields

		private T[] _vector;

		#endregion

		#region Properties

		public T this[int index]
		{
			get { return _vector[index]; }
			set { _vector[index] = value; }
		}
		public int Length { get { return _vector.Length; } }
		#endregion

		#region Costructors

		public Vector(int size)
		{
			_vector = new T[size];
			for (int i = 0; i < size; i++)
				_vector[i] = default(T);
		}

		#endregion

		#region private Methods



		#endregion

		#region public Methods

		public void Print(TextWriter writer)
		{
			for (int i = 0; i < Length; i++)
				writer.Write(string.Format("{0}  ", _vector[i]));
			writer.WriteLine();
		}

		public void Fill(T fillElement)
		{
			for (int i = 0; i < Length; i++)
				_vector[i] = fillElement;
		}

		public void CopyTo(Vector<T> source, int sourceIndex, int destinationIndex, int length)
		{
			for (var i = destinationIndex; i < destinationIndex + length; i++)
				_vector[i] = source[sourceIndex++];
		}

		public Vector<T> Copy()
		{
			var result = new Vector<T>(Length);
			for (int i = 0; i < Length; i++)
				result[i] = _vector[i];
			return result;
		}

		#endregion
	}
}
