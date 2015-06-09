/*
---------------------------------------------------------------------------------
--              Laboratory Work #8
--        Lab8. Tasks in MPI library
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      09.06.2015
--      F        MA = MB*MC + alpha*MK
---------------------------------------------------------------------------------
*/


#include <iostream>
#include <windows.h>
#include <mpi.h>
#include <stdio.h>
#include "Data.h"

//#define N 1650
#define N 11
#define P 11
#define H N/P
#define MIN_SIZE H*N

#define FILL_NUMBER 1

#define ResourcesTag 1
#define ResultTag 2

using namespace std;

void calculate(Matrix *MB, Matrix *MC, Matrix *MK, int alpha,int* dest)
{
	for (int i = 0; i < H; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB->matrix[i][k] * MC->matrix[k][j];
		*(dest + i*N + j) = sum + alpha * MK->matrix[i][j];
	}
}

void task1()
{
	auto MA = new Matrix(N),
		MB = new Matrix(N),
		MC = new Matrix(N),
		MK = new Matrix(N);
	auto alpha = 1;
	MA->generate(0);
	MB->generate(FILL_NUMBER);
	MC->generate(FILL_NUMBER);
	MK->generate(FILL_NUMBER);

	MPI_Send(MB->matrix[1], 3 * MIN_SIZE, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MK->matrix[1], 3 * MIN_SIZE, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD);

	MPI_Send(MB->matrix[4], 3 * MIN_SIZE, MPI_INT, 4, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MK->matrix[4], 3 * MIN_SIZE, MPI_INT, 4, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 4, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 4, ResourcesTag, MPI_COMM_WORLD);

	MPI_Send(MB->matrix[7], 4 * MIN_SIZE, MPI_INT, 7, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MK->matrix[7], 4 * MIN_SIZE, MPI_INT, 7, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 7, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 7, ResourcesTag, MPI_COMM_WORLD);

	calculate(MB, MC, MK, alpha, MA->matrix[0]);

	MPI_Recv(MA->matrix[H], MIN_SIZE * 3, MPI_INT, 1, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MA->matrix[4 * H], MIN_SIZE * 3, MPI_INT, 4, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MA->matrix[7 * H], MIN_SIZE * 4, MPI_INT, 7, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	Sleep(2000);
	MA->print();
	delete MA, MB, MC, MK;
}
void task2()
{
	const auto buffer_size = 3 * MIN_SIZE;
	auto buff = new int[buffer_size];
	auto MB = new Matrix(H,N),
		MC = new Matrix(N),
		MK = new Matrix(H,N);
	auto alpha = 0;

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MB->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, 2 * MIN_SIZE, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MK->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, 2 * MIN_SIZE, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(MC->matrix[0], N*N, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(&alpha, 1, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(&alpha, 1, MPI_INT, 2, ResourcesTag, MPI_COMM_WORLD);

	calculate(MB, MC, MK, alpha, buff);

	MPI_Recv(buff + MIN_SIZE, MIN_SIZE * 2, MPI_INT, 2, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(buff, buffer_size, MPI_INT, 0, ResultTag, MPI_COMM_WORLD);
	delete MB, MC, MK, buff;
}
void task3()
{
	const auto buffer_size = 2 * MIN_SIZE;
	auto buff = new int[buffer_size];
	auto MB = new Matrix(H,N),
		MC = new Matrix(N),
		MK = new Matrix(H,N);
	auto alpha = 0;

	MPI_Recv(buff, buffer_size, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MB->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(buff, buffer_size, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MK->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(MC->matrix[0], N*N, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(&alpha, 1, MPI_INT, 1, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(&alpha, 1, MPI_INT, 3, ResourcesTag, MPI_COMM_WORLD);

	calculate(MB, MC, MK, alpha, buff);

	MPI_Recv(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 3, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(buff, buffer_size, MPI_INT, 1, ResultTag, MPI_COMM_WORLD);
	delete MB, MC, MK, buff;
}
void task5()
{
	const auto buffer_size = 3 * MIN_SIZE;
	auto buff = new int[buffer_size];
	auto MB = new Matrix(H,N),
		MC = new Matrix(N),
		MK = new Matrix(H,N);
	auto alpha = 0;

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MB->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 5, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 6, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MK->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 5, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 6, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(MC->matrix[0], N*N, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 5, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 6, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(&alpha, 1, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(&alpha, 1, MPI_INT, 5, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 6, ResourcesTag, MPI_COMM_WORLD);

	calculate(MB, MC, MK, alpha, buff);

	MPI_Recv(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 5, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 6, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(buff, buffer_size, MPI_INT, 0, ResultTag, MPI_COMM_WORLD);
	delete MB, MC, MK, buff;
}
void task8()
{
	const auto buffer_size = 4 * MIN_SIZE;
	auto buff = new int[buffer_size];
	auto MB = new Matrix(H,N),
		MC = new Matrix(N),
		MK = new Matrix(H,N);
	auto alpha = 0;

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MB->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 8, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 9, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 3, MIN_SIZE, MPI_INT, 10, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(buff, buffer_size, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	memcpy(MK->matrix[0], buff, MIN_SIZE*sizeof(int));
	MPI_Send(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 8, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 9, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(buff + MIN_SIZE * 3, MIN_SIZE, MPI_INT, 10, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(MC->matrix[0], N*N, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 8, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT, 9, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(MC->matrix[0], N*N, MPI_INT,10, ResourcesTag, MPI_COMM_WORLD);

	MPI_Recv(&alpha, 1, MPI_INT, 0, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(&alpha, 1, MPI_INT, 8, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 9, ResourcesTag, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT,10, ResourcesTag, MPI_COMM_WORLD);

	calculate(MB, MC, MK, alpha, buff);

	MPI_Recv(buff + MIN_SIZE, MIN_SIZE, MPI_INT, 8, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(buff + MIN_SIZE * 2, MIN_SIZE, MPI_INT, 9, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(buff + MIN_SIZE * 3, MIN_SIZE, MPI_INT, 10, ResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Send(buff, buffer_size, MPI_INT, 0, ResultTag, MPI_COMM_WORLD);
}
void lastTasks(int parentProcess)
{
	auto MB = new Matrix(H, N),
		MC = new Matrix(N),
		MK = new Matrix(H, N);
	auto alpha = 0;
	int result[MIN_SIZE];

	MPI_Recv(MB->matrix[0], MIN_SIZE, MPI_INT, parentProcess, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK->matrix[0], MIN_SIZE, MPI_INT, parentProcess, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MC->matrix[0], N*N, MPI_INT, parentProcess, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, parentProcess, ResourcesTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	calculate(MB, MC, MK, alpha, &result[0]);

	MPI_Send(result, MIN_SIZE, MPI_INT, parentProcess, ResultTag, MPI_COMM_WORLD);
	delete MB, MC, MK;
}

void runTasks(int rank)
{
	printf("Task #%d started!!!\n", rank + 1);
	cout.flush();
	switch (rank)
	{
	case 0:
		task1();
		break;
	case 1:
		task2();
		break;
	case 2:
		task3();
		break;
	case 3:
		lastTasks(2);
		break;
	case 4:
		task5();
		break;
	case 5:
		lastTasks(4);
		break;
	case 6:
		lastTasks(4);
		break;
	case 7:
		task8();
		break;
	case 8:
		lastTasks(7);
		break;
	case 9:
		lastTasks(7);
		break;
	case 10:
		lastTasks(7);
		break;
	default:
		break;
	}
	printf("Task #%d finished!!!\n", rank + 1);
	cout.flush();
}

int main(int argc, char** argv)
{
	int rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	runTasks(rank);
	MPI_Finalize();
	return 0;

}

