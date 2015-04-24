/*
---------------------------------------------------------------------------------
--              Course Work. Part #3
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      21.04.2014
--      Task	  MA = MB*MC*alpha + (E*T)*MK
---------------------------------------------------------------------------------
*/

#include <mpi.h>
#include<iostream>

#include "data.h"

#define N 10
#define P 2
#define H N/P

#define T1Input 1
#define TpInput 2
#define ETResultTag 3
#define FinishResultTag 4

int getStart(int);
int getEnd(int);
void firstTask();
void lastTask();
void otherTask(int);

int main(int argc, char* argv[])
{
	std::printf("asd");
	int rank;
	if (P < 2) return 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	std::cout << "asd";
	std::printf("asd");
	if (rank == 0)
	{
		std::cout << "inside if\n";
		firstTask();
	}
	else if (rank == P - 1) lastTask();
	else otherTask(rank);

	MPI_Finalize();
	return 0;
}

void firstTask()
{
	std::cout << "t1 started";
	//First task started!!!
	int size = N, alpha;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	MB.generate();
	T.generate();

	//send MB, T data to the next thread;
	MPI_Send(MB.matrix[0], size*size, MPI_INT, 1, T1Input, MPI_COMM_WORLD);
	MPI_Send(T.vector, size, MPI_INT, 1, T1Input, MPI_COMM_WORLD);
	std::cout << "t1 send to t2";

	//receive MC, MK, E, alpha from the last thread
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	if (P > 2)
	{
		//send MC, MK, E, alpha to the next thread
		MPI_Send(MC.matrix[0], size*size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
	}

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(1), endIndex = getEnd(1);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];
	// receive result of E*T from other threads
	for (int i = 1; i < P; i++)
	{
		int res;
		MPI_Recv(&res, 1, MPI_INT, MPI_ANY_SOURCE, ETResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		ET_sum += res;
	}
	// send ET_sum to all threds except this
	for (int i = 1; i < P; i++)
		MPI_Send(&ET_sum, 1, MPI_INT, i, ETResultTag, MPI_COMM_WORLD);

	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_sum;
	}
	//receive final result from all threads
	int *buffer = new int[size*size];
	for (int i = 1; i < P; i++)
	{
		int begin = getStart(i + 1), end = getEnd(i + 1);
		MPI_Recv(buffer,size, MPI_INT, i, ETResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int j = begin; j < end; j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
	}
	delete buffer;
}

void lastTask()
{
	std::cout << "last started!!!";
	// Last task started!!!
	int size = N, alpha, ET_result;;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	// input MC, MK, E, alpha
	MC.generate();
	MK.generate();
	E.generate();
	alpha = 1;
	// receive T, MB from previous thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	// send MC, MK, E, alpha to the first thread
	MPI_Send(MC.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(MK.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(E.vector, size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 0, TpInput, MPI_COMM_WORLD);

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(P), endIndex = getEnd(P);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];
	// send result of E*T to the first thread
	MPI_Send(&ET_sum, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD);
	// receive ready sum result of E*T's parts
	MPI_Recv(&ET_result, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_result;
	}
	MPI_Send(MA.matrix[0], size*size, MPI_INT, 0, FinishResultTag, MPI_COMM_WORLD);
}

void otherTask(int id)
{
	int size = N, alpha, ET_result;;
	Matrix MA(N), MB(N), MC(N), MK(N);
	Vector E(N), T(N);

	// receive T, MB from previous thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, id - 1, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, id - 1, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	//send MB, T data to the next thread;
	MPI_Send(MB.matrix[0], size*size, MPI_INT, id + 1, T1Input, MPI_COMM_WORLD);
	MPI_Send(T.vector, size, MPI_INT, id + 1, T1Input, MPI_COMM_WORLD);

	//receive MC, MK, E, alpha from the last thread
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, id - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, id - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, id - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, id - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	//send MC, MK, E, alpha to the next thread
	MPI_Send(MC.matrix[0], size*size, MPI_INT, id + 1, TpInput, MPI_COMM_WORLD);
	MPI_Send(MK.matrix[0], size*size, MPI_INT, id + 1, TpInput, MPI_COMM_WORLD);
	MPI_Send(E.vector, size, MPI_INT, id + 1, TpInput, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, id + 1, TpInput, MPI_COMM_WORLD);

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(id + 1), endIndex = getEnd(id + 1);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];
	// send result of E*T to the first thread
	MPI_Send(&ET_sum, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD);
	// receive ready sum result of E*T's parts
	MPI_Recv(&ET_result, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_result;
	}
	MPI_Send(MA.matrix[0], size*size, MPI_INT, 0, FinishResultTag, MPI_COMM_WORLD);
}

int getStart(int number)
{
	int h = N / P;
	int h0 = N % P;
	h0 = number <= h0 ? number - 1 : h0;
	return (number - 1) * h + h0;
}

int getEnd(int number)
{
	int h = N / P;
	int h0 = N % P;
	h0 = number <= h0 ? number : h0;
	return number * h + h0;
}