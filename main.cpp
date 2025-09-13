#include <iostream>
#include <format>
#include <random>
#include <stdexcept>

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
  Matrix(int lines, int columns, Args... args) : _lines(lines), _columns(columns) {
    _Matrixptr = new T[_lines * _columns];
    if constexpr(sizeof...(args) == 0) { //constexpr because args work in time to compilation
      for (int i = 0; i < lines * columns; i++) {
        _Matrixptr[i] = T();
      }
    }
    else {
      T temparray[] = { args... };
      size_t args_count = sizeof...(args);
      size_t total_size = lines * columns;

      for (int i = 0; i < total_size; i++) {
        if (i < args_count) {
          _Matrixptr[i] = temparray[i];
        }
        else {
          _Matrixptr[i] = T(); // Заполняем оставшиеся значения
        }
      }
    }
  }

	Matrix(bool flag, int lines, int columns,int inf, int sup) : _lines(lines), _columns(columns) { //only int
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

  int getlines() const {
    return _lines;
  }

  int getcolumns() const {
    return _columns;
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

  Matrix& operator+=(const Matrix rhs) {
      for (int i = 0; i < _lines * _columns; i++) {
        _Matrixptr[i] += rhs._Matrixptr[i];
      }
      return *this;
  }

Matrix operator+(const Matrix& rhs) {
  Matrix result(*this);
  result += rhs;
  return result;
}

Matrix& operator-=(const Matrix rhs) {
  for (int i = 0; i < _lines * _columns; i++) {
    _Matrixptr[i] -= rhs._Matrixptr[i];
  }
  return *this;
}

Matrix operator-(const Matrix& rhs) {
  Matrix result(*this);
  result -= rhs;
  return result;
}

Matrix operator*(T scalar) const {
  Matrix result(_lines, _columns);
  for (int i = 0; i < _lines * _columns; i++) {
    result._Matrixptr[i] = _Matrixptr[i] * scalar;
  }
  return result;
}

friend Matrix operator*(T scalar, const Matrix& rhs) {
  return rhs * scalar; 
}

Matrix operator/(T scalar) const {
  Matrix result(_lines, _columns);
  for (int i = 0; i < _lines * _columns; i++) {
    result._Matrixptr[i] = _Matrixptr[i] / scalar;
  }
  return result;
}

/*Matrix& operator*=(const Matrix& rhs) {
  Matrix result(_lines, rhs._columns);

  for (int i = 0; i < _lines; i++) {
    for (int j = 0; j < rhs._columns; j++) {
      T sum = 0;
      for (int k = 0; k < _columns; k++) {
        sum += _Matrixptr[i * _columns + k] * rhs._Matrixptr[k * rhs._columns + j];
      }
      result._Matrixptr[i * result._columns + j] = sum;
    }
  }

  // Глубокое копирование вместо поверхностного
  delete[] _Matrixptr;
  _Matrixptr = new T[result._lines * result._columns];
  std::copy(result._Matrixptr, result._Matrixptr + result._lines * result._columns, _Matrixptr);

  _lines = result._lines;
  _columns = result._columns;

  return *this;
}*/


	~Matrix() {
		delete[] _Matrixptr;
	}
};


int main() {
	cout << "Helloworld";
	cout << "\nvivod\n";
	Matrix<int>test(true,2,4,1,5);
	Matrix<int>test1(test);
	test1.vivod();
	cout << "3 line 1 column = " << test(1, 1);
	test1(1, 1) = 111;
  test1.vivod();
  Matrix<int>result = test1/2;
	result.vivod();
	return 0;
}
