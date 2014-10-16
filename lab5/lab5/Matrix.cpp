#include "Data.h"
#include <algorithm>
#include <iostream>

Matrix::Matrix(int size){
	size = size;
	matrix = new int*[size];
	for (int i = 0; i < size; ++i)
		matrix[i] = new int[size];
}

void Matrix::generate(int filledDigit){
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			matrix[i][j] = filledDigit;
}

Matrix* Matrix::operator*(Matrix *matr){
	int **rez = new int*[size];
	for (int i = 0; i < size; i++)
		rez[i] = new int[size];
	int tempValue = 0;
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++) {
		tempValue = 0;
		for (int k = 0; k < size; k++)
			tempValue += matrix[j][k] * matr->getElement(k, j);
		rez[i][j] = tempValue;
	}
	std::copy(rez, rez + size*size, matrix);
	for (int i = 0; i < size; i++)
		delete[] rez[i];
	delete[] rez;
	return this;
}

void Matrix::print(){
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++)
			std::printf("%6d  ", matrix[i][j]);
		std::printf("\n");
	}
}

Vector* Matrix::operator*(Vector *vector){
	int *rez = new int[size];
	int tempValue = 0;
	for (int i = 0; i < size; i++){
		tempValue = 0;
		for (int j = 0; j < size; j++)
			tempValue += vector->getElement(i) * matrix[j][i];
		rez[i] = tempValue;
	}
	for (int i = 0; i < size; i++)
		vector->setElement(i, rez[i]);
	delete[] rez;
	return vector;
}

Matrix* Matrix::operator+(Matrix *matr){
	int **rezult = new int*[size];
	for (int i = 0; i < size; i++)
		rezult[i] = new int[size];
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
		rezult[i][j] = matrix[i][j] + matr->getElement(i, j);
	std::copy(rezult, rezult + size*size, matrix);
	for (int i = 0; i < size; i++)
		delete[] rezult[i];
	delete[] rezult;
	return this;
}

Matrix* Matrix::sort(){
	for (int i = 0; i < size; i++){
		qsort(matrix[i], size, sizeof(int), [](const void *a, const void *b){return *((int*)b) - *((int*)a); });
		return this;
	}
}

Matrix* Matrix::transpose(){
	int **rez = new int*[size];
	for (int i = 0; i < size; i++)
		rez[i] = new int[size];
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
		rez[i][j] = matrix[j][i];
	std::copy(rez, rez + size*size, matrix);
	for (int i = 0; i < size; i++)
		delete[] rez[i];
	delete[] rez;
	return this;
}

int Matrix::getSize(){
	return size;
}

int Matrix::getElement(int line, int column){
	return matrix[line][column];
}
void Matrix::setElement(int line, int column, int value){
	matrix[line][column] = value;
}