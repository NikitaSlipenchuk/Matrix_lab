#include <iostream>
#include <format>
#include <random>
#include <stdexcept>
#include <complex>

using namespace std;

template <typename T>

class Matrix {
private:
  T* _Matrixptr;
  size_t _lines;
  size_t _columns;
public:
  Matrix() :_Matrixptr(nullptr), _lines(0), _columns(0) {}

  Matrix(size_t lines, size_t columns, T value) : _lines(lines), _columns(columns) {
    _Matrixptr = new T[_lines * _columns];
    for (size_t i = 0; i < lines * columns; i++) {
      _Matrixptr[i] = value;
    }
  }

	Matrix(size_t lines, size_t columns,int inf, int sup) : _lines(lines), _columns(columns) { //for  int
		_Matrixptr = new T[lines * columns];
    std::random_device engine;
    std::uniform_int_distribution distribution(std::min(inf, sup), std::max(inf, sup));
    for (size_t i = 0; i < columns*lines; i++)
    {
      _Matrixptr[i] = distribution(engine);
    }
	}

  Matrix(size_t lines, size_t columns, float inf, float sup) : _lines(lines), _columns(columns) { //for  float
    _Matrixptr = new T[lines * columns];
    std::random_device engine;
    std::uniform_real_distribution distribution(std::min(inf, sup), std::max(inf, sup));
    for (size_t i = 0; i < columns * lines; i++)
    {
      _Matrixptr[i] = distribution(engine);
    }
  }

  Matrix(size_t lines, size_t columns, double inf, double sup) : _lines(lines), _columns(columns) { //for  float
    _Matrixptr = new T[lines * columns];
    std::random_device engine;
    std::uniform_real_distribution distribution(std::min(inf, sup), std::max(inf, sup));
    for (size_t i = 0; i < columns * lines; i++)
    {
      _Matrixptr[i] = distribution(engine);
    }
  }

  Matrix(size_t lines, size_t columns, complex<float> inf, complex<float> sup) : _lines(lines), _columns(columns) { //for  complex<float>
    _Matrixptr = new T[lines*columns];
    std::random_device engine;
    std::uniform_real_distribution distributionre(std::min(sup.real(), inf.real()), std::max(sup.real(), inf.real()));
    std::uniform_real_distribution distributionim(std::min(sup.imag(), inf.imag()), std::max(sup.imag(), inf.imag()));
    for (size_t i = 0; i < columns*lines; i++)
    {
      _Matrixptr[i] = std::complex<float>(distributionre(engine), distributionim(engine));
    }
  }


  Matrix(size_t lines, size_t columns, complex<double> inf, complex<double> sup) : _lines(lines), _columns(columns) { //for  complex<double>
    _Matrixptr = new T[lines * columns];
    std::random_device engine;
    std::uniform_real_distribution distributionre(std::min(sup.real(), inf.real()), std::max(sup.real(), inf.real()));
    std::uniform_real_distribution distributionim(std::min(sup.imag(), inf.imag()), std::max(sup.imag(), inf.imag()));
    for (size_t i = 0; i < columns * lines; i++)
    {
      _Matrixptr[i] = std::complex<double>(distributionre(engine), distributionim(engine));
    }
  }

	Matrix(const Matrix& other)
		: _lines(other._lines), _columns(other._columns) {
		if (other._Matrixptr == nullptr) {
			_Matrixptr = nullptr;
			return;
		}
		_Matrixptr = new T[_lines * _columns];
		for (size_t i = 0; i < _lines * _columns; i++) {
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
    size_t temp = 0;
		for (size_t i = 0; i < _lines * _columns; i++) {
      cout << _Matrixptr[i] << " ";
			temp++;
			if (temp >= _columns) {
				cout << format("\n");
				temp = 0;
			}
		}
	}

  Matrix& operator+=(const Matrix rhs) {
      for (size_t i = 0; i < _lines * _columns; i++) {
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
  for (size_t i = 0; i < _lines * _columns; i++) {
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
  for (size_t i = 0; i < _lines * _columns; i++) {
    result._Matrixptr[i] = _Matrixptr[i] * scalar;
  }
  return result;
}

friend Matrix operator*(T scalar, const Matrix& rhs) {
  return rhs * scalar; 
}

/*
Matrix operator/(T scalar) const {
  Matrix result(_lines, _columns);
  for (int i = 0; i < _lines * _columns; i++) {
    result._Matrixptr[i] = _Matrixptr[i] / scalar;
  }
  return result;
}
*/

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
  Matrix<complex<double>>test(2, 4, complex<double>(2.3,15), complex<double>(4.8, 2.3));
	Matrix<complex<double>>test1(test);
	test1.vivod();
	cout <<endl<< "2 line 2 column = " << test(1, 1) <<endl;
  Matrix<complex<double>>test2;
  test2 = test + test1;
	test1(1, 1) = 111;
  test2.vivod();
  //Matrix<int>result = test1/2;
//	result.vivod();
	return 0;
}
