﻿/*
----------------------------------------
-- Lab2                               --
-- Pashchuk Eduard Fedorovich         --
-- Group IP-22                        --
-- A = sort(B)*alpha + beta*C*(MO*MK) --
-- Data : 27.03.2015                  --
----------------------------------------
*/

#include <windows.h>
#include <iostream>
#include "Data.h"

#define N 4
#define P 4
#define H N/P
#define min 0

void bubblesort(int* vectorPart, int size);
void merge(int* vectorPart1, int* vectorPart2, int size1, int size2, int* dest);

using namespace std;

int alpha, beta;
Vector A(N), B(N), C(N), Z(N);
Matrix MO(N), MK(N), MX(N);

HANDLE S1, S2;
HANDLE M;
HANDLE E1, E2, E3, E4, E5, E6, E7, E8, E9, E10, E11, E12;
CRITICAL_SECTION CrSec;

void t1() {
	int processID = 0;
	printf("t1 started!!!\n");
	//ввід B, MO
	//B.generate();
	B.generate();
	MO.generate();
	//Сигнал Т2,Т3,Т4 про завершення вводу.
	ReleaseSemaphore(S1, 3, NULL);
	//Чекати на закінчення вводу в потоку Т4
	WaitForSingleObject(S2, INFINITE);
	//Копіювання аlpha, beta, МK
	WaitForSingleObject(M, INFINITE);
	int a1 = alpha;
	int b1 = beta;
	Matrix MK1(N);
	MK1.copy(MK);
	ReleaseMutex(M);
	//Знаходження MXh = MOh * MK1
	for (int i = processID*H; i < processID*H + H; i++)
	for (int j = 0; j < MK1.SIZE; j++)
	{
		int sum = 0;
		for (int k = 0; k < MK1.SIZE; k++)
			sum += MO.matrix[i][k] * MK1.matrix[k][j];
		MX.matrix[i][j] = sum;
	}
	//Сигнал потокам Т2, Т3, Т4 про завершення рахування MX
	SetEvent(E1);

	//Очікування завершення рахування MX у потоках T2, T3, T4
	WaitForSingleObject(E2, INFINITE);
	WaitForSingleObject(E3, INFINITE);
	WaitForSingleObject(E4, INFINITE);

	//Знаходження Zh(1) = sort(Bh)
	bubblesort(B.vector + (processID*H), H);

	//Очікування завершення сортування Zh(2) у потоці Т2
	WaitForSingleObject(E5, INFINITE);

	//Знаходження Z2h(i) = merge(Zh(1), Zh(2))
	merge(B.vector, B.vector + H, H, H, Z.vector);
	std::memcpy(B.vector, Z.vector, 8 * H);

	//Очікування завершення сортування Z2h(2) у потоці Т3
	WaitForSingleObject(E7, INFINITE);

	//Знаходження Z = merge(Z2h(1), Z2h(2))
	merge(B.vector, B.vector + 2 * H, 2 * H, 2 * H, Z.vector);

	//Сигнал про завершення сортування потокам Т2, Т3, Т4
	SetEvent(E8);

	//Копіювання C1 = C
	EnterCriticalSection(&CrSec);
	Vector C1(N);
	C1.copy(C);
	LeaveCriticalSection(&CrSec);

	//Знаходження Ah = Zh * alpha1 + beta1 * C1 * MXh
	for (int i = processID*H; i < processID*H + H; i++)
	{
		int res = 0;
		for (int j = 0; j < Z.SIZE; j++)
			res += C1.vector[j] * MX.matrix[j][i];
		A.vector[i] = Z.vector[i] * a1 + b1*res;
	}

	//Очікування завершення обчислень у потоках T2, T3, T4
	WaitForSingleObject(E10, INFINITE);
	WaitForSingleObject(E11, INFINITE);
	WaitForSingleObject(E12, INFINITE);
	A.print();
	printf("t1 finished!!!\n");
}

void t2() {
	int processID = 1;
	printf("t2 started!!!\n");
	//Чекати на закінчення вводу в потоках Е1, Т4
	WaitForSingleObject(S1, INFINITE);
	WaitForSingleObject(S2, INFINITE);

	//Копіювання аlpha, beta, МK
	WaitForSingleObject(M, INFINITE);
	int a2 = alpha;
	int b2 = beta;
	Matrix MK2(N);
	MK2.copy(MK);
	ReleaseMutex(M);
	//Знаходження MXh = MOh * MK2
	for (int i = processID*H; i < processID*H + H; i++)
	for (int j = 0; j < MK2.SIZE; j++)
	{
		int sum = 0;
		for (int k = 0; k < MK2.SIZE; k++)
			sum += MO.matrix[i][k] * MK2.matrix[k][j];
		MX.matrix[i][j] = sum;
	}
	//Сигнал потокам Т1, Т3, Т4 про завершення рахування MX
	SetEvent(E2);

	//Очікування завершення рахування MX у потоках T1, T3, T4
	WaitForSingleObject(E1, INFINITE);
	WaitForSingleObject(E3, INFINITE);
	WaitForSingleObject(E4, INFINITE);

	//Знаходження Zh(2) = sort(Bh)
	bubblesort(B.vector + (processID*H), H);

	//Сигнал про завершення сортування Zh(2) потоку Т1
	SetEvent(E5);

	//Очікування завершення сортування у потоці Т1
	WaitForSingleObject(E8, INFINITE);

	//Копіювання C2 = C
	EnterCriticalSection(&CrSec);
	Vector C2(N);
	C2.copy(C);
	LeaveCriticalSection(&CrSec);

	//Знаходження Ah = Zh * alpha2 + beta2 * C2 * MXh
	for (int i = processID*H; i < processID*H + H; i++)
	{
		int res = 0;
		for (int j = 0; j < Z.SIZE; j++)
			res += C2.vector[j] * MX.matrix[j][i];
		A.vector[i] = Z.vector[i] * a2 + b2*res;
	}

	//Сигнал про завершення обчислень потоку T1
	SetEvent(E10);
	printf("t2 finished!!!\n");
}

void t3() {
	int processID = 2;
	printf("t3 started!!!\n");
	//Чекати на закінчення вводу в потоках Е1, Т4
	WaitForSingleObject(S1, INFINITE);
	WaitForSingleObject(S2, INFINITE);

	//Копіювання аlpha, beta, МK
	WaitForSingleObject(M, INFINITE);
	int a3 = alpha;
	int b3 = beta;
	Matrix MK3(N);
	MK3.copy(MK);
	ReleaseMutex(M);
	//Знаходження MXh = MOh * MK3
	for (int i = processID*H; i < processID*H + H; i++)
	for (int j = 0; j < MK3.SIZE; j++)
	{
		int sum = 0;
		for (int k = 0; k < MK3.SIZE; k++)
			sum += MO.matrix[i][k] * MK3.matrix[k][j];
		MX.matrix[i][j] = sum;
	}
	//Сигнал потокам Т1, Т2, Т4 про завершення рахування MX
	SetEvent(E3);

	//Очікування завершення рахування MX у потоках T1, T2, T4
	WaitForSingleObject(E1, INFINITE);
	WaitForSingleObject(E2, INFINITE);
	WaitForSingleObject(E4, INFINITE);

	//Знаходження Zh(3) = sort(Bh)
	bubblesort(B.vector + (processID*H), H);

	//Очікування завершення сортування Zh(4) у потоці Т4
	WaitForSingleObject(E6, INFINITE);

	//Знаходження Z2h(2) = merge(Zh(3), Zh(4))
	merge(B.vector + 2 * H, B.vector + 3 * H, H, H, Z.vector + 2 * H);
	std::memcpy(B.vector + 2 * H, Z.vector + 2 * H, 8 * H);

	//Сигнал про завершення сортування Z2h(2) потоку Т1
	SetEvent(E7);

	//Очікування завершення сортування у потоці Т1
	WaitForSingleObject(E8, INFINITE);

	//Копіювання C3 = C
	EnterCriticalSection(&CrSec);
	Vector C3(N);
	C3.copy(C);
	LeaveCriticalSection(&CrSec);

	//Знаходження Ah = Zh * alpha3 + beta3 * C3 * MXh
	for (int i = processID*H; i < processID*H + H; i++)
	{
		int res = 0;
		for (int j = 0; j < Z.SIZE; j++)
			res += C3.vector[j] * MX.matrix[j][i];
		A.vector[i] = Z.vector[i] * a3 + b3*res;
	}

	//Сигнал про завершення обчислень потоку T1
	SetEvent(E11);
	printf("t3 finished!!!\n");
}

void t4() {
	int processID = 3;
	printf("t4 started!!!\n");
	//ввід C, alpha, beta, MK
	C.generate();
	alpha = 1;
	beta = 1;
	MK.generate();
	//Сигнал Т1,Т2,Т3 про завершення вводу.
	ReleaseSemaphore(S2, 3, NULL);
	//Чекати на закінчення вводу в потоці T1
	WaitForSingleObject(S1, INFINITE);

	//Копіювання аlpha, beta, МK
	WaitForSingleObject(M, INFINITE);
	int a4 = alpha;
	int b4 = beta;
	Matrix MK4(N);
	MK4.copy(MK);
	ReleaseMutex(M);
	//Знаходження MXh = MOh * MK4
	for (int i = processID*H; i < processID*H + H; i++)
	for (int j = 0; j < MK4.SIZE; j++)
	{
		int sum = 0;
		for (int k = 0; k < MK4.SIZE; k++)
			sum += MO.matrix[i][k] * MK4.matrix[k][j];
		MX.matrix[i][j] = sum;
	}
	//Сигнал потокам Т1, Т2, Т3 про завершення рахування MX
	SetEvent(E4);

	//Очікування завершення рахування MX у потоках T1, T2, T3
	WaitForSingleObject(E1, INFINITE);
	WaitForSingleObject(E2, INFINITE);
	WaitForSingleObject(E3, INFINITE);

	//Знаходження Zh(4) = sort(Bh)
	bubblesort(B.vector + (processID*H), H);

	//Сигнал про завершення сортування Zh(4) потоку Т3
	SetEvent(E6);

	//Очікування завершення сортування у потоці Т1
	WaitForSingleObject(E8, INFINITE);

	//Копіювання C4 = C
	EnterCriticalSection(&CrSec);
	Vector C4(N);
	C4.copy(C);
	LeaveCriticalSection(&CrSec);

	//Знаходження Ah = Zh * alpha4 + beta4 * C4 * MXh
	for (int i = processID*H; i < processID*H + H; i++)
	{
		int res = 0;
		for (int j = 0; j < Z.SIZE; j++)
			res += C4.vector[j] * MX.matrix[j][i];
		A.vector[i] = Z.vector[i] * a4 + b4*res;
	}

	//Сигнал про завершення обчислень потоку T1
	SetEvent(E12);
	printf("t4 finished!!!\n");
}

int main() {
	S1 = CreateSemaphore(NULL, 0, 3, NULL);
	S2 = CreateSemaphore(NULL, 0, 3, NULL);
	M = CreateMutex(NULL, FALSE, NULL);
	InitializeCriticalSection(&CrSec);
	E1 = CreateEvent(NULL, 1, 0, NULL);
	E2 = CreateEvent(NULL, 1, 0, NULL);
	E3 = CreateEvent(NULL, 1, 0, NULL);
	E4 = CreateEvent(NULL, 1, 0, NULL);
	E5 = CreateEvent(NULL, 1, 0, NULL);
	E6 = CreateEvent(NULL, 1, 0, NULL);
	E7 = CreateEvent(NULL, 1, 0, NULL);
	E8 = CreateEvent(NULL, 1, 0, NULL);
	E9 = CreateEvent(NULL, 1, 0, NULL);
	E10 = CreateEvent(NULL, 1, 0, NULL);
	E11 = CreateEvent(NULL, 1, 0, NULL);
	E12 = CreateEvent(NULL, 1, 0, NULL);

	HANDLE T1 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)t1, NULL, 0, NULL);
	HANDLE T2 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)t2, NULL, 0, NULL);
	HANDLE T3 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)t3, NULL, 0, NULL);
	HANDLE T4 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)t4, NULL, 0, NULL);

	WaitForSingleObject(T1, INFINITE);

	CloseHandle(T1);
	CloseHandle(T2);
	CloseHandle(T3);
	CloseHandle(T4);
	CloseHandle(S1);
	CloseHandle(S2);
	CloseHandle(M);
	CloseHandle(E1);
	CloseHandle(E2);
	CloseHandle(E3);
	CloseHandle(E4);
	CloseHandle(E5);
	CloseHandle(E6);
	CloseHandle(E7);
	CloseHandle(E8);
	CloseHandle(E9);
	CloseHandle(E10);
	CloseHandle(E11);
	CloseHandle(E12);
	DeleteCriticalSection(&CrSec);

	return 0;
}

void bubblesort(int* vectorPart, int size)
{
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size - i - 1; j++)
	if (*(vectorPart + j)>*(vectorPart + j + 1))
		std::swap(*(vectorPart + j), *(vectorPart + j + 1));
}

void merge(int* vectorPart1, int* vectorPart2, int size1, int size2, int* dest)
{
	int *last1 = vectorPart1 + size1,
		*last2 = vectorPart2 + size2,
		len = size1 + size2;

	for (int i = 0; i < len; i++)
	{
		if (vectorPart1 == last1){ std::memcpy(dest + i, vectorPart2, (len - i) * 4);	break; }
		if (vectorPart2 == last2){ std::memcpy(dest + i, vectorPart1, (len - i) * 4);	break; }
		dest[i] = *vectorPart1 < *vectorPart2 ?
			*vectorPart1++ : *vectorPart2++;
	}
}
