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
#include <stdio.h>
#include "Data.h"

#define SIZE 10
#define FILL_NUMBER 1

#define PrintTag 1
#define ResourcesTag 2
#define RezultTag 3

void task1();
void task2();
void task3();

using namespace std;

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		int f1;
		Matrix *f2 = new Matrix(SIZE);
		Vector *f3 = new Vector(SIZE);
		char *buffer = new char[50];
		for (int i = 0; i < 18; i++)
			MPI_Recv(buffer, 50, MPI_CHAR, MPI_ANY_SOURCE, PrintTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(&f1, 1, MPI_INT, 1, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(f2, SIZE*SIZE + 1, MPI_INT, 2, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(f3, SIZE + 1, MPI_INT, 3, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		if (SIZE <= 10)
		{
			printf("d = %d\n", f1);
			printf("MC = \n");
			f2->print();
			printf("D = \n");
			f3->print();
		}
	}
	if (rank == 1)
		task1();
	if (rank == 2)
		task2();
	if (rank == 3)
		task3();
	cout << "Main thread close" << endl;
	MPI_Finalize();
	return 0;

}

void task1() {
	MPI_Send("Task1 started\n", 14, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Vector *A = new Vector(SIZE),
		*B = new Vector(SIZE),
		*C = new Vector(SIZE);
	Matrix *MA = new Matrix(SIZE),
		*MZ = new Matrix(SIZE);
	MPI_Send("generating vector A ...\n", 24, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	A->generate(FILL_NUMBER);
	MPI_Send(A, SIZE + 1, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send("generating vector B ...\n", 24, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	B->generate(FILL_NUMBER);
	MPI_Send("generating vector C ...\n", 24, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	C->generate(FILL_NUMBER);
	MPI_Send("generating matrix MA ...\n", 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MA->generate(FILL_NUMBER);
	MPI_Send(MA, SIZE*SIZE + 1, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send("generating matrix MZ ...\n", 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MZ->generate(FILL_NUMBER);
	Sleep(3000);
	MPI_Send("Calculating F1 ...\n", 19, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	int d = (*B * *C) + (*A * *B) + (*C * *(*B * *(*MA * *MZ)));
	delete A, B, C, MA, MZ;
	MPI_Send("Task1 finished\n", 15, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&d, 1, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
}


void task2() {
	MPI_Send("Task2 started\n", 14, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Matrix *MA = new Matrix(SIZE),
		*MB = new Matrix(SIZE);
	MPI_Recv(MA, SIZE*SIZE + 1, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send("generating matrix MB ...\n", 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MB->generate(FILL_NUMBER);
	Sleep(1500);
	MPI_Send("Calculating F2 ...\n", 19, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Matrix *MC = (*MA->transpose() * *MB)->sort();
	MPI_Send("Task2 finished\n", 15, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&MC, SIZE*SIZE+1, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
	delete MA, MB, MC;
}


void task3() {
	MPI_Send("Task3 started\n", 14, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Vector *A = new Vector(SIZE),
		*M = new Vector(SIZE);
	Matrix *MC = new Matrix(SIZE),
		*MM = new Matrix(SIZE);
	MPI_Recv(A, SIZE + 1, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send("generating vector M ...\n", 24, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	M->generate(FILL_NUMBER);
	MPI_Send("generating matrix MC ...\n", 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MC->generate(FILL_NUMBER);
	MPI_Send("generating matrix MM ...\n", 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MM->generate(FILL_NUMBER);
	MPI_Send("Calculating F3 ...\n", 19, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Sleep(100);
	Vector *D = *((*A + *M)->sort()) * *((*MC * *MM)->transpose());
	MPI_Send("Task3 finished\n", 15, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&D, SIZE + 1, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
	delete D, A, M, MC, MM;
}