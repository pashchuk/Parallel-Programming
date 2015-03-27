#ifndef __DATA_H__
#define __DATA_H__

#include <stdlib.h>

class Vector {
public:
	int SIZE;
	int *vector;
	Vector(int n) {
		SIZE = n;
		vector = new int[n];
	}

	void generate() {
		for (int i = 0; i < SIZE; i++) {
			vector[i] = 1;
		}
	}

	void copy(Vector& A) {
		for (int i = 0; i < SIZE; i++) {
			vector[i] = A.vector[i];
		}
	}

	void print() {
		if (SIZE <= 12) {
			for (int i = 0; i < SIZE; i++) {
				std::printf("%6d  ", vector[i]);
			}
			std::printf("\n");
		}
	}
	~Vector()
	{
		delete vector;
	}
};

class Matrix {
public:
	int SIZE;
	int **matrix;
	Matrix(int n) {
		SIZE = n;
		int *data = new int[n*n];
		matrix = new int*[n];
		for (int i = 0; i < n; i++) {
			matrix[i] = &data[i*n];
		}
	}

	void generate() {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				matrix[i][j] = 1;
			}
		}
	}

	void copy(Matrix& MA) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				matrix[i][j] = MA.matrix[i][j];
			}
		}
	}

	void print() {
		if (SIZE <= 12) {
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					std::printf("%6d  ", matrix[i][j]);
				}
				std::printf("\n");
			}
			std::printf("\n");
		}
	}

	~Matrix()
	{
		delete matrix[0];
		delete matrix;
	}
};

#endif // __DATA_H__