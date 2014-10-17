#include <iostream>
#include <omp.h>
#include <stdio.h>
int main()
{
	long start = omp_get_wtime();
	__int64 sum = 0;

	for (int i = 0; i < 100000000; i++)
		sum++;
	long end = omp_get_wtime();
	printf("time = %d\nrezult = %lld\n", end-start,sum);
	std::cin.get();
}

