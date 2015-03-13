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
int Vector::getElement(int index){
	return vector[index];
}

void Vector::setElement(int index, int value){
	vector[index] = value;
}

void Vector::print(){
	for (int i = 0; i < size; i++)
		std::printf("%6d  ", vector[i]);
	std::printf("\n");
}

int Vector::operator*(Vector &vec){
	int rezult = 0;
	for (int i = 0; i < size; i++)
		rezult += vector[i]*vec.getElement(i);
	return rezult;
}

Vector* Vector::operator+(Vector &vec){
	Vector *rezult = new Vector(size);
	for (int i = 0; i < size; i++)
		rezult->setElement(i, vector[i] + vec.getElement(i));
	return rezult;
}

Vector* Vector::sort(){
	Vector * rezult = new Vector(size);
	std::copy(vector, (int*)vector + size, rezult->vector);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size - i - 1; j++)
			if (rezult->vector[j]>rezult->vector[j + 1])
				std::swap(rezult->vector[j], rezult->vector[j + 1]);
	return rezult;
}

int Vector::getSize(){
	return size;
}

Vector::~Vector()
{
	delete[] vector;
}
