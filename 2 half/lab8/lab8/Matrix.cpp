#include "Data.h"
#include <algorithm>
#include <iostream>

Matrix::Matrix(int sizes){
	size = sizes;
	matrix = new int*[size];
	int *alloc = new int[sizes*sizes];
	for (int i = 0; i < sizes; i++)
		matrix[i] = &(alloc[sizes*i]);
}

void Matrix::generate(int filledDigit){
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
		matrix[i][j] = filledDigit;
}

Matrix* Matrix::operator*(Matrix &matr){
	Matrix *rez = new Matrix(size);
	int tempValue = 0;
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++) {
		tempValue = 0;
		for (int k = 0; k < size; k++)
			tempValue += matrix[j][k] * matr.getElement(k, j);
		rez->setElement(i, j, tempValue);
	}
	return rez;
}

void Matrix::print(){
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++)
			std::printf("%6d  ", matrix[i][j]);
		std::printf("\n");
	}
}

Matrix* Matrix::operator+(Matrix &matr){
	Matrix *rezult = new Matrix(size);
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
		rezult->setElement(i, j, matrix[i][j] + matr.getElement(i, j));
	return rezult;
}

int compare(const void *a, const void *b)
{
	return *((int*)(b)) - *((int*)a);
}

Matrix* Matrix::sort(){
	Matrix *rezult = new Matrix(size);
	std::copy(matrix, static_cast<int**>(matrix) + size, rezult->matrix);
	for (int i = 0; i < size; i++)
		qsort(rezult->matrix[i], size, sizeof(int), &compare);
	return rezult;
}

Matrix* Matrix::transpose(){
	Matrix *rezult = new Matrix(size);
	for (int i = 0; i < size; i++)
	for (int j = 0; j < size; j++)
		rezult->setElement(i, j, matrix[j][i]);
	return rezult;
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

Matrix::~Matrix()
{
	delete[] matrix[0];
	delete[] matrix;
}

Vector* operator*(Matrix &matrix, Vector &vector){
	Vector *rez = new Vector(vector.getSize());
	int tempValue = 0;
	for (int i = 0; i < vector.getSize(); i++){
		tempValue = 0;
		for (int j = 0; j < vector.getSize(); j++)
			tempValue += vector.getElement(i) * matrix.getElement(j, i);
		rez->setElement(i, tempValue);
	}
	return rez;
}

Vector* operator*(Vector &vector, Matrix &matrix){
	Vector *rez = new Vector(vector.getSize());
	int tempValue = 0;
	for (int i = 0; i < vector.getSize(); i++){
		tempValue = 0;
		for (int j = 0; j < vector.getSize(); j++)
			tempValue += vector.getElement(i) * matrix.getElement(j, i);
		rez->setElement(i, tempValue);
	}
	return rez;
}