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
void test();

using namespace std;

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		int f1 = 0;
		Matrix *f2 = new Matrix(SIZE);
		Vector *f3 = new Vector(SIZE);
		f2->generate(FILL_NUMBER);
		f3->generate(FILL_NUMBER);
		char buffer[27];
		/*strcpy(buffer, "asd");
		MPI_Recv(buffer, 25, MPI_CHAR, MPI_ANY_SOURCE, PrintTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		printf(buffer);*/
		for (int i = 0; i < 18; i++)
		{
			MPI_Recv(buffer, 25, MPI_CHAR, MPI_ANY_SOURCE, PrintTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
			printf(buffer);
		}
		MPI_Recv(&f1, 1, MPI_INT, 1, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(&(f2->matrix[0][0]), SIZE*SIZE, MPI_INT, 2, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		MPI_Recv(&(f3->vector[0]), SIZE, MPI_INT, 3, RezultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		if (SIZE <= 10)
		{
			printf("d = %d\n", f1);
			printf("MC = \n");
			f2->print();
			printf("D = \n");
			f3->print();
		}
		cout << "Main thread close" << endl;
	}
	if (rank == 1)
		task1();
	if (rank == 2)
		task2();
	if (rank == 3)
		task3();
	MPI_Finalize();
	return 0;

}

void task1() {
	char buffer[27];
	strcpy(buffer, "Task1 started\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Vector *A = new Vector(SIZE),
		*B = new Vector(SIZE),
		*C = new Vector(SIZE);
	Matrix *MA = new Matrix(SIZE),
		*MZ = new Matrix(SIZE);
	strcpy(buffer, "generating vector A ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	A->generate(FILL_NUMBER);
	MPI_Send(&(A->vector[0]), SIZE, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);
	strcpy(buffer, "generating vector B ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	B->generate(FILL_NUMBER);
	strcpy(buffer, "generating vector C ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	C->generate(FILL_NUMBER);
	strcpy(buffer, "generating matrix MA ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MA->generate(FILL_NUMBER);
	MPI_Send(&(MA->matrix[0][0]), SIZE*SIZE, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);
	strcpy(buffer, "generating matrix MZ ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MZ->generate(FILL_NUMBER);
	strcpy(buffer, "Calculating F1 ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	int d = (*B * *C) + (*A * *B) + (*C * *(*B * *(*MA * *MZ)));
	delete A, B, C, MA, MZ;
	strcpy(buffer, "Task1 finished\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&d, 1, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
}


void task2() {
	char buffer[27];
	strcpy(buffer, "Task2 started\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Matrix *MA = new Matrix(SIZE),
		*MB = new Matrix(SIZE);
	MPI_Recv(&(MA->matrix[0][0]), SIZE*SIZE, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	strcpy(buffer, "generating matrix MB ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MB->generate(FILL_NUMBER);
	strcpy(buffer, "Calculating F2 ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Matrix *MC = (*MA->transpose() * *MB)->sort();
	strcpy(buffer, "Task2 finished\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&(MC->matrix[0][0]), SIZE*SIZE, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
	delete MA, MB, MC;
}


void task3() {
	char buffer[27];
	strcpy(buffer, "Task3 started\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Vector *A = new Vector(SIZE),
		*M = new Vector(SIZE);
	Matrix *MC = new Matrix(SIZE),
		*MM = new Matrix(SIZE);
	MPI_Recv(&(A->vector[0]), SIZE, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	strcpy(buffer, "generating vector M ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	M->generate(FILL_NUMBER);
	strcpy(buffer, "generating matrix MC ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MC->generate(FILL_NUMBER);
	strcpy(buffer, "generating matrix MM ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MM->generate(FILL_NUMBER);
	strcpy(buffer, "Calculating F3 ...\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	Vector *D = *((*A + *M)->sort()) * *((*MC * *MM)->transpose());
	strcpy(buffer, "Task3 finished\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
	MPI_Send(&(D->vector[0]), SIZE, MPI_INT, 0, RezultTag, MPI_COMM_WORLD);
	delete D, A, M, MC, MM;
}

void test()
{
	char buffer[27];
	strcpy(buffer, "Task1 started\n");
	MPI_Send(buffer, 25, MPI_CHAR, 0, PrintTag, MPI_COMM_WORLD);
}