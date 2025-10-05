#ifndef FUNCTION_H
#define FUNCTION_H

#include "matrix.h"

template<typename T>
Matrix<T> luDecompositionL(const Matrix<T>& A) {
  if (A.getlines() != A.getcolumns()) {
    throw std::invalid_argument("LU decomposition requires square matrix");
  }

  size_t n = A.getlines();
  Matrix<T> L(n, n, T(0));
  Matrix<T> U(n, n, T(0));

  for (size_t i = 0; i < n; i++) {
    // High triangle U (line i)
    for (size_t j = i; j < n; j++) {
      T sum = T(0);
      for (size_t k = 0; k < i; k++) {
        sum += L(i, k) * U(k, j);
      }
      U(i, j) = A(i, j) - sum;
    }

    // low triangle L (column i)
    for (size_t j = i; j < n; j++) {
      if (i == j) {
        L(i, i) = T(1);
      }
      else {
        T sum = T(0);
        for (size_t k = 0; k < i; k++) {
          sum += L(j, k) * U(k, i);
        }
        if (U(i, i) == T(0)) {
          throw std::runtime_error("Matrix is singular");
        }
        L(j, i) = (A(j, i) - sum) / U(i, i);
      }
    }
  }
  return L;
}

template<typename T>
Matrix<T> luDecompositionR(const Matrix<T>& A) {
  if (A.getlines() != A.getcolumns()) {
    throw std::invalid_argument("LU decomposition requires square matrix");
  }

  size_t n = A.getlines();
  Matrix<T> L(n, n, T(0));
  Matrix<T> U(n, n, T(0));

  for (size_t i = 0; i < n; i++) {
    // high triangle U (line i)
    for (size_t j = i; j < n; j++) {
      T sum = T(0);
      for (size_t k = 0; k < i; k++) {
        sum += L(i, k) * U(k, j);
      }
      U(i, j) = A(i, j) - sum;
    }

    // low triangle L (column i)
    for (size_t j = i; j < n; j++) {
      if (i == j) {
        L(i, i) = T(1);
      }
      else {
        T sum = T(0);
        for (size_t k = 0; k < i; k++) {
          sum += L(j, k) * U(k, i);
        }
        if (U(i, i) == T(0)) {
          throw std::runtime_error("Matrix is singular");
        }
        L(j, i) = (A(j, i) - sum) / U(i, i);
      }
    }
  }
  return U;
}

void example() {
  Matrix<double>A(3, 4, 13.2, 67.5), B(3, 4, 5.1, 14.4);
  cout << "A=" << A << "B=" << B;
  cout << "A+B=" << A + B;
  cout << "A-B=" << A - B;
  cout << "A*5=" << A * 5;
  cout << "A/3" << A / 3;
  Matrix<double>C(4, 3, 1.4, 7.8);
  cout << "C=" << C;
  cout << "A*C=" << A * C;
  cout << "trace of C=" << C.trace() << "\n";
  cout << "element 1,1 =" << C(1, 1) << "\n";
  Matrix<double>F(3, 3, 1.4, 10.8);
  cout << "F=" << F;
  cout << "In my task L=" << luDecompositionL(F);
  cout << "R=" << luDecompositionR(F);
  cout << "Input=" << luDecompositionL(F) * luDecompositionR(F);
}

#endif
