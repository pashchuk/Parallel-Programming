/*
---------------------------------------------------------------------------------
--              Laboratory Work #5
--        Lab5.Threads in WinAPI library
--      Name      Pashchuk Eduard Fedorovich
--      Group     IP-22
--      Date      16.10.2014
--      F1        d = (B*C) + (A*B) +(C*(B*(MA*MZ)))
--      F2        MC = SORT(TRANS(MA)*MB)
--      F3        D = SORT(A + M)*TRANS(MC*MM)
---------------------------------------------------------------------------------
*/


#include <windows.h>
#include <iostream>
#include "Data.h"

#define SIZE 10
#define FILL_NUMBER 1

void task1();
void task2();
void task3();

using namespace std;

int main(int argc, TCHAR* argv[])
{
	DWORD TidA, TidB, TidC;
	HANDLE T1 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)task1, NULL, 0, &TidA);
	SetThreadPriority(T1, 1);
	HANDLE T2 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)task2, NULL, 0, &TidB);
	SetThreadPriority(T2, 2);
	HANDLE T3 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)task3, NULL, 0, &TidC);
	SetThreadPriority(T3, 3);
	WaitForSingleObject(T1, INFINITE);
	WaitForSingleObject(T2, INFINITE);
	WaitForSingleObject(T3, INFINITE);
	CloseHandle(T1);
	CloseHandle(T2);
	CloseHandle(T3);
	cout << "Main thread close" << endl;
	cin.get();
	return 0;

}

void task1() {
	printf("Task1 started\n");
	Vector *A = new Vector(SIZE),
		*B = new Vector(SIZE),
		*C = new Vector(SIZE);
	Matrix *MA = new Matrix(SIZE),
		*MZ = new Matrix(SIZE);
	printf("generating vector A ...\n");
	A->generate(FILL_NUMBER);
	printf("generating vector B ...\n");
	B->generate(FILL_NUMBER);
	printf("generating vector C ...\n");
	C->generate(FILL_NUMBER);
	printf("generating matrix MA ...\n");
	MA->generate(FILL_NUMBER);
	printf("generating matrix MZ ...\n");
	MZ->generate(FILL_NUMBER);
	Sleep(3000);
	printf("Calculating F1 ...\n");
	int d = (*B * *C) + (*A * *B) + (*C * *(*B * *(*MA * *MZ)));
	if (SIZE<=10)
		printf("d = %d\n", d);
	delete A, B, C, MA, MZ;
	printf("Task1 finished\n");
}


void task2() {
	printf("Task2 started\n");
	Matrix *MA = new Matrix(SIZE),
		*MB = new Matrix(SIZE);
	printf("generating matrix MA ...\n");
	MA->generate(FILL_NUMBER);
	printf("generating matrix MB ...\n");
	MB->generate(FILL_NUMBER);
	Sleep(1500);
	printf("Calculating F2 ...\n");
	Matrix *MC = (* MA->transpose() * *MB)->sort();
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
	Vector *A = new Vector(SIZE),
		*M = new Vector(SIZE);
	Matrix *MC = new Matrix(SIZE),
		*MM = new Matrix(SIZE);
	printf("generating vector A ...\n");
	A->generate(FILL_NUMBER);
	printf("generating vector M ...\n");
	M->generate(FILL_NUMBER);
	printf("generating matrix MC ...\n");
	MC->generate(FILL_NUMBER);
	printf("generating matrix MM ...\n");
	MM->generate(FILL_NUMBER);
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