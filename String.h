#pragma once
#include <cassert>
#include<iostream>
#include<fstream>
#include<istream>
#include <vector>

#include "Vector.h"

namespace DataContainers{
	using namespace std;
	class String
	{
	protected:
		char* data_;
		uint32_t size_;
		// TODO implement capacity
		uint32_t capacity_;
	public:
		String() {
			data_ = nullptr;
			size_ = 0;
			
		}

		String(const char* str) {
			size_ = getLen(str)+1;
			data_ = new char[size_];

			for (uint32_t i = 0; i < size_; i++)
				data_[i] = str[i];
			data_[size_ - 1] = '\0';

		}
		String(const String& str) {
			this->size_ = str.size_;
			this->data_ = new char[this->size_];

			for (int i = 0; i < this->size_; i++)
				this->data_[i] = str.data_[i];
		}
		~String() {
			clear();
		}

		static uint32_t getLen(const char* str) {
			int len = 0;
			while (str[len] != '\0')
				len += 1;
			return len;
		}
		String append(const char* str) {
			const uint32_t len = getLen(str);

			if (str == nullptr || len == 1)
				return *this;

			uint32_t newLen = 0;

			if (size_ == 0)
				newLen++;
			else
				newLen += size_ - 1;

			newLen += len;

			char* newData = new char[newLen];

			for (uint32_t i = 0; i < size_; i++)
				newData[i] = this->data_[i];

			for (uint32_t i = 0; i < len; i++)
				newData[size_ - 1 + i] = str[i];

			newData[size_ + len - 1] = '\0';
			return String{ newData };
		}
		String append(const String& str) {
			return append(str.cStr());
		}
		String revers() const {
			char* newData = new char[size_];
			for (uint32_t i = 0, j = size_ - 2; i < size_ - 1; i++, j--)
				newData[i] = this->data_[j];

			newData[size_ - 1] = '\0';
			return newData;
		}
		void clear() {
			if (data_ != nullptr) {
				delete[] data_;
				data_ = nullptr;
			}
			size_ = 0;
		}
		bool empty() const {
			return size_ == 0;
		}
		char* cStr() const {
			char* result = new char[size_];

			for (uint32_t i = 0; i < size_; i++)
				result[i] = data_[i];

			return data_;
		}
		uint32_t len() const {
			return size_-1;
		}

		char operator[](uint32_t index) const {
			assert(index <= size_ - 1 && "Index out of range");
			return data_[index];
		}
		friend std::ostream& operator<<(std::ostream& os, const String& str) {
			if (!str.empty())
				os << str.data_;
			return os;
		}
		friend std::istream& operator>>(std::istream& in, String& str) {
			char* buffer = new char[_MAX_PATH];
			std::cin.getline(buffer, _MAX_PATH);
			const uint32_t totalLen = String::getLen(buffer);

			if (!str.empty())
				str.clear();

			str.data_ = new char[totalLen + 1];
			str.size_ = totalLen + 1;
			for (uint32_t i = 0; i < totalLen; i++)
				str.data_[i] = buffer[i];

			str.data_[totalLen] = '\0';

			return in;
		}
		void operator+=(const char* str) {
			const uint32_t oldLen = size_;
			if (size_ == 0) size_++;
			size_ += getLen(str) - 1;

			char* oldData = new char[oldLen];

			for (uint32_t i = 0; i < oldLen; i++)
				oldData[i] = data_[i];

			if (data_ != nullptr) {
				delete[]data_;
				data_ = nullptr;
			}

			data_ = new char[size_];
			uint32_t i = 0;

			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					data_[i] = oldData[i];

			for (uint32_t j = 0; i < size_; i++, j++)
				data_[i] = str[j];
		}
		void operator+=(const String& str) {
			const uint32_t oldLen = size_;
			if (oldLen == 0) size_++;
			size_ += str.size_ - 1;

			char* oldData = new char[oldLen];

			for (uint32_t i = 0; i < oldLen; i++)
				oldData[i] = data_[i];

			if (data_ != nullptr) {
				delete[]data_;
				data_ = nullptr;
			}

			data_ = new char[size_];
			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					data_[i] = oldData[i];

			for (uint32_t j = 0; i < size_; i++, j++)
				data_[i] = str[j];

		}
		char* operator+(const char* str) {
			const uint32_t oldLen = size_;
			if (size_ == 0) size_++;
			size_ += (String::getLen(str) - 1);

			char* NewData = new char[size_];

			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					NewData[i] = data_[i];


			for (uint32_t j = 0; i < size_; i++, j++)
				NewData[i] = str[j];

			return NewData;
		}
		String operator+(const String& str) {
			int oldLen = size_;
			if (oldLen == 0) size_++;
			size_ += (str.size_ - 1);

			char* NewData = new char[size_];

			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					NewData[i] = data_[i];

			for (uint32_t j = 0; i < size_; i++, j++)
				NewData[i] = str[j];

			return String(NewData);
		}
		String& operator=(const char* str) {
			size_ = getLen(str);

			data_ = new char[size_];
			for (uint32_t i = 0; i < size_; i++)
				data_[i] = str[i];

			return *this;
		}
		String& operator=(const String& str) {
			if (this == &str) return *this;
			size_ = str.len()+1;

			this->data_ = new char[size_];
			for (uint32_t i = 0; i < size_; i++)
				this->data_[i] = str.data_[i];

			return *this;
		}


		// Based on Levenshtein distance algorithm
		// Return similarity between strings in percentage from 0 to 100
		uint8_t similarity(const String& str) const {
			auto string1 = cStr();
			auto string2 = str.cStr();

			auto len1 = size_-1;
			auto len2 = str.len();

			if(len2 > len1) {
				swap(string1, string2);
				swap(len1, len2);
			}

			Vector<uint32_t> currentRow(len1+1);
			for (uint32_t i = 0; i < len1 + 1; i++)
				currentRow.pushBack(i);

			for (size_t i=1; i < len2+1; i++) {
				Vector<uint32_t> previousRow = currentRow;
				currentRow[0] = i;
				for (size_t j=1; j < len1+1; j++) {
					uint32_t add = previousRow[j] + 1;
					uint32_t del = currentRow[j - 1] + 1;
					uint32_t change = previousRow[j - 1];

					if (string1[j - 1] != string2[i - 1])
						change += 1;

					Vector<uint32_t> tmp = { add, del, change };
					currentRow[j] = tmp.reduce([](uint32_t a, uint32_t b)
					{
						return std::min(a, b);
					});
				}
			}
			return (1 - static_cast<float>(currentRow[len1]) / len2) * 100;
		}

		Vector<String> split(const char separator = ' ', const bool emptyElements = true) const {
			Vector<String> result;

			uint32_t lastIDX = 0;
			for (uint32_t i = 0; i < size_-1; i++) {

				if(i != 0 && data_[i - 1] == separator && data_[i] == separator) {
					if(emptyElements)
						result.append("");
					lastIDX += 1;
					continue;
				}

				if((data_[i] == separator && i != 0) ||
				   (data_[i] != separator && i == size_ - 2)) {

					auto len = i - lastIDX;

					if (i == size_ - 2) {
						len += 1;
						i += 1;
					}
					char* prev = new char[len + 1];

					for (uint32_t k = lastIDX, ind = 0; k < i; k++, ind++)
						prev[ind] = data_[k];
					
					prev[len] = '\0';
					result.append(String(prev));
					lastIDX = i + 1;
				}
				else if (data_[i] == separator && i == 0) {
					lastIDX += 1;
				}
			}

			return result;
		}
	};

	

	
}

