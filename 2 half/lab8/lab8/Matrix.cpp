#include "Data.h"
#include <algorithm>
#include <iostream>

Matrix::Matrix(int rows, int columns): rows(rows), columns(columns)
{
	matrix = new int*[rows];
	int *alloc = new int[rows*columns];
	for (int i = 0; i < rows; i++)
		matrix[i] = &(alloc[columns*i]);
}
Matrix::Matrix(int size) : Matrix(size,size) { }

void Matrix::generate(int filledDigit){
	for (int i = 0; i < rows; i++)
	for (int j = 0; j < columns; j++)
		matrix[i][j] = filledDigit;
}

void Matrix::print(){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++)
			std::printf("%6d  ", matrix[i][j]);
		std::printf("\n");
	}
}

Matrix::~Matrix()
{
	delete[] matrix[0];
	delete[] matrix;
}
