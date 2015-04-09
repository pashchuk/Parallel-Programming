using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace lab3
{
	public class Matrix<T> where T : struct 
	{
		#region Fields

		private T[,] _matrix;

		#endregion

		#region Properties

		public T this[int row, int column]
		{
			get { return _matrix[row, column]; }
			set { _matrix[row, column] = value; }
		}
		public int Rows { get { return _matrix.GetLength(0); } }
		public int Columns { get { return _matrix.GetLength(1); } }
		#endregion

		#region Costructors

		public Matrix(int rows, int columns)
		{
			_matrix = new T[rows, columns];
			for (int i = 0; i < rows; i++)
				for (int j = 0; j < columns; j++)
					_matrix[i, j] = default(T);
		}
		public Matrix(int size) : this(size, size) { }

		#endregion

		#region private Methods



		#endregion

		#region public Methods

		public void Print(TextWriter writer)
		{
			for (int i = 0; i < Rows; i++)
			{
				for (int j = 0; j < Columns; j++)
					writer.Write(string.Format("{0:0.000}  ", _matrix[i, j]));
				writer.WriteLine();
			}
		}
		#endregion
	}
}