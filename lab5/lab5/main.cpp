/*
---------------------------------------------------------------------------------
--              Laboratory Work #5
--        Lab5.Threads in C# programming language
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
#include "Matrix.h"
#include "Vector.h"

void Task1();
void Task2();
void Task3();

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD TidA, TidB, TidC;
	HANDLE T1 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)Task1, NULL, 0, &TidA);
	SetThreadPriority(T1, 1);
	HANDLE T2 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)Task2, NULL, 0, &TidB);
	SetThreadPriority(T2, 2);
	HANDLE T3 = CreateThread(NULL, 10000, (LPTHREAD_START_ROUTINE)Task3, NULL, 0, &TidC);
	SetThreadPriority(T3, 3);
	WaitForSingleObject(T1, INFINITE);
	WaitForSingleObject(T2, INFINITE);
	WaitForSingleObject(T3, INFINITE);
	CloseHandle(T1);
	CloseHandle(T2);
	CloseHandle(T3);
	cout << "Main thread close" << endl;
	system("pause");
	return 0;

}

void Task1(){
	Vector  A, B, Func1_Result;
	int** MA, **MZ;
	Data data;
	cout << ("Task1 started") << endl;
	A = data.Get_Vector();
	B = data.Get_Vector();
	MA = data.Get_Matrix();
	MZ = data.Get_Matrix();
	Sleep(100);
	Func1_Result = data.Func1(A, B, MA, MZ);
	cout << ("Func1 Result:") << endl;
	data.Print_Vector(Func1_Result);
	cout << endl << ("Task 1 finished") << endl;
	for (int i = 0; i < data.getSize(); ++i)
		delete[] MZ[i];
	delete[] MZ, Func1_Result, A, B;
}

void Task2(){
	int** MA, **MB, **MC, **Func2_Result;
	cout << ("Task2 started") << endl;
	Data data;
	MA = data.Get_Matrix();
	MB = data.Get_Matrix();
	MC = data.Get_Matrix();
	Sleep(3000);
	Func2_Result = data.Func2(MA, MB, MC);
	cout << endl << ("Func2 Result:") << endl;
	data.Print_Matrix(Func2_Result);
	cout << ("Task2 finished") << endl;
	for (int i = 0; i < data.getSize(); ++i)
		delete[] MA[i], MB[i], MC[i], Func2_Result[i];
	delete[] MA, MB, MC, Func2_Result;
}

void Task3() {
	// TODO Auto-generated method stub
	int** MB, **MC, **MM, **Func3_Result;
	cout << ("Task3 started") << endl;
	Data data;
	MB = data.Get_Matrix();
	MC = data.Get_Matrix();
	MM = data.Get_Matrix();
	Sleep(5000);
	Func3_Result = data.Func3(MB, MC, MM);
	cout << ("Func3 Result:") << endl;
	data.Print_Vector(Func3_Result);
	cout << endl << ("Task3 finished") << endl;
	for (int i = 0; i < data.getSize(); ++i)
		delete[] MM[i], MB[i], MC[i], Func3_Result[i];
	delete[] MM, MB, MC, Func3_Result;
}