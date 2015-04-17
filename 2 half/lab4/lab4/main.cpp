/*
-------------------------------------
-- Lab4                            --
-- Pashchuk Eduard Fedorovich      --
-- Group IP-22
-- A = B*(MO*MK) + alpha*(C*MD)*MR --
-- Data : 16.04.2015               --
-------------------------------------
*/

#include <iostream>
#include "omp.h"
#include "data.h"

#define N 6
#define P 6
#define H N/P

using namespace std;

int alpha;
Vector A(N), B(N), C(N), X(N);
Matrix MO(N), MK(N), MD(N), MR(N);

int main(int argc, char* argv[])
{
	printf("Main thread: started\n");

	omp_set_num_threads(P);

#pragma omp parallel 
	{
		int num = omp_get_thread_num();

		printf("Thread %d: started\n", num + 1);

		switch (num) {
		case 0:
			B.generate();
			break;
		case 1:
			MO.generate();
			MR.generate();
			break;
		case 2:
			MK.generate();
			alpha = 1;
			break;
		case 3:
			C.generate();
			MD.generate();
			break;
		}

#pragma omp barrier

		Vector Ci(N);

#pragma omp critical(copy)
		{
			Ci.copy(C);
		}
		for (int i = num*H; i < num*H + H; i++)
		{
			X.vector[i] = 0;
			for (int j = 0; j < N; j++)
				X.vector[i] += Ci.vector[j] * MD.matrix[j][i];
		}

#pragma omp barrier

		Vector Xi(N), Bi(N);
		Matrix MOi(N);
		int ai;

#pragma omp critical(copy)
		{
			Xi.copy(X);
			Bi.copy(B);
			MOi.copy(MO);
			ai = alpha;
		}
		for (int i = num*H; i < num*H + H; i++)
		{
			A.vector[i] = 0;
			for (int j = 0; j < N; j++)
			{
				int sum = 0;
				for (int k = 0; k < N; k++)
					sum += MO.matrix[j][k] * MK.matrix[k][i];
				A.vector[i] += Bi.vector[j] * sum;
			}
			int sum = 0;
			for (int j = 0; j < N; j++)
				sum += Xi.vector[j] * MR.matrix[j][i];
			A.vector[i] += alpha * sum;
		}


#pragma omp barrier

		if (num == 0) {
			A.print();
		}
		printf("Thread %d: finished\n", num + 1);
	}

	printf("Main thread: finished\n");
	cin.get();
	return 0;
}
