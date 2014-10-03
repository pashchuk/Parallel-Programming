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

        #endregion

        #region Vector manipulation fuctions

        #endregion
    }
}
