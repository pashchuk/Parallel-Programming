#include "Data.h"
#include <iostream>

Vector::Vector(int sizes){
	size = sizes;
	vector = new int[size];
}

void Vector::generate(int filledDigit){
	for (int i = 0; i < size; i++)
		vector[i] = filledDigit;
}

void Vector::print(){
	for (int i = 0; i < size; i++)
		std::printf("%6d  ", vector[i]);
	std::printf("\n");
}

Vector::~Vector()
{
	delete[] vector;
}