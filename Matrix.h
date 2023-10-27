#pragma once
#define NOMINMAX
#include <Windows.h>
#include <iomanip>
#include "String.h"
#include <iostream>
#include <string>
#include <cassert>
#include <optional>
#include <format>
#include "Vector.h"


namespace DataContainers {
	template <typename type, typename = enable_if_t<is_arithmetic_v<type>>>
	class Matrix {
	private:
		unsigned int rows;
		unsigned int columns;
		int currRow;
		int currCol;
		type** data;

		static int MaxSize(type** init, int row, int col) {
			int res = 0;
			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++)
					if (to_string(init[i][j]).size() > res)
						res = to_string(init[i][j]).size();
			return res;
		}
	public:
		~Matrix() {
			for (size_t i = 0; i < rows; i++) {
				delete[] data[i];
				data[i] = nullptr;
			}
			delete data;
			data = nullptr;
		}
		Matrix(unsigned int row, unsigned int col) {
			data = new type * [row];
			for (size_t i = 0; i < row; i++)
				data[i] = new type[col];

			this->rows = row;
			this->columns = col;
			this->currRow = 0;
			this->currCol = -1;
		}
		Matrix(const Matrix<type>& elem) : Matrix(elem.rows, elem.columns) {
			assert(elem.Height() == rows && elem.Width() == columns);
			for (size_t i = 0; i < rows; i++)
				for (size_t j = 0; j < columns; j++)
					data[i][j] = elem.data[i][j];
		}
		Matrix(type** data, unsigned int row, unsigned int col) {
			this->data = new type * [row];
			for (size_t i = 0; i < row; i++)
				this->data[i] = new type[col];

			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++)
					this->data[i][j] = data[i][j];

			this->rows = row;
			this->columns = col;
			this->currCol = row - 1;
			this->currRow = col - 1;
		}
		Matrix(type* data, unsigned int row, unsigned int col) : Matrix(row, col) {
			int tmp = 0;
			for (size_t i = 0; i < row; i++)
				for (size_t j = 0; j < col; j++) {
					this->data[i][j] = data[tmp++];
					currCol = j;
					currRow = i;
				}
		}

		void Append(type elem) {
			if (currCol + 1 <= columns - 1) {
				currCol += 1;
				data[currRow][currCol] = elem;
			}
			else {
				if (currRow + 1 <= rows - 1) {
					data[currRow + 1][0] = elem;
					currRow += 1;
					currCol = 0;
				}
				else
					throw "Matrix if full";
			}
		}
		static void Initialize(type**& init, int row, int col) {
			init = new type * [row];
			for (size_t i = 0; i < row; i++)
				init[i] = new type[col];
		}
		static void Delete(type**& init, int row) {
			for (size_t i = 0; i < row; i++) {
				delete[] init[i];
				init[i] = nullptr;
			}
			init = nullptr;
		}
		type* GetColumn(int col) {
			type* result = new type[columns];
			for (size_t i = 0; i < rows; i++)
				result[i] = data[i][col];
			return result;

		}
		type* GetRow(int row) {
			type* result = new type[rows];
			for (size_t i = 0; i < columns; i++)
				result[i] = data[row][i];
			return result;
		}

		friend ostream& operator<<(ostream& out, Matrix<type> p) {
			int maxSize = MaxSize(p.data, p.rows, p.columns);

			for (size_t i = 0; i < p.rows; i++) {
				for (size_t j = 0; j < p.columns; j++)
					out << setw(maxSize + 2) << p.data[i][j];
				out << endl;
			}
			return out;
		}
		friend istream& operator>>(istream& in, Matrix<type>& p) {
			for (size_t i = 0; i < p.rows; i++) {
				COORD coord;
				coord.Y = i + 1;
				for (size_t j = 0; j < p.columns; j++) {
					if (j == 0)
						coord.X = j + 1;
					bool flag;
					do {
						int tmp;
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

						in >> tmp;
						p.data[i][j] = tmp;

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
		friend Matrix operator+(const Matrix<type>& first, const Matrix<type>& second) {
			assert(first.columns == second.rows);

			Matrix<type> tmp(first.rows, second.columns);

			for (size_t i = 0; i < tmp.rows; i++) {
				for (size_t j = 0; j < tmp.columns; j++) {
					tmp.data[i][j] = 0;
					for (size_t k = 0; k < first.columns; k++) {

						if (first.data[i][k] != 0 || second.data[k][j] != 0)
							tmp.data[i][j] += (first.data[i][k] + second.data[k][j]);

					}
				}
			}
			return tmp;

		}
		friend Matrix operator-(const Matrix<type>& first, const Matrix<type>& second) {
			assert((first.columns == second.columns && first.rows == second.rows) && "Matrix sizes do not match");


			//cout << "\n\n\n";
			//first.Print();
			//cout << "\n\n\n";
			//second.Print();


			Matrix<type> tmp(first.rows, first.columns);

			for (size_t i = 0; i < tmp.rows; i++)
				for (size_t j = 0; j < tmp.columns; j++) {
					/*cout << first.data[i][j] << " " << second.data[i][j];
					cout << "\n\n";*/
					tmp.data[i][j] = first.data[i][j] - second.data[i][j];
				}

			return tmp;
		}
		friend Matrix operator*(const Matrix<type>& first, const Matrix<type>& second) {
			assert(first.columns == second.rows);

			Matrix<type> tmp(first.rows, second.columns);

			for (int i = 0; i < tmp.rows; ++i)
				for (int j = 0; j < tmp.columns; ++j)
					tmp.data[i][j] = 0;

			for (int i = 0; i < tmp.rows; ++i)
				for (int j = 0; j < tmp.columns; ++j)
					for (int k = 0; k < first.columns; ++k)
						tmp.data[i][j] += first.data[i][k] * second.data[k][j];

			return tmp;
		}
		friend Matrix operator*(const type num, const Matrix<type>& second) {
			Matrix<type> tmp(second.rows, second.columns);

			for (size_t i = 0; i < tmp.rows; i++) {
				for (size_t j = 0; j < tmp.columns; j++) {
					for (size_t k = 0; k < second.columns; k++)
						tmp.data[i][j] = second.data[i][j] * num;
				}
			}
			return tmp;
		}
		friend Matrix operator*(Matrix<type>& second, type num) {
			Matrix<type> tmp(second.rows, second.columns);

			for (size_t i = 0; i < tmp.rows; i++) {
				for (size_t j = 0; j < tmp.columns; j++) {
					for (size_t k = 0; k < second.columns; k++)
						tmp.data[i][j] = second.data[i][j] * num;
				}
			}
			return tmp;
		}
		friend Matrix operator/(const Matrix<type>& first, const Matrix<type>& second) {
			assert((first.columns == second.rows) && "Matrix sizes do not match");

			Matrix<type> tmp(first.rows, second.columns);

			for (size_t i = 0; i < tmp.rows; i++) {
				for (size_t j = 0; j < tmp.columns; j++) {
					tmp.data[i][j] = 0;
					for (size_t k = 0; k < first.columns; k++) {
						if (first.data[i][k] != 0 || second.data[k][j] != 0)
							tmp.data[i][j] += (first.data[i][k] / second.data[k][j]);
					}
				}
			}
			return tmp;

		}
		Matrix& operator=(const Matrix<type>& second)
		{
			if (data != nullptr)
				Clear();

			rows = second.rows;
			columns = second.columns;

			data = new type * [rows];
			for (size_t i = 0; i < columns; i++)
				data[i] = new type[columns];

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++)
					data[i][j] = second.data[i][j];
			}
			return *this;
		}
		friend bool operator==(Matrix<type>& first, Matrix<type>& second) {
			if (first.rows != second.rows || first.columns != second.columns) return false;

			for (size_t i = 0; i < first.rows; i++) {
				for (size_t j = 0; j < first.columns; j++)
					if (first.data[i][j] != second.data[i][j])
						return false;

			}
			return true;
		}
		friend bool operator!=(Matrix<type>& first, Matrix<type>& second) {
			if (first.rows != second.rows || first.columns != second.columns)
				return true;

			for (size_t i = 0; i < first.rows; i++) {
				for (size_t j = 0; j < first.columns; j++)
					if (first.data[i][j] != second.data[i][j])
						return true;
			}
			return false;
		}

		void Clear()
		{
			for (size_t i = 0; i < rows; i++) {
				delete[] data[i];
				data[i] = nullptr;
			}
			delete data;
			data = nullptr;
			rows = 0;
			columns = 0;
		}
		void RandomFill(const type min = 0, const type max = 10) {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++) {
					if constexpr (is_integral_v<type>) data[i][j] = rand() % (max - min + 1) + min;
					else data[i][j] = (max - min) * rand() / RAND_MAX + min;
				}
			}
		}
		void Print(unsigned int space = 2, unsigned int height = 0) {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++)
					cout << setw(MaxSize(data, rows, columns) + space)
					<< data[i][j];
				cout << endl;
				for (size_t j = 0; j < height; j++)
					cout << endl;

			}
		}
		Matrix<type> Transposition() {
			type** newData;
			Initialize(newData, columns, rows);

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++)
					newData[j][i] = data[i][j];
			}
			return Matrix<type>(newData, columns, rows);
		}
		// Request user input in console
		void KbInput(int posX = -1, int posY = -1) {
			if (posX == -1 || posY == -1) {
				HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO cbsi;
				GetConsoleScreenBufferInfo(h, &cbsi);
				posX = cbsi.dwCursorPosition.X;
				posY = cbsi.dwCursorPosition.Y;
			}


			for (size_t i = 0; i < rows; i++) {
				COORD coord;
				coord.Y = i + posY;
				for (size_t j = 0; j < columns; j++) {
					if (j == 0) coord.X = j + posX;
					bool flag;
					do {
						int tmp;
						SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

						cin >> tmp;
						data[i][j] = tmp;

						int size_tmpElem = to_string(tmp).size();
						if (cin.fail()) {
							cin.clear();
							cin.ignore(32767, '\n');
							SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
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
		}
		unsigned int Height() const { return rows; }
		unsigned int Width() const { return columns; }
		void DeleteRow(int pos = -1) {
			assert(pos >= -1 && pos <= rows - 1);
			if (pos == -1) pos = rows - 1;

			type** NewData = new type * [rows - 1];

			Initialize(NewData, rows - 1, columns);

			if (pos != 0) {
				for (size_t i = 0; i < pos; i++) {
					for (size_t j = 0; j < columns; j++)
						NewData[i][j] = data[i][j];
				}
			}

			for (size_t i = pos + 1, i_posNew = pos; i < rows; i++, i_posNew++) {
				for (size_t j = 0, j_posNew = 0; j < columns; j++, j_posNew++)
					NewData[i_posNew][j_posNew] = data[i][j];
			}

			Delete(data, rows);
			data = NewData;
			rows--;
		}
		void DeleteColumn(int pos = -1) {
			if (pos == -1) pos = columns - 1;
			assert(pos >= 0 && pos <= columns - 1);

			type** NewData;
			Initialize(NewData, rows, columns - 1);

			if (pos != 0) {
				for (size_t i = 0; i < rows; i++) {
					for (size_t j = 0; j < pos; j++)
						NewData[i][j] = data[i][j];
				}
			}

			for (size_t i = 0; i < rows; i++) {
				for (size_t j = pos + 1, j_NewData = pos; j < columns; j++, j_NewData++)
					NewData[i][j_NewData] = data[i][j];
			}

			Delete(data, rows);
			data = NewData;
			columns--;

		}
		void AddRow(type* elem, int pos = -1) {
			assert(pos >= -1 || pos < rows);
			if (pos == -1) pos = rows;

			type** NewData;
			Initialize(NewData, rows + 1, columns);


			for (size_t i = 0; i < pos; i++)
				for (size_t j = 0; j < columns; j++)
					NewData[i][j] = data[i][j];

			for (size_t i = 0; i < columns; i++)
				NewData[pos][i] = elem[i];

			for (size_t i = pos; i < rows; i++)
				for (size_t j = 0; j < columns; j++)
					NewData[i + 1][j] = data[i][j];


			Delete(data, rows);
			data = NewData;
			rows++;
		}
		void AddCol(type*& elem, int pos = -1) {
			assert(pos > 0 || pos < columns);

			if (pos != -1)
				pos = columns;

			type** NewData;
			Initialize(NewData, rows, columns + 1);

			for (size_t i = 0; i < rows; i++)
				for (size_t j = 0; j < pos; j++)
					NewData[i][j] = data[i][j];

			for (size_t i = 0; i < rows; i++)
				NewData[i][pos] = elem[i];


			for (size_t i = 0; i < rows; i++)
				for (size_t j_new = pos + 1, j_old = pos; j_old < columns; j_new++, j_old++)
					NewData[i][j_new] = data[i][j_old];


			Delete(data, rows);
			data = NewData;
			columns++;

		}
		type Max() {
			type res = data[0][0];
			for (size_t i = 0; i < rows; i++)
				for (size_t j = 0; j < columns; j++)
					if (data[i][j] > res)
						res = data[i][j];
			return res;
		}
		type Min() {
			type res = data[0][0];
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns; j++) {
					if (data[i][j] < res)
						res = data[i][j];
				}
			}
			return res;
		}
		void SortLineByLine() {
			for (size_t i = 0; i < rows; i++) {
				for (size_t j = 0; j < columns - 1; j++) {
					int min = j;
					for (size_t b = j + 1; b < columns; b++) {
						if (data[i][b] < data[i][min])
							min = b;
					}
					swap(data[i][j], data[i][min]);
				}
			}
		}
		void Sort() {
			for (size_t i_swap = 0; i_swap < rows; i_swap++) {
				for (size_t j_swap = 0; j_swap < columns; j_swap++) {
					int row_min = i_swap;
					int col_min = j_swap;
					for (size_t i = row_min; i < rows; i++) {
						if (i == row_min) {
							for (size_t j = col_min + 1; j < columns; j++) {
								if (data[i][j] < data[row_min][col_min]) {
									row_min = i;
									col_min = j;
								}
							}
						}
						else {
							for (size_t j = 0; j < columns; j++) {
								if (data[i][j] < data[row_min][col_min]) {
									row_min = i;
									col_min = j;
								}
							}
						}
					}
					swap(data[i_swap][j_swap], data[row_min][col_min]);
				}

			}
		}
		Matrix<type> Pow(int degree = 2) {
			assert(rows == columns & degree >= 0);
			Matrix<type> result = *this;
			for (size_t i = 1; i < degree; i++)
				result = result * result;
			return result;

		}
		type Determinant(type** data = nullptr, int row = 0, int col = 0) {
			assert(row == col && row >= 0 && col >= 0);
			if (!data) {
				data = this->data;
				row = rows;
				col = columns;
			}

			if (row == 2 && col == 2)
				return (data[0][0] * data[1][1]) - (data[0][1] * data[1][0]);



			type** newData = nullptr;
			Matrix<type>::Initialize(newData, row - 1, col - 1);

			type result = 0;
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
			}
			return result;
		}
		Vector<type> GetVector() {

			Vector<type> result(rows * columns);

			for (size_t i = 0; i < currRow + 1; i++) {
				for (size_t j = 0; j < columns; j++) {
					result.PushBack(data[i][j]);
					if (currRow == i && j == currCol)
						break;
				}
			}
			return result;

		}

		//Gauss-Jordan method for calculate invesrse matrix
		Matrix<type> Inverse() {
			assert(rows == columns);

			Matrix<type> augmented(rows, columns * 2);

			for (int i = 0; i < rows; ++i)
				for (int j = 0; j < columns; ++j) {
					augmented.data[i][j] = data[i][j];
					augmented.data[i][j + columns] = (i == j) ? 1 : 0;
				}

			for (int i = 0; i < rows; ++i) {

				if (augmented.data[i][i] == 0) {
					int nonZeroRow = -1;

					for (int k = i + 1; k < rows; ++k)
						if (augmented.data[k][i] != 0) {
							nonZeroRow = k;
							break;
						}

					assert(nonZeroRow != -1); // Матрица вырожденная

					for (int j = 0; j < columns * 2; ++j)
						std::swap(augmented.data[i][j], augmented.data[nonZeroRow][j]);
				}



				type divisor = augmented.data[i][i];
				for (int j = 0; j < columns * 2; ++j)
					augmented.data[i][j] /= divisor;

				// Обнуляем все элементы в столбце, кроме текущего
				for (int k = 0; k < rows; ++k)
					if (k != i) {
						type factor = augmented.data[k][i];
						for (int j = 0; j < columns * 2; ++j)
							augmented.data[k][j] -= factor * augmented.data[i][j];
					}

			}



			Matrix<type> result(rows, columns);
			for (int i = 0; i < rows; ++i)
				for (int j = 0; j < columns; ++j)
					result.data[i][j] = augmented.data[i][j + columns];

			return result;
		}

		static Matrix<double> Parse(const String& data) {
			auto rows = data.Split('\n', false);
			int h = rows.Size();
			int w = rows[0].Split(' ', false).Size();

			auto m = Matrix<double>(h, w);

			for(const auto& row : rows)	{
				for(const auto& elem : row.Split(' ', false)) {
					double buffer;
					_atodbl((_CRT_DOUBLE*)&buffer, elem.CStr());
					m.Append(buffer);
				}
			}
			return m;

			
		}

	};
}