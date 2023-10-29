#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif NOMINMAX
#include <Windows.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <cassert>
#include <optional>
#include <format>

#include "Vector.h"
#include "String.h"

namespace DataContainers {
	template <typename T, typename = enable_if_t<is_arithmetic_v<T>>>
	class Matrix {
	private:
		uint32_t rows_;
		uint32_t columns_;
		uint32_t currRow_;
		uint32_t currCol_;
		T** data_;

		static uint32_t maxSize(T** init, uint32_t row, uint32_t col) {
			uint32_t result= 0;
			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++)
					if (to_string(init[i][j]).size() > result)
						result= to_string(init[i][j]).size();
			return result;
		}
	public:
		~Matrix() {
			for (size_t i = 0; i < rows_; i++) {
				delete[] data_[i];
				data_[i] = nullptr;
			}
			delete data_;
			data_ = nullptr;
		}
		Matrix(uint32_t row, uint32_t col) {
			data_ = new T*[row];
			for (size_t i = 0; i < row; i++)
				data_[i] = new T[col];

			this->rows_ = row;
			this->columns_ = col;
			this->currRow_ = 0;
			this->currCol_ = -1;
		}
		Matrix(const Matrix<T>& elem) : Matrix(elem.rows_, elem.columns_) {
			assert(elem.height() == rows_ && elem.width() == columns_);
			for (size_t i = 0; i < rows_; i++)
				for (size_t j = 0; j < columns_; j++)
					data_[i][j] = elem.data_[i][j];
		}
		Matrix(T** data, uint32_t row, uint32_t col) {
			this->data_ = new T * [row];
			for (size_t i = 0; i < row; i++)
				this->data_[i] = new T[col];

			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++)
					this->data_[i][j] = data[i][j];

			this->rows_ = row;
			this->columns_ = col;
			this->currCol_ = row - 1;
			this->currRow_ = col - 1;
		}
		Matrix(T* data, uint32_t row, uint32_t col) : Matrix(row, col) {
			uint32_t tmp = 0;
			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++) {
					this->data_[i][j] = data[tmp++];
					currCol_ = j;
					currRow_ = i;
				}
		}

		void append(const T elem) {
			if (currCol_ + 1 <= columns_ - 1) {
				currCol_ += 1;
				data_[currRow_][currCol_] = elem;
			}
			else {
				if (currRow_ + 1 <= rows_ - 1) {
					data_[currRow_ + 1][0] = elem;
					currRow_ += 1;
					currCol_ = 0;
				}
				else
					throw "Matrix is full";
			}
		}
		static void initialize(T**& init, uint32_t row, uint32_t col) {
			init = new T * [row];
			for (size_t i = 0; i < row; i++)
				init[i] = new T[col];
		}
		static void Delete(T**& init, uint32_t row) {
			for (size_t i = 0; i < row; i++) {
				delete[] init[i];
				init[i] = nullptr;
			}
			init = nullptr;
		}
		T* getColumn(uint32_t col) {
			T* result = new T[columns_];
			for (size_t i = 0; i < rows_; i++)
				result[i] = data_[i][col];
			return result;

		}
		T* getRow(uint32_t row) {
			T* result = new T[rows_];
			for (size_t i = 0; i < columns_; i++)
				result[i] = data_[row][i];
			return result;
		}

		friend ostream& operator<<(ostream& out, Matrix<T> p) {
			int maxSize = maxSize(p.data_, p.rows_, p.columns_);

			for (size_t i = 0; i < p.rows_; i++) {
				for (size_t j = 0; j < p.columns_; j++)
					out << setw(maxSize + 2) << p.data_[i][j];
				out << endl;
			}
			return out;
		}
		friend istream& operator>>(istream& in, Matrix<T>& p) {
			for (size_t i = 0; i < p.rows_; i++) {
				COORD coord;
				coord.Y = i + 1;
				for (size_t j = 0; j < p.columns_; j++) {
					if (j == 0)
						coord.X = j + 1;
					bool flag;
					do {
						int tmp;
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

						in >> tmp;
						p.data_[i][j] = tmp;

						const int elemSize = to_string(tmp).size();
						if (in.fail()) {
							in.clear();
							in.ignore(32767, '\n');
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
							for (size_t i = 0; i < 100; i++) cout << " ";
							flag = true;
						}
						else {
							flag = false;
							coord.X += elemSize + 2;
						}
					} while (flag);
				}
			}
			return in;

		}
		friend Matrix operator+(const Matrix<T>& first, const Matrix<T>& second) {
			assert(first.columns_ == second.rows_);

			Matrix<T> tmp(first.rows_, second.columns_);

			for (size_t i = 0; i < tmp.rows_; i++) {
				for (size_t j = 0; j < tmp.columns_; j++) {
					tmp.data_[i][j] = 0;
					for (size_t k = 0; k < first.columns_; k++) {

						if (first.data_[i][k] != 0 || second.data_[k][j] != 0)
							tmp.data_[i][j] += (first.data_[i][k] + second.data_[k][j]);

					}
				}
			}
			return tmp;

		}
		friend Matrix operator-(const Matrix<T>& first, const Matrix<T>& second) {
			assert(first.columns_ == second.columns_ && first.rows_ == second.rows_ && "Matrix sizes do not match");

			Matrix<T> result(first.rows_, first.columns_);

			for (size_t i = 0; i < result.rows_; i++)
				for (size_t j = 0; j < result.columns_; j++)
					result.data_[i][j] = first.data_[i][j] - second.data_[i][j];

			return result;
		}
		friend Matrix operator*(const Matrix<T>& first, const Matrix<T>& second) {
			assert(first.columns_ == second.rows_);

			Matrix<T> tmp(first.rows_, second.columns_);

			for (int i = 0; i < tmp.rows_; ++i)
				for (int j = 0; j < tmp.columns_; ++j)
					tmp.data_[i][j] = 0;

			for (int i = 0; i < tmp.rows_; ++i)
				for (int j = 0; j < tmp.columns_; ++j)
					for (int k = 0; k < first.columns_; ++k)
						tmp.data_[i][j] += first.data_[i][k] * second.data_[k][j];

			return tmp;
		}
		friend Matrix operator*(const T num, const Matrix<T>& second) {
			Matrix<T> tmp(second.rows_, second.columns_);

			for (size_t i = 0; i < tmp.rows_; i++) {
				for (size_t j = 0; j < tmp.columns_; j++) {
					for (size_t k = 0; k < second.columns_; k++)
						tmp.data_[i][j] = second.data_[i][j] * num;
				}
			}
			return tmp;
		}
		friend Matrix operator*(Matrix<T>& second, T num) {
			Matrix<T> tmp(second.rows_, second.columns_);

			for (size_t i = 0; i < tmp.rows_; i++) {
				for (size_t j = 0; j < tmp.columns_; j++) {
					for (size_t k = 0; k < second.columns_; k++)
						tmp.data_[i][j] = second.data_[i][j] * num;
				}
			}
			return tmp;
		}
		friend Matrix operator/(const Matrix<T>& first, const Matrix<T>& second) {
			assert(first.columns_ == second.rows_ && "Matrix sizes do not match");

			Matrix<T> tmp(first.rows_, second.columns_);

			for (size_t i = 0; i < tmp.rows_; i++) {
				for (size_t j = 0; j < tmp.columns_; j++) {
					tmp.data_[i][j] = 0;
					for (size_t k = 0; k < first.columns_; k++) {
						if (first.data_[i][k] != 0 || second.data_[k][j] != 0)
							tmp.data_[i][j] += (first.data_[i][k] / second.data_[k][j]);
					}
				}
			}
			return tmp;

		}
		Matrix& operator=(const Matrix<T>& second)
		{
			if (data_ != nullptr)
				clear();

			rows_ = second.rows_;
			columns_ = second.columns_;

			data_ = new T * [rows_];
			for (size_t i = 0; i < columns_; i++)
				data_[i] = new T[columns_];

			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_; j++)
					data_[i][j] = second.data_[i][j];
			}
			return *this;
		}
		friend bool operator==(const Matrix<T>& first, const Matrix<T>& second) {
			if (first.rows_ != second.rows_ || first.columns_ != second.columns_) return false;

			for (size_t i = 0; i < first.rows_; i++) {
				for (size_t j = 0; j < first.columns_; j++)
					if (first.data_[i][j] != second.data_[i][j])
						return false;

			}
			return true;
		}
		friend bool operator!=(Matrix<T>& first, Matrix<T>& second) {
			if (first.rows_ != second.rows_ || first.columns_ != second.columns_)
				return true;

			for (size_t i = 0; i < first.rows_; i++)
				for (size_t j = 0; j < first.columns_; j++)
					if (first.data_[i][j] != second.data_[i][j])
						return true;
			return false;
		}

		void clear() {
			for (size_t i = 0; i < rows_; i++) {
				delete[] data_[i];
				data_[i] = nullptr;
			}
			delete data_;
			data_ = nullptr;
			rows_ = 0;
			columns_ = 0;
		}
		void randomFill(const T min = 0, const T max = 10) {
			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_; j++) {
					if constexpr (is_integral_v<T>) data_[i][j] = rand() % (max - min + 1) + min;
					else data_[i][j] = (max - min) * rand() / RAND_MAX + min;
				}
			}
		}
		void print(uint32_t space = 2, uint32_t height = 0) {
			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_; j++)
					cout << setw(maxSize(data_, rows_, columns_) + space)
					<< data_[i][j];
				cout << endl;
				for (size_t j = 0; j < height; j++)
					cout << endl;

			}
		}
		Matrix<T> transposition() {
			T** newData;
			initialize(newData, columns_, rows_);

			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_; j++)
					newData[j][i] = data_[i][j];
			}
			return Matrix<T>(newData, columns_, rows_);
		}
		
		uint32_t height() const  { return rows_; }
		uint32_t width()  const  { return columns_; }
		void deleteRow(int pos = -1) {
			assert(pos >= -1 && pos <= rows_ - 1);
			if (pos == -1) pos = rows_ - 1;

			T** NewData = new T * [rows_ - 1];

			initialize(NewData, rows_ - 1, columns_);

			if (pos != 0)
				for (size_t i = 0; i < pos; i++)
					for (size_t j = 0; j < columns_; j++)
						NewData[i][j] = data_[i][j];

			for (size_t i = pos + 1, i_posNew = pos; i < rows_; i++, i_posNew++)
				for (size_t j = 0, j_posNew = 0; j < columns_; j++, j_posNew++)
					NewData[i_posNew][j_posNew] = data_[i][j];

			Delete(data_, rows_);
			data_ = NewData;
			--rows_;
		}
		void deleteColumn(int pos = -1) {
			if (pos == -1) pos = columns_ - 1;
			assert(pos >= 0 && pos <= columns_ - 1);

			T** NewData;
			initialize(NewData, rows_, columns_ - 1);

			if (pos != 0) {
				for (size_t i = 0; i < rows_; i++) {
					for (size_t j = 0; j < pos; j++)
						NewData[i][j] = data_[i][j];
				}
			}

			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = pos + 1, j_NewData = pos; j < columns_; j++, j_NewData++)
					NewData[i][j_NewData] = data_[i][j];
			}

			Delete(data_, rows_);
			data_ = NewData;
			columns_--;

		}
		void addRow(T* elem, int pos = -1) {
			assert(pos >= -1 || pos < rows_);
			if (pos == -1) pos = rows_;

			T** NewData;
			initialize(NewData, rows_ + 1, columns_);


			for (size_t i = 0; i < pos; i++)
				for (size_t j = 0; j < columns_; j++)
					NewData[i][j] = data_[i][j];

			for (size_t i = 0; i < columns_; i++)
				NewData[pos][i] = elem[i];

			for (size_t i = pos; i < rows_; i++)
				for (size_t j = 0; j < columns_; j++)
					NewData[i + 1][j] = data_[i][j];


			Delete(data_, rows_);
			data_ = NewData;
			rows_++;
		}
		void addCol(T* elem, int pos = -1) {
			assert(pos > 0 || pos < columns_);

			if (pos != -1)
				pos = columns_;

			T** NewData;
			initialize(NewData, rows_, columns_ + 1);

			for (size_t i = 0; i < rows_; i++)
				for (size_t j = 0; j < pos; j++)
					NewData[i][j] = data_[i][j];

			for (size_t i = 0; i < rows_; i++)
				NewData[i][pos] = elem[i];


			for (size_t i = 0; i < rows_; i++)
				for (size_t j_new = pos + 1, j_old = pos; j_old < columns_; j_new++, j_old++)
					NewData[i][j_new] = data_[i][j_old];


			Delete(data_, rows_);
			data_ = NewData;
			columns_++;

		}

		T max() {
			T result = data_[0][0];
			for (size_t i = 0; i < rows_; i++)
				for (size_t j = 0; j < columns_; j++)
					if (data_[i][j] > result)
						result = data_[i][j];
			return result;
		}
		T min() {
			T result = data_[0][0];
			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_; j++) {
					if (data_[i][j] < result)
						result = data_[i][j];
				}
			}
			return result;
		}

		void sortLineByLine() {
			for (size_t i = 0; i < rows_; i++) {
				for (size_t j = 0; j < columns_ - 1; j++) {
					int min = j;
					for (size_t b = j + 1; b < columns_; b++) {
						if (data_[i][b] < data_[i][min])
							min = b;
					}
					swap(data_[i][j], data_[i][min]);
				}
			}
		}
		void sort() {
			for (size_t i_swap = 0; i_swap < rows_; i_swap++) {
				for (size_t j_swap = 0; j_swap < columns_; j_swap++) {
					int row_min = i_swap;
					int col_min = j_swap;
					for (size_t i = row_min; i < rows_; i++) {
						if (i == row_min) {
							for (size_t j = col_min + 1; j < columns_; j++) {
								if (data_[i][j] < data_[row_min][col_min]) {
									row_min = i;
									col_min = j;
								}
							}
						}
						else {
							for (size_t j = 0; j < columns_; j++) {
								if (data_[i][j] < data_[row_min][col_min]) {
									row_min = i;
									col_min = j;
								}
							}
						}
					}
					swap(data_[i_swap][j_swap], data_[row_min][col_min]);
				}

			}
		}

		Matrix<T> pow(int degree = 2) {
			assert(rows_ == columns_ & degree >= 0);
			Matrix<T> result = *this;
			for (size_t i = 1; i < degree; i++)
				result = result * result;
			return result;

		}
		T determinant(T** data = nullptr, int row = 0, int col = 0) {
			assert(row == col && row >= 0 && col >= 0);
			if (!data) {
				data = this->data_;
				row = rows_;
				col = columns_;
			}

			if (row == 2 && col == 2)
				return (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);



			T** newData = nullptr;
			Matrix<T>::initialize(newData, row - 1, col - 1);

			T result = 0;
			for (size_t i = 0; i < col; i++) {

				int r = 0, c = 0;
				// create sub matrix
				for (size_t k = 1; k < row; ++k)
					for (size_t j = 0; j < col; ++j) {
						if (j != i) {
							newData[r][c] = data[k][j];

							c += 1;
							if (c == col - 1) {
								c = 0;
								r += 1;
							}

						}
					}
				result += std::pow(-1, i + 2) *
					data_[0][i] * determinant(newData, row - 1, col - 1);
			}
			return result;
		}
		Vector<T> getVector() {
			Vector<T> result(rows_ * columns_);

			for (size_t i = 0; i < currRow_ + 1; i++) {
				for (size_t j = 0; j < columns_; j++) {
					result.pushBack(data_[i][j]);
					if (currRow_ == i && j == currCol_)
						break;
				}
			}
			return result;

		}

		//Gauss-Jordan method for calculate invesrse matrix
		Matrix<T> inverse() {
			assert(rows_ == columns_);

			Matrix<T> augmented(rows_, columns_ * 2);

			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j) {
					augmented.data_[i][j] = data_[i][j];
					augmented.data_[i][j + columns_] = (i == j) ? 1 : 0;
				}

			for (int i = 0; i < rows_; ++i) {
				if (augmented.data_[i][i] == 0) {
					int nonZeroRow = -1;

					for (int k = i + 1; k < rows_; ++k)
						if (augmented.data_[k][i] != 0) {
							nonZeroRow = k;
							break;
						}

					assert(nonZeroRow != -1);

					for (int j = 0; j < columns_ * 2; ++j)
						std::swap(augmented.data_[i][j], augmented.data_[nonZeroRow][j]);
				}


				T divisor = augmented.data_[i][i];
				for (int j = 0; j < columns_ * 2; ++j)
					augmented.data_[i][j] /= divisor;

				for (int k = 0; k < rows_; ++k)
					if (k != i) {
						T factor = augmented.data_[k][i];
						for (int j = 0; j < columns_ * 2; ++j)
							augmented.data_[k][j] -= factor * augmented.data_[i][j];
					}

			}



			Matrix<T> result(rows_, columns_);
			for (int i = 0; i < rows_; ++i)
				for (int j = 0; j < columns_; ++j)
					result.data_[i][j] = augmented.data_[i][j + columns_];

			return result;
		}

		static Matrix<double> parse(const String& data) {
			auto rows = data.split('\n', false);
			int h = rows.size();
			int w = rows[0].split(' ', false).size();

			auto result = Matrix<double>(h, w);

			for(const auto& row : rows)	{
				for(const auto& elem : row.split(' ', false)) {
					double buffer;
					_atodbl((_CRT_DOUBLE*)&buffer, (char*)elem.cStr());
					result.append(buffer);
				}
			}
			return result;

			
		}
	};


	Matrix<double> KbInput(uint32_t row, uint32_t col, int posX = -1, int posY = -1) {
		HANDLE stdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (posX == -1 || posY == -1) {
			CONSOLE_SCREEN_BUFFER_INFO cbsi;
			GetConsoleScreenBufferInfo(stdHandle, &cbsi);
			posX = cbsi.dwCursorPosition.X;
			posY = cbsi.dwCursorPosition.Y;
		}

		Matrix<double> result(row, col);
		for (size_t i = 0; i < row; i++) {
			COORD coord;
			coord.Y = i + posY;
			for (size_t j = 0; j < col; j++) {
				if (j == 0) coord.X = j + posX;
				bool flag;
				do {
					int tmp;
					SetConsoleCursorPosition(stdHandle, coord);

					cin >> tmp;
					result.append(tmp);

					int size_tmpElem = to_string(tmp).size();
					if (cin.fail()) {
						cin.clear();
						cin.ignore(32767, '\n');
						SetConsoleCursorPosition(stdHandle, coord);
						for (size_t i = 0; i < 100; i++) cout << " ";
						flag = true;
					}
					else {
						flag = false;
						coord.X += size_tmpElem + 2;
					}
				} while (flag);
			}
		}
		return result;
	}
}