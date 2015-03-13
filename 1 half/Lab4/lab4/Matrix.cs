using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4
{
    public class Matrix
    {
        private int[,] matrix;
        public int Size { get { return matrix.GetLength(0); } }

        public Matrix(int size)
        {
            matrix = new int[size,size];
        }

        public void Generate(int fillDigit)
        {
            for (int i = 0; i < Size; i++)
                for (int j = 0; j < Size; j++)
                    matrix[i, j] = fillDigit;
        }

        #region Operators
        public static Matrix operator *(Matrix m1, Matrix m2)
        {
            Matrix rezult = new Matrix(m1.Size);
            int tempValue = 0;
            for (int i = 0; i < rezult.Size; i++)
                for (int j = 0; j < rezult.Size; j++)
                {
                    tempValue = 0;
                    for (int k = 0; k < rezult.Size; k++)
                        tempValue += m1.GetElement(j, k) * m2.GetElement(k, j);
                    rezult.SetElement(i, j, tempValue);
                }
            return rezult;
        }

        public static Vector operator *(Matrix matrix, Vector vector)
        {
            Vector rezult = new Vector(vector.Size);
            int tempValue = 0;
            for (int i = 0; i < rezult.Size; i++)
            {
                tempValue = 0;
                for (int j = 0; j < rezult.Size; j++)
                    tempValue += vector.GetElement(i) * matrix.GetElement(j, i);
                rezult.SetElement(i, tempValue);
            }
            return rezult;
        }

        public static Matrix operator +(Matrix m1, Matrix m2)
        {
            Matrix rezult = new Matrix(m1.Size);
            for (int i = 0; i < m1.Size; i++)
                for (int j = 0; j < m1.Size; j++)
                    rezult.SetElement(i, j, m1.GetElement(i, j) + m2.GetElement(i, j));
            return rezult;
        }

        #endregion

        #region Matrix manipulation functions

        public int GetElement(int line, int column)
        {
            return this.matrix[line, column];
        }

        public void SetElement(int line, int column, int value)
        {
            this.matrix[line, column] = value;
        }

        public Matrix Transpose()
        {
            int[,] rezult = new int[Size,Size];
            for (int i = 0; i < Size; i++)
                for (int j = 0; j < Size; j++)
                    rezult[i, j] = matrix[j, i];
            matrix = rezult;
            return this;
        }

        public Matrix Sort()
        {
            int[] temparray = new int[Size];
            for (int i = 0; i < Size; i++)
            {
                for (int j = 0; j < Size; j++)
                    temparray[j] = matrix[i, j];
                temparray = temparray.OrderByDescending(c => c).ToArray();
                for (int j = 0; j < Size; j++)
                    matrix[i, j] = temparray[j];
            }
            return this;
        }

        public void Print()
        {
            for (int i = 0; i < Size; i++)
            {
                for (int j = 0; j < Size; j++)
                    Console.Write("{0,4:D}  ", matrix[i, j]);
                Console.WriteLine();
            }
        }

        #endregion
    }
}
