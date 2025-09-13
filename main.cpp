#include <iostream>
#include <format>
#include <random>

using namespace std;

template <typename T>

class Matrix {
private:
	T* _Matrixptr;
	int _lines;
	int _columns;
public:
	Matrix() :_Matrixptr(nullptr), _lines(0), _columns(0) {}

	template<typename... Args>
	Matrix(int lines, int columns,Args... args) : _lines(lines), _columns(columns) {
		cout << "standart";
		_Matrixptr = new T[_lines * _columns];
		T temparray[] = { args... };
		for (int i = 0; i < lines * columns; i++) {
			_Matrixptr[i] = temparray[i];
		}
	}

	Matrix(bool flag, int lines, int columns,int inf, int sup) : _lines(lines), _columns(columns) { //только для целых цисел
		_Matrixptr = new T[lines * columns];
		default_random_engine e;
		uniform_int_distribution d(inf, sup);
		for (int i = 0; i < lines * columns; i++) {
			_Matrixptr[i] = d(e);
		}
	}

	Matrix(const Matrix& other)
		: _lines(other._lines), _columns(other._columns) {
		if (other._Matrixptr == nullptr) {
			_Matrixptr = nullptr;
			return;
		}
		_Matrixptr = new T[_lines * _columns];
		for (int i = 0; i < _lines * _columns; i++) {
			_Matrixptr[i] = other._Matrixptr[i];
		}
	}

	T& operator()(int line, int column) {
		return _Matrixptr[line * _columns + column];
	}


	void vivod() const {
		cout << "\n";
		int temp = 0;
		for (int i = 0; i < _lines * _columns; i++) {
			cout << format("{} ", _Matrixptr[i]);
			temp++;
			if (temp >= _columns) {
				cout << format("\n");
				temp = 0;
			}
		}
	}

	~Matrix() {
		delete[] _Matrixptr;
	}
};

int main() {
	cout << "Helloworld";
	cout << "\nvivod\n";
	Matrix<int>test(true,2,4,12,24);
	Matrix<int>test1(test);
	test1.vivod();
	cout << "число 3 сторка 1 столбец:" << test(1, 1);
	test1(1, 1) = 111;
	test1.vivod();
	return 0;
}