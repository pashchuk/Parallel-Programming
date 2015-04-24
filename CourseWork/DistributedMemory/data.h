#ifndef __DATA_H__
#define __DATA_H__

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

	void copy(Vector A) {
		for (int i = 0; i < SIZE; i++) {
			vector[i] = A.vector[i];
		}
	}

	Vector sub(int start, int end) {
		Vector sub(end - start);
		int index = 0;
		for (int i = start; i < end; i++) {
			sub.vector[index++] = vector[i];
		}
		return sub;
	}

	void print() {
		if (SIZE < 13) {
			for (int i = 0; i < SIZE; i++) {
				std::printf("%4d  ", vector[i]);
			}
			std::printf("\n");
		}
	}
	~Vector()
	{
		delete[] vector;
	}
};

class Matrix {
public:
	int SIZE;
	int **matrix;
	Matrix(int n) {
		SIZE = n;
		matrix = new int*[n];
		int *tempArr = new int[n*n];
		for (int i = 0; i < n; ++i) {
			matrix[i] = &tempArr[i*n];
		}
	}

	void generate() {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				matrix[i][j] = 1;
			}
		}
	}

	void copy(Matrix MA) {
		for (int i = 0; i < SIZE; i++) {
			for (int j = 0; j < SIZE; j++) {
				matrix[i][j] = MA.matrix[i][j];
			}
		}
	}

	void print() {
		if (SIZE < 13) {
			for (int i = 0; i < SIZE; i++) {
				for (int j = 0; j < SIZE; j++) {
					std::printf("%4d  ", matrix[i][j]);
				}
				std::printf("\n");
			}
			std::printf("\n");
		}
	}
	~Matrix()
	{
		delete[] matrix[0];
		delete matrix;
	}
};

#endif //__DATA_H__