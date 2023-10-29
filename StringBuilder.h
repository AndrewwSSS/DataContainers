#pragma once
#include "String.h"
using namespace std;


namespace DataContainers
{
	class StringBuilder
	{
	private:
		char** data_;		 
		uint32_t size_;		 // Number of substrings
		uint32_t capacity_;
		uint32_t totalLen_;  // Len of result string 

		void setCapacity(uint32_t newCapacity = 0) {

			if (newCapacity == 0 || newCapacity <= size_)
				newCapacity = 2 * capacity_;

			char** newData = new char*[newCapacity];
			for (int i = 0; i < newCapacity; i++)
				newData[i] = nullptr;

			for (int i = 0; i < size_; i++)
				newData[i] = data_[i];

			delete[] data_;
			data_ = newData;
			capacity_ = newCapacity;
		}
		int getLen(const char* string) {
			int result= 0;
			while (string[res++] != '\0') {}
			return result;
		}
	public:
		StringBuilder(uint32_t capacity = 8) {
			this->capacity_ = capacity;
			data_ = new char*[this->capacity_];
			size_ = 0;
			totalLen_ = 0;
		}
		~StringBuilder() {
			for (int i = 0; i < size_; i++) {
				delete[] data_[i];
				data_[i] = nullptr;
			}
			capacity_ = 0;
			size_ = 0;
			delete[] data_;
		}

		void clear() {
			for (int i = 0; i < size_; i++) {
				delete[] data_[i];
				data_[i] = nullptr;
			}
			delete[] data_;

			this->capacity_ = 8;
			data_ = new char* [this->capacity_];
			size_ = 0;
		}
		void append(const char* string, bool endLine = false) {
			int len = getLen(string);

			if (size_ == capacity_)
				setCapacity();

			if (endLine)
				len += 1;

			data_[size_] = new char[len];  // allocate memory to store new line

			for (int i = 0; string[i] != '\0'; i++)
				data_[size_][i] = string[i];

			if(endLine)
				data_[size_][len - 2] = '\n';

			data_[size_][len - 1] = '\0';
			totalLen_ += len - 1;
			size_++;

		}
		void append(const String& str, bool endLine = false) {
			uint32_t len = str.len();

			if (size_ == capacity_)
				setCapacity();

			if (endLine)
				len += 1;

			data_[size_] = new char[len];  // allocate memory to store new line

			for (int i = 0; str[i] != '\0'; i++)
				data_[size_][i] = str[i];

			if (endLine)
				data_[size_][len - 2] = '\n';

			data_[size_][len - 1] = '\0';
			totalLen_ += len - 1;
			size_++;
		}
		void appendLine(const char* string = "") {
			append(string, true);
		}

		String toString() const {
			char* result = new char[totalLen_ + 1];
			uint32_t current = 0;
			for (uint32_t s = 0; s < size_; s++) {
				int i = 0;
				while (data_[s][i] != '\0')
					result[current++] = data_[s][i++];
			}
			result[totalLen_] = '\0';

			return String(result);
		}
	};
}


