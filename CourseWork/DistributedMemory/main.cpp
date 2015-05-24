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
#include <ctime>

#include "data.h"

#define N 1300
#define P 1
#define H N/P
#define mid P/2

#define T1Input 1
#define TpInput 2
#define ETResultTag 3
#define FinishResultTag 4

int getStart(int);
int getEnd(int);
void firstTask();
void lastTask();
void otherTask(int);
void oneTask();

int main(int argc, char* argv[])
{
	clock_t tStart = clock();
	int rank, count;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &count);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (count == 1)
		oneTask();
	else
	{
		if (rank == 0)	firstTask();
		else if (rank == P - 1) lastTask();
		else otherTask(rank);
	}
	if (rank == 0)
		std::printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	MPI_Finalize();
	return 0;
}

void firstTask()
{
	std::cout << "Task " << 1 << ". Started!\n";
	std::cout.flush();
	// First task started!!!
	int size = N, alpha;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	MB.generate();
	T.generate();

	// send MB, T data to the last thread;
	MPI_Send(MB.matrix[0], size*size, MPI_INT, P - 1, T1Input, MPI_COMM_WORLD);
	MPI_Send(T.vector, size, MPI_INT, P - 1, T1Input, MPI_COMM_WORLD);
	
	// receive MC, MK, E, alpha from the last thread
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	if (P > 2)
	{
		// send all data to the next thread
		MPI_Send(MB.matrix[0], size*size, MPI_INT, 1, T1Input, MPI_COMM_WORLD);
		MPI_Send(T.vector, size, MPI_INT, 1, T1Input, MPI_COMM_WORLD);
		MPI_Send(MC.matrix[0], size*size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, 1, TpInput, MPI_COMM_WORLD);
	}

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(1), endIndex = getEnd(1);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];
	int tempETSum = 0;
	if (P > 2)
	{
		// receive result of E*T from previous node
		MPI_Recv(&tempETSum, 1, MPI_INT, 1, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		ET_sum += tempETSum;
	}

	// receive result of E*T from last node
	MPI_Recv(&tempETSum, 1, MPI_INT, P - 1, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	ET_sum += tempETSum;

	// send result of E*T to the next and last node
	if (P > 2)
		MPI_Send(&ET_sum, 1, MPI_INT, 1, ETResultTag, MPI_COMM_WORLD);
	MPI_Send(&ET_sum, 1, MPI_INT, P - 1, ETResultTag, MPI_COMM_WORLD);

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
	if (P > 2)
	{
		MPI_Recv(buffer, size*size, MPI_INT, 1, FinishResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int j = getStart(2); j < getEnd(P/2+1); j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
	}
	MPI_Recv(&buffer[0], size*size, MPI_INT, P - 1, FinishResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	for (int j = getStart(P==2?2:P/2 + 2); j < getEnd(P); j++)
	for (int k = 0; k < size; k++)
		MA.matrix[j][k] = *(buffer + j*size + k);
	delete buffer;
	std::cout << "Task " << 1 << ". Finished!\n";
	std::cout.flush();
	if (N <= 10)
		MA.print();
}

void lastTask()
{
	std::cout << "Task " << P << ". Started!\n";
	std::cout.flush();
	// Last task started!!!
	int size = N, alpha, ET_result;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	// input MC, MK, E, alpha
	MC.generate();
	MK.generate();
	E.generate();
	alpha = 1;
	// receive T, MB from first thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, 0, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, 0, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	// send MC, MK, E, alpha to the first thread
	MPI_Send(MC.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(MK.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(E.vector, size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 0, TpInput, MPI_COMM_WORLD);

	if (P > 4)
	{
		// send all data to the next thread
		MPI_Send(MB.matrix[0], size*size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD);
		MPI_Send(T.vector, size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD);
		MPI_Send(MC.matrix[0], size*size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
	}

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(P), endIndex = getEnd(P);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];

	// receive result of E*T from previous node
	if (P > 4)
	{
		int tempETSum = 0;
		MPI_Recv(&tempETSum, 1, MPI_INT, P - 2, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		ET_sum += tempETSum;
	}
	// send result of E*T to the T1
	MPI_Send(&ET_sum, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD);

	// receive ready sum result of E*T's parts
	MPI_Recv(&ET_result, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	if (P > 4)
		MPI_Send(&ET_result, 1, MPI_INT, P - 2, ETResultTag, MPI_COMM_WORLD);

	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_result;
	}
	//receive final result from previous node

	std::cout.flush();
	if (P > 4)
	{
		int *buffer = new int[size*size];
		MPI_Recv(buffer, size*size, MPI_INT, P - 2, FinishResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		for (int j = getStart(P / 2 + 2); j < getEnd(P - 1); j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
		delete buffer;
	}
	MPI_Send(MA.matrix[0], size*size, MPI_INT, 0, FinishResultTag, MPI_COMM_WORLD);
	std::cout << "Task " << P << ". Finished!\n";
	std::cout.flush();
}

void otherTask(int id)
{
	std::cout << "Task " << id + 1 << ". Started!\n";
	std::cout.flush();
	int size = N, alpha, ET_result, difference = id <= mid ? -1 : 1;
	Matrix MA(N), MB(N), MC(N), MK(N);
	Vector E(N), T(N);

	//receive all data from prev or next thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, id + difference, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, id + difference, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	//send data to the next thread 
	if ((id <= mid - 1) || (id > mid + 1))
	{
		MPI_Send(MB.matrix[0], size*size, MPI_INT, id - difference, T1Input, MPI_COMM_WORLD);
		MPI_Send(T.vector, size, MPI_INT, id - difference, T1Input, MPI_COMM_WORLD);
		MPI_Send(MC.matrix[0], size*size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
	}
	// calculate E*T
	int ET_sum = 0, startIndex = getStart(id + 1), endIndex = getEnd(id + 1);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];

	// receive result of E*T from previous node
	if ((id <= mid - 1) || (id > mid + 1))
	{
		int tempETSum = 0;
		MPI_Recv(&tempETSum, 1, MPI_INT, id - difference, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		ET_sum += tempETSum;
	}
	// send result of E*T to the next node
	MPI_Send(&ET_sum, 1, MPI_INT, id + difference, ETResultTag, MPI_COMM_WORLD);

	// receive ready sum result of E*T's parts
	MPI_Recv(&ET_result, 1, MPI_INT, id + difference, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	if ((id <= mid - 1) || (id > mid + 1))
		MPI_Send(&ET_result, 1, MPI_INT, id - difference, ETResultTag, MPI_COMM_WORLD);


	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_result;
	}
	//receive final result from previous node
	if ((id <= mid - 1) || (id > mid + 1))
	{
		int *buffer = new int[size*size];
		MPI_Recv(buffer, size*size, MPI_INT, id - difference, FinishResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		int begin = getStart(difference < 0 ? id - difference + 1 : mid + 2), end = getEnd(difference < 0 ? mid + 1 : id - difference + 1);
		for (int j = begin; j < end; j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
		delete buffer;
	}
	MPI_Send(MA.matrix[0], size*size, MPI_INT, id + difference, FinishResultTag, MPI_COMM_WORLD);
	std::cout << "Task " << id + 1 << ". Finished!\n";
	std::cout.flush();
}

void oneTask()
{
	// First task started!!!
	std::cout << "Task " << 1 << ". Started!\n";
	std::cout.flush();
	int size = N, alpha;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	// input data
	MB.generate();
	MC.generate();
	MK.generate();
	E.generate();
	T.generate();
	alpha = 1;
	
	// calculate E*T
	int ET_sum = 0;
	for (int i = 0; i < size; i++)
		ET_sum += E.vector[i] * T.vector[i];
	// calculate MA
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
	{
		int sum = 0;
		for (int k = 0; k < size; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;
		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_sum;
	}
	std::cout << "Task " << 1 << ". Finished!\n";
	std::cout.flush();
	if (N <= 10)
		MA.print();
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