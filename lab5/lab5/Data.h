
class Vector {
private:
	int size;
	int *vector;
public:
	Vector(int);

	~Vector();

	void generate(int);

	int getElement(int);

	void setElement(int, int);

	int operator*(Vector*);

	void print();

	Vector* operator+(Vector*);

	Vector* sort();

	int getSize();
};

class Matrix {
private:
	int size;
	int **matrix;

public:
	Matrix(int);

	~Matrix();

	void generate(int);

	Matrix* operator*(Matrix*);

	void print();

	Vector* operator*(Vector*);

	Matrix* operator+(Matrix*);

	Matrix* sort();

	Matrix* transpose();

	int getSize();

	int getElement(int line, int column);

	void setElement(int line, int column, int value);
};
