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

#define N 2000
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
	int rank;
	if (P < 2) return 0;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
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
	std::cout << "t1 started\n";
	// First task started!!!
	int size = N, alpha;
	Matrix MA(size), MB(size), MC(size), MK(size);
	Vector E(size), T(size);
	MB.generate();
	T.generate();

	std::cout << "1\n";
	std::cout.flush();

	// send MB, T data to the last thread;
	MPI_Send(MB.matrix[0], size*size, MPI_INT, P - 1, T1Input, MPI_COMM_WORLD);
	MPI_Send(T.vector, size, MPI_INT, P - 1, T1Input, MPI_COMM_WORLD);
	
	std::cout << "2\n";
	std::cout.flush();

	// receive MC, MK, E, alpha from the last thread
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, P - 1, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	std::cout << "3\n";
	std::cout.flush();

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

	std::cout << "4\n";
	std::cout.flush();

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

	std::cout << "5\n";
	std::cout.flush();

	// receive result of E*T from last node
	MPI_Recv(&tempETSum, 1, MPI_INT, P - 1, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	ET_sum += tempETSum;
	std::cout << "5.1\n";
	std::cout.flush();
	// send result of E*T to the next and last node
	if (P > 2)
		MPI_Send(&ET_sum, 1, MPI_INT, 1, ETResultTag, MPI_COMM_WORLD);
	MPI_Send(&ET_sum, 1, MPI_INT, P - 1, ETResultTag, MPI_COMM_WORLD);

	std::cout << "6\n";
	std::cout.flush();

	for (int i = startIndex; i < endIndex; i++)
	for (int j = 0; j < N; j++)
	{
		int sum = 0;
		for (int k = 0; k < N; k++)
			sum += MB.matrix[i][k] * MC.matrix[k][j];
		sum *= alpha;

		MA.matrix[i][j] = sum + MK.matrix[i][j] * ET_sum;
	}
	std::cout << "7\n";
	std::cout.flush();
	//receive final result from all threads
	int *buffer = new int[size*size];
	if (P > 2)
	{
		std::printf("Task %d. --before-- MA[%d][0] = %d\n", 0, getStart(2), MA.matrix[getStart(2)][0]);
		std::printf("Task %d. --before-- MA[%d][0] = %d\n", 0, getEnd(P / 2+1) - 1, MA.matrix[getEnd(P / 2+1) - 1][0]);
		std::cout.flush();
		MPI_Recv(buffer, size*size, MPI_INT, 1, FinishResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int j = getStart(2); j < getEnd(P/2+1); j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
		std::printf("Task %d. --after-- MA[%d][0] = %d\n", 0, getStart(2), MA.matrix[getStart(2)][0]);
		std::printf("Task %d. --after-- MA[%d][0] = %d\n", 0, getEnd(P / 2) - 1, MA.matrix[getEnd(P / 2) - 1][0]);
		std::cout.flush();
	}
	MPI_Recv(&buffer[0], size*size, MPI_INT, P - 1, FinishResultTag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	std::printf("Task %d. --before-- MA[%d][0] = %d\n", 0, 1, MA.matrix[1][0]);
	std::printf("Task %d. --before-- MA[%d][0] = %d\n", 0, getEnd(P)-1, MA.matrix[getEnd(P)-1][0]);
	std::cout.flush();
	for (int j = getStart(P==2?2:P/2 + 2); j < getEnd(P); j++)
	for (int k = 0; k < size; k++)
		MA.matrix[j][k] = *(buffer + j*size + k);
	delete buffer;
	std::printf("Task %d. --after-- MA[%d][0] = %d\n", 0, 1, MA.matrix[1][0]);
	std::printf("Task %d. --after-- MA[%d][0] = %d\n", 0, getEnd(P)-1, MA.matrix[getEnd(P)-1][0]);
	std::cout.flush();

	std::cout << "9\n";
	std::cout.flush();

	std::cout << "t1 finished\n";
	if (N <= 10)
		MA.print();
}

void lastTask()
{
	std::cout << "Task " << P - 1 << ". Started!\n";
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
	std::cout << "Task " << P - 1 << ". Before exchange data with task 0!\n";
	std::cout.flush();
	// receive T, MB from first thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, 0, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, 0, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);

	// send MC, MK, E, alpha to the first thread
	MPI_Send(MC.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(MK.matrix[0], size*size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(E.vector, size, MPI_INT, 0, TpInput, MPI_COMM_WORLD);
	MPI_Send(&alpha, 1, MPI_INT, 0, TpInput, MPI_COMM_WORLD);

	std::cout << "Task " << P - 1 << ". After exchange data with task 0!\n";
	std::cout.flush();

	if (P > 4)
	{
		std::cout << "Task " << P - 1 << ". Before send data to the task "<< P - 2 << "\n";
		std::cout.flush();
		// send all data to the next thread
		MPI_Send(MB.matrix[0], size*size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD);
		MPI_Send(T.vector, size, MPI_INT, P - 2, T1Input, MPI_COMM_WORLD);
		MPI_Send(MC.matrix[0], size*size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, P - 2, TpInput, MPI_COMM_WORLD);
		std::cout << "Task " << P - 1 << ". After send data to the task " << P - 2 << "\n";
		std::cout.flush();
	}

	// calculate E*T
	int ET_sum = 0, startIndex = getStart(P), endIndex = getEnd(P);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];

	// receive result of E*T from previous node
	if (P > 4)
	{
		std::cout << "Task " << P - 1 << ". Before receive ET_sum from the task " << P - 2 << "\n";
		std::cout.flush();
		int tempETSum = 0;
		MPI_Recv(&tempETSum, 1, MPI_INT, P - 2, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		ET_sum += tempETSum;

		std::cout << "Task " << P - 1 << ". After receive ET_sum from the task " << P - 2 << "\n";
		std::cout.flush();
	}
	std::cout << "before send et_sum from last task to first\n";
	std::cout.flush();
	// send result of E*T to the next node
	MPI_Send(&ET_sum, 1, MPI_INT, 0, ETResultTag, MPI_COMM_WORLD);
	std::cout << "after send et_sum from last task to first\n";
	std::cout.flush();

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
		std::cout << "Task " << P - 1 << ". Before receive MA from the task " << P - 2 << "\n";
		std::cout.flush();
		int *buffer = new int[size*size];
		MPI_Recv(buffer, size*size, MPI_INT, P - 2, FinishResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		for (int j = getStart(P / 2 + 2); j < getEnd(P - 1); j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
		delete buffer;
		std::cout << "Task " << P - 1 << ". After receive MA from the task " << P - 2 << "\n";
		std::cout.flush();
	}
	std::cout << "Task " << P - 1 << ". ET_sum =  " << ET_sum << " MA.matrix[" << 4 << "][0] = " << MA.matrix[4][0] << "\n";
	MPI_Send(MA.matrix[0], size*size, MPI_INT, 0, FinishResultTag, MPI_COMM_WORLD);
	std::cout << "Task " << P - 1 << ". Finished!\n";
	std::cout.flush();
}

void otherTask(int id)
{
	std::cout << "Task " << id << ". Started!\n";
	std::cout.flush();
	int size = N, alpha, ET_result, difference = id <= P / 2 ? -1 : 1;
	Matrix MA(N), MB(N), MC(N), MK(N);
	Vector E(N), T(N);

	std::cout << "Task " << id << ". Before receive data from the task " << id + difference << "\n";
	std::cout.flush();
	//receive all data from prev or next thread
	MPI_Recv(MB.matrix[0], size*size, MPI_INT, id + difference, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(T.vector, size, MPI_INT, id + difference, T1Input, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MC.matrix[0], size*size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(MK.matrix[0], size*size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(E.vector, size, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	MPI_Recv(&alpha, 1, MPI_INT, id + difference, TpInput, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	std::cout << "Task " << id << ". After receive data from the task " << id + difference << "\n";
	std::cout.flush();

	//send data to the next thread 
	if ((id <= P / 2 - 1) || (id > P / 2 + 1))
	{
		std::cout << "Task " << id << ". Before send data to the task " << id - difference << "\n";
		std::cout.flush();
		MPI_Send(MB.matrix[0], size*size, MPI_INT, id - difference, T1Input, MPI_COMM_WORLD);
		MPI_Send(T.vector, size, MPI_INT, id - difference, T1Input, MPI_COMM_WORLD);
		MPI_Send(MC.matrix[0], size*size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(MK.matrix[0], size*size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(E.vector, size, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);
		MPI_Send(&alpha, 1, MPI_INT, id - difference, TpInput, MPI_COMM_WORLD);

		std::cout << "Task " << id << ". After send data to the task " << id - difference << "\n";
		std::cout.flush();
	}
	// calculate E*T
	int ET_sum = 0, startIndex = getStart(id + 1), endIndex = getEnd(id + 1);
	for (int i = startIndex; i < endIndex; i++)
		ET_sum += E.vector[i] * T.vector[i];

	// receive result of E*T from previous node
	if ((id <= P / 2 - 1) || (id > P / 2 + 1))
	{
		int tempETSum = 0;
		MPI_Recv(&tempETSum, 1, MPI_INT, id - difference, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		ET_sum += tempETSum;
	}
	// send result of E*T to the next node
	MPI_Send(&ET_sum, 1, MPI_INT, id + difference, ETResultTag, MPI_COMM_WORLD);

	// receive ready sum result of E*T's parts
	MPI_Recv(&ET_result, 1, MPI_INT, id + difference, ETResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
	if ((id <= P / 2 - 1) || (id > P / 2 + 1))
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
	if ((id <= P / 2 - 1) || (id > P / 2 + 1))
	{
		int *buffer = new int[size*size];
		MPI_Recv(buffer, size*size, MPI_INT, id - difference, FinishResultTag, MPI_COMM_WORLD, MPI_STATUSES_IGNORE);
		int begin = getStart(difference < 0 ? id - difference + 1 : P / 2 + 2), end = getEnd(difference < 0 ? P / 2 + 1 : id - difference + 1);
		std::printf("Task %d. --before-- MA[%d][0] = %d\n", id, begin, MA.matrix[begin][0]);
		std::printf("Task %d. --before-- MA[%d][0] = %d\n", id, end - 1, MA.matrix[end - 1][0]);
		std::cout.flush();
		for (int j = begin; j < end; j++)
		for (int k = 0; k < size; k++)
			MA.matrix[j][k] = *(buffer + j*size + k);
		delete buffer;
		std::printf("Task %d. --after-- MA[%d][0] = %d\n", id, begin, MA.matrix[begin][0]);
		std::printf("Task %d. --after-- MA[%d][0] = %d\n", id, end - 1, MA.matrix[end - 1][0]);
		std::cout.flush();
	}
	MPI_Send(MA.matrix[0], size*size, MPI_INT, id + difference, FinishResultTag, MPI_COMM_WORLD);
	std::cout << "Task " << id << ". Finished!\n";
	std::cout.flush();
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