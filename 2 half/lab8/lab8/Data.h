#ifndef __DATA_H_
#define __DATA_H_

class Vector {
private:
	int size;
public:
	int *vector;
	Vector(int);

	~Vector();

	void generate(int);

	void print();
};

class Matrix {
private:
	const int rows, columns;

public:
	int **matrix;
	Matrix(int size);
	Matrix(int rows, int columns);

	~Matrix();

	void generate(int);

	void print();

};

#endif // __DATA_H_