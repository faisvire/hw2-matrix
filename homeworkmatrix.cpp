#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept> 
using namespace std;
template <class T>
class Matrix {
protected:
	int cols;
	int rows;
	vector<vector<T>> values;
public:
	Matrix(int str, int stl) {
		cols = stl;
		rows = str;
		values.resize(rows);
		for (int i = 0; i < rows; ++i) {
			values[i].resize(cols);
		}
	}
	Matrix(const Matrix& matrix) {
		cols = matrix.cols;
		rows = matrix.rows;
		values = matrix.values;
	}
	Matrix() {
		cols = 0;
		rows = 0;
	}
	void inputfile(const char* name, Matrix& matrix) {
		ifstream input(name);
		input >> matrix.rows >> matrix.cols;
		matrix.values.resize(matrix.rows);
		for (int i = 0; i < matrix.rows; ++i) {
			matrix.values[i].resize(matrix.cols);
		}
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				T value;
				input >> value;
				matrix.values[i][j] = value;
			}
		}
		input.close();
	}
	void outputfile(const char* name, Matrix& matrix) {
		ofstream output;
		output.open(name);
		output << "Matrix: " << endl;
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				output << matrix.values[i][j] << " ";
			}
			output << endl;
		}
		output.close();
		cout << "Output to file was successful";
	}
	friend void operator >> (istream& cin, Matrix& matrix) {
		if (matrix.rows == 0 || matrix.cols == 0) {
			cout << "Input number of columns: ";
			cin >> matrix.cols;
			cout << "Input number of rows: ";
			cin >> matrix.rows;
			matrix.values.resize(matrix.rows);
			for (int i = 0; i < matrix.rows; ++i) {
				matrix.values[i].resize(matrix.cols);
			}
		}
		cout << "Input " << matrix.rows * matrix.cols << " elements of matrix: ";
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				cin >> matrix.values[i][j];
			}
		}
	}
	friend void operator << (ostream& cout, Matrix& matrix) {
		cout << "Matrix: " << endl;
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				cout << "  " << matrix.values[i][j] << "  ";
			}
			cout << endl;
		}
		cout << endl;
	}
	friend bool operator == (Matrix& matrix, double scalar) {
		if (matrix.cols != matrix.rows) return false; else {
			for (int i = 0; i < matrix.rows; ++i) {
				for (int j = 0; j < matrix.cols; ++j) {
					if ((i == j && matrix.values[i][j] == scalar) || (i != j && matrix.values[i][j] == 0));
					else {
						return false;
					}
				}
			}
			return true;
		}
	}
	friend bool operator != (Matrix& matrix, double scalar) {
		if (matrix == scalar) return false; else return true;
	}
	friend bool operator == (Matrix& matrix1, Matrix& matrix2) {
		if (matrix1.cols != matrix2.cols || matrix1.rows != matrix2.rows) {
			return false;
		}
		else if (matrix1.values == matrix2.values) return true;
		else return false;
	}
	friend bool operator != (Matrix& matrix1, Matrix& matrix2) {
		if (matrix1 == matrix2) return false; else return true;
	}
	friend Matrix operator + (Matrix& matrix1, Matrix& matrix2) {
		if (matrix1.cols != matrix2.cols || matrix1.rows != matrix2.rows) throw "Matrixes can not be summed";
		else {
			Matrix matrix3(matrix1.rows, matrix1.cols);
			for (int i = 0; i < matrix3.rows; ++i) {
				for (int j = 0; j < matrix3.cols; ++j) {
					matrix3.values[i][j] = (matrix1.values[i][j] + matrix2.values[i][j]);
				}
			}
			return matrix3;
		}
	}
	friend Matrix operator - (Matrix& matrix1, Matrix& matrix2) {
		if (matrix1.cols != matrix2.cols || matrix1.rows != matrix2.rows) throw "Matrixes can not be subtracted";
		else {
			Matrix matrix3(matrix1.rows, matrix1.cols);
			for (int i = 0; i < matrix3.rows; ++i) {
				for (int j = 0; j < matrix3.cols; ++j) {
					matrix3.values[i][j] = (matrix1.values[i][j] - matrix2.values[i][j]);
				}
			}
			return matrix3;
		}
	}
	Matrix operator = (Matrix& matrix1) {
		cols = matrix1.cols;
		rows = matrix1.rows;
		values = matrix1.values;
		return *this;
	}
	friend Matrix operator * (Matrix& matrix, double scalar) {
		Matrix matrix2(matrix.rows, matrix.cols);
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				matrix2.values[i][j] = matrix.values[i][j] * scalar;
			}
		}
		return matrix2;
	}
	friend Matrix operator * (Matrix& matrix1, Matrix& matrix2) {
		int newcols, newrows;
		if ((matrix1.cols == matrix2.cols && matrix1.rows == matrix2.rows && matrix1.cols == matrix1.rows) || (matrix1.cols == matrix2.rows)) {
			if (matrix1.cols == matrix2.cols && matrix1.rows == matrix2.rows && matrix1.cols == matrix1.rows) {
				newcols = matrix1.cols;
				newrows = matrix1.rows;
			}
			else if (matrix1.cols == matrix2.rows) {
				newcols = matrix2.cols;
				newrows = matrix1.rows;
			}
			Matrix matrix3(newrows, newcols);
			for (int i = 0; i < newrows; ++i) {
				for (int j = 0; j < newcols; ++j) {
					matrix3.values[i][j] = 0;
					for (int m = 0; m < matrix2.rows; ++m) {
						matrix3.values[i][j] += matrix1.values[i][m] * matrix2.values[m][j];
					}
				}
			}
			return matrix3;
		}
	else cerr << "Matrixes cannot be multiplied";
	}
	friend T determinant (Matrix& matrix) {
		T det = 0;
		if (matrix.cols != matrix.rows) throw runtime_error("Determinant cannot be found");
		else {
			int rowscurr = matrix.rows;
			if (rowscurr == 1) return matrix.values[0][0];
			else if (rowscurr == 2) {
				det = matrix.values[0][0] * matrix.values[1][1] - matrix.values[0][1] * matrix.values[1][0];
				return det;
			}
			else {
				det = 0;
				for (int i = 0; i < rowscurr; ++i) {
					Matrix newmatrix((rowscurr - 1), (rowscurr - 1));
					int schtk = 0;
					int schtm = 0;
					for (int k = 0; k < rowscurr; ++k) {
						for (int m = 0; m < rowscurr; ++m) {
							if (k != 0 && m != i) {
								newmatrix.values[schtk][schtm] = matrix.values[k][m];
								schtm += 1;
								if (schtm == rowscurr - 1) {
									schtm = 0;
									schtk += 1;
								}
							}
						}
					}
					det += determinant(newmatrix) * matrix.values[0][i] * pow((-1.0), i);
				}
			}
			return det;
		}
	}
	friend Matrix algdop(Matrix matrix) {
		Matrix rezmatr(matrix.rows, matrix.cols);
		for (int i = 0; i < matrix.rows; ++i) {
			for (int j = 0; j < matrix.cols; ++j) {
				Matrix prommatr((matrix.cols)-1, (matrix.rows)-1);
				int schtk = 0;
				int schtm = 0;
				for (int k = 0; k < matrix.rows; ++k) {
					for (int m = 0; m < matrix.rows; ++m) {
						if (k != i && m != j) {
							prommatr.values[schtk][schtm] = matrix.values[k][m];
							schtm += 1;
							if (schtm == matrix.rows - 1) {
								schtm = 0;
								schtk += 1;
							}
						}
					}
				}
				rezmatr.values[i][j] = determinant(prommatr) * pow(-1, (i + j));
			}
		}
		return rezmatr;
	}
	friend Matrix transpmatrix(Matrix matrix) {
		Matrix matr2(matrix.cols, matrix.rows);
		for (int i = 0; i < matrix.cols; ++i) {
			for (int j = 0; j < matrix.rows; ++j) {
				matr2.values[i][j] = matrix.values[j][i];
			}
		}
		return matr2;
	}
	friend Matrix operator! (Matrix& matrix) {
		if (matrix.rows != matrix.cols) throw "Inverse matrix does not exist";
		else if (determinant(matrix) == 0) throw "Inverse matrix does not exist";
		else {
			double obrdet = 1 / determinant(matrix);
			Matrix obratn(matrix.rows, matrix.cols);
			for (int i = 0; i < matrix.rows; ++i) {
				for (int j = 0; j < matrix.cols; ++j) {
					obratn.values[i][j] = (transpmatrix(algdop(matrix))).values[i][j] / determinant(matrix) ;
				}
			}
			return obratn;
		}
	}
	static Matrix zero(int rows, int cols) {
		Matrix matrix(rows, cols);
		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				matrix.values[i][j] = 0;
			}
		}
		return matrix;
	}
	static Matrix identity(int size) {
		Matrix matrix(size, size);
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				matrix.values[i][j] = (i == j) ? 1 : 0; 
			}
		}
		return matrix;
	}
	~Matrix() { }
};
int main() {
	Matrix<double> matrix1(2,2);
	cin >> matrix1;
	cout << matrix1;
	try {
		Matrix<double> matrix3(!matrix1);
		cout << matrix3;
	}
	catch(...) {
		cerr << "Inverse matrix does not exist" << endl;
	}
	Matrix<int> zeromatrix = Matrix<int>::zero(2,2);
	cout << zeromatrix;
	Matrix<int> identitymatrix = Matrix<int>::identity(3);
	cout << identitymatrix;
}