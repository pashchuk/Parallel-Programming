/*
---------------------------------------------------------------------------------
--              Laboratory Work #7
--        Lab7.Threads in MPI library
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      19.10.2014
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ)))
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM)
---------------------------------------------------------------------------------
*/


#include <windows.h>
#include <iostream>
#include <mpi.h>
#include "Data.h"

#define SIZE 10
#define FILL_NUMBER 1

#define PrintTag 1
#define ResourceTag 2

void task1();
void task2();
void task3();

using namespace std;

int main(int argc, char** argv)
{
	int rank;
	Vector *A = new Vector(SIZE),
		*B = new Vector(SIZE),
		*C = new Vector(SIZE),
		*M = new Vector(SIZE);
	Matrix *MA = new Matrix(SIZE),
		*MB = new Matrix(SIZE),
		*MC = new Matrix(SIZE),
		*MM = new Matrix(SIZE),
		*MZ = new Matrix(SIZE);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		//Sending all data into generate process
		MPI_Send(A, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(B, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(C, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(M, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(MA, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(MB, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);	
		MPI_Send(MC, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(MM, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		MPI_Send(MZ, SIZE*SIZE + 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD);
		
	}
	if (rank == 1)
		task1();
	if (rank == 2)
		task2();
	if (rank == 3)
		task3();
	cout << "Main thread close" << endl;
	MPI_Finalize();
	cin.get();
	return 0;

}

void task1() {
	Vector *A, *B, *C, *M;
	Matrix *MA, *MB, *MC, *MM, *MZ;

	printf("generating vector A ...\n");
	A->generate(FILL_NUMBER);
	printf("generating vector B ...\n");
	B->generate(FILL_NUMBER);
	printf("generating vector C ...\n");
	C->generate(FILL_NUMBER);
	printf("generating vector M ...\n");
	M->generate(FILL_NUMBER);
	printf("generating matrix MA ...\n");
	MA->generate(FILL_NUMBER);
	printf("generating matrix MB ...\n");
	MB->generate(FILL_NUMBER);
	printf("generating matrix MC ...\n");
	MC->generate(FILL_NUMBER);
	printf("generating matrix MM ...\n");
	MM->generate(FILL_NUMBER);
	printf("generating matrix MZ ...\n");
	MZ->generate(FILL_NUMBER);

	//Sending all data into generate process
	//Send data to complete first task
	MPI_Send(A, SIZE*SIZE + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(B, SIZE*SIZE + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(C, SIZE*SIZE + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(MA, SIZE*SIZE + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(MZ, SIZE*SIZE + 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
	//Send data to complete second task
	MPI_Send(MA, SIZE*SIZE + 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(MB, SIZE*SIZE + 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD);
	//Send data to complete third task
	MPI_Send(A, SIZE*SIZE + 1, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(M, SIZE*SIZE + 1, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(MC, SIZE*SIZE + 1, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD);
	MPI_Send(MM, SIZE*SIZE + 1, MPI_INT, 3, MPI_ANY_TAG, MPI_COMM_WORLD);
	printf("Task1 started\n");
	Sleep(3000);
	printf("Calculating F1 ...\n");
	int d = (*B * *C) + (*A * *B) + (*C * *(*B * *(*MA * *MZ)));
	if (SIZE <= 10)
		printf("d = %d\n", d);
	delete A, B, C, MA, MZ;
	printf("Task1 finished\n");
	int a = 1;
	MPI_Send(&a, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD);
}


void task2() {
	printf("Task2 started\n");
	Matrix *MA ,*MB ;
	Sleep(1500);
	printf("Calculating F2 ...\n");
	Matrix *MC = (*MA->transpose() * *MB)->sort();
	if (SIZE <= 10)
	{
		printf("MC = \n");
		MC->print();
	}
	delete MA, MB, MC;
	printf("Task2 finished\n");
}


void task3() {
	printf("Task3 started\n");
	Vector *A, *M;
	Matrix *MC, *MM;
	printf("Calculating F3 ...\n");
	Sleep(100);
	Vector *D = *((*A + *M)->sort()) * *((*MC * *MM)->transpose());
	if (SIZE <= 10)
	{
		printf("D = \n");
		D->print();
	}
	delete D, A, M, MC, MM;
	printf("Task3 finished\n");
}