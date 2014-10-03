using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace lab4
{
    public class Vector
    {
        private int[] vector;

        public int Size { get { return vector.Length; } }

        public Vector(int size)
        {
            vector = new int[size];
        }

        public void Generate(int fillDigit)
        {
            for (int i = 0; i < Size; i++)
                vector[i] = fillDigit;
        }

        #region Operators
        public static Vector operator *(Matrix matrix, Vector vector)
        {
            var rezult = new Vector(vector.Size);
            var tempValue = 0;
            for (int i = 0; i < rezult.Size; i++)
            {
                tempValue = 0;
                for (int j = 0; j < rezult.Size; j++)
                    tempValue += vector.GetElement(i) * matrix.GetElement(j, i);
                rezult.SetElement(i, tempValue);
            }
            return rezult;
        }

        public static int operator *(Vector v1, Vector v2)
        {
            int rezult = 0;
            for (int i = 0; i < v1.Size; i++)
                rezult += v1.GetElement(i) * v2.GetElement(i);
            return rezult;
        }

        public static Vector operator +(Vector v1, Vector v2)
        {
            Vector rezult = new Vector(v1.Size);
            for (int i = 0; i < v1.Size; i++)
                rezult.SetElement(i, v1.GetElement(i) + v2.GetElement(i));
            return rezult;
        }

        #endregion

        #region Vector manipulation fuctions

        public int GetElement(int index)
        {
            return this.vector[index];
        }

        public void SetElement(int index, int value)
        {
            this.vector[index] = value;
        }

        public void Print()
        {
            for (int i = 0; i < vector.Length; i++)
                Console.Write("{0,4:D}  ", vector[i]);
            Console.WriteLine();
        }

        public void Sort()
        {
            Array.Sort(vector);
        }
        #endregion
    }
}
