#pragma once
#include <cassert>
#include<iostream>
#include<fstream>
#include<istream>
#include <vector>
#include "Vector.h"

using namespace std;
namespace DataContainers{
	class String
	{
	protected:
		char* data;
		uint32_t size;
		// TODO implement capacity
		//uint32_t capacity;
	public:
		String() {
			data = nullptr;
			size = 0;
		}
		String(const char* str) {
			size = getLen(str)+1;
			data = new char[size];

			for (uint32_t i = 0; i < size; i++)
				data[i] = str[i];
			data[size - 1] = '\0';

		}
		String(const String& str) {
			this->size = str.size;
			this->data = new char[this->size];

			for (int i = 0; i < this->size; i++)
				this->data[i] = str.data[i];
		}
		~String() {
			Ñlear();
		}

		static uint32_t getLen(const char* str) {
			int len = 0;
			while (str[len] != '\0')
				len += 1;
			return len;
		}
		String Append(const char* str) {
			const uint32_t strtotalLen = getLen(str);

			if (str == nullptr || strtotalLen == 1)
				return *this;

			uint32_t newLen = 0;

			if (size == 0)
				newLen++;
			else
				newLen += size - 1;

			newLen += strtotalLen;

			char* newData = new char[newLen];

			for (uint32_t i = 0; i < size; i++)
				newData[i] = this->data[i];

			for (uint32_t i = 0; i < strtotalLen; i++)
				newData[size - 1 + i] = str[i];

			newData[size + strtotalLen - 1] = '\0';
			return String{ newData };
		}
		String Append(const String& str) {
			return Append(str.CStr());
		}
		String Revers() const {
			char* newData = new char[size];
			for (uint32_t i = 0, j = size - 2; i < size - 1; i++, j--)
				newData[i] = this->data[j];

			newData[size - 1] = '\0';
			return newData;
		}
		void Ñlear() {
			if (data != nullptr) {
				delete[] data;
				data = nullptr;
			}
			size = 0;
		}
		bool Empty() const {
			return size == 0;
		}
		char* CStr() const {
			char* result = new char[size];

			for (uint32_t i = 0; i < size; i++)
				result[i] = data[i];

			return data;
		}
		uint32_t Len() const {
			return size-1;
		}
		char operator[](uint32_t index)const {
			assert(index <= size - 1 && "Index out of range");
			return data[index];
		}

		friend std::ostream& operator<<(std::ostream& os, const String& str) {
			if (!str.Empty())
				os << str.data;
			return os;
		}
		friend std::istream& operator>>(std::istream& in, String& str) {
			char* buffer = new char[_MAX_PATH];
			std::cin.getline(buffer, _MAX_PATH);
			const uint32_t totalLen = String::getLen(buffer);

			if (!str.Empty())
				str.Ñlear();

			str.data = new char[totalLen + 1];
			str.size = totalLen + 1;
			for (uint32_t i = 0; i < totalLen; i++)
				str.data[i] = buffer[i];

			str.data[totalLen] = '\0';

			return in;
		}


		void operator+=(const char* str) {
			const int oldLen = size;
			if (size == 0) size++;
			size += (String::getLen(str) - 1);

			char* oldData = new char[oldLen];

			for (uint32_t i = 0; i < oldLen; i++)
				oldData[i] = data[i];

			if (data != nullptr) {
				delete[]data;
				data = nullptr;
			}

			data = new char[size];
			uint32_t i = 0;

			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					data[i] = oldData[i];

			for (uint32_t j = 0; i < size; i++, j++)
				data[i] = str[j];
		}
		void operator+=(const String& str) {
			int oldLen = size;
			if (oldLen == 0) size++;
			size += (str.size - 1);

			char* oldData = new char[oldLen];

			for (uint32_t i = 0; i < oldLen; i++)
				oldData[i] = data[i];

			if (data != nullptr) {
				delete[]data;
				data = nullptr;
			}

			data = new char[size];
			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					data[i] = oldData[i];

			for (uint32_t j = 0; i < size; i++, j++)
				data[i] = str[j];

		}
		char* operator+(const char* str) {
			const uint32_t oldLen = size;
			if (size == 0) size++;
			size += (String::getLen(str) - 1);

			char* NewData = new char[size];

			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					NewData[i] = data[i];


			for (uint32_t j = 0; i < size; i++, j++)
				NewData[i] = str[j];

			return NewData;
		}
		String operator+(const String& str) {
			int oldLen = size;
			if (oldLen == 0) size++;
			size += (str.size - 1);

			char* NewData = new char[size];

			uint32_t i = 0;
			if (oldLen != 0)
				for (; i < oldLen - 1; i++)
					NewData[i] = data[i];

			for (uint32_t j = 0; i < size; i++, j++)
				NewData[i] = str[j];

			return String(NewData);
		}
		String& operator=(const char* str) {
			size = getLen(str);

			data = new char[size];
			for (uint32_t i = 0; i < size; i++)
				data[i] = str[i];

			return *this;
		}
		String& operator=(const String& str) {
			size = str.Len()+1;

			this->data = new char[size];
			for (uint32_t i = 0; i < size; i++)
				this->data[i] = str.data[i];

			return *this;
		}


		// Based on Levenshtein distance algorithm
		// Return similarity between strings in percentage from 0 to 100
		uint32_t Similarity(const String& str) {
			auto string1 = CStr();
			auto string2 = str.CStr();

			auto len1 = size-1;
			auto len2 = str.Len();

			if(len2 > len1) {
				swap(string1, string2);
				swap(len1, len2);
			}

			Vector<uint32_t> currentRow(len1+1);
			for (uint32_t i = 0; i < len1 + 1; i++)
				currentRow.PushBack(i);

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
					currentRow[j] = tmp.Reduce([](uint32_t a, uint32_t b)
					{
						return std::min(a, b);
					});
				}
			}
			return (1 - static_cast<float>(currentRow[len1]) / len2) * 100;
		}

		Vector<String> Split(const char separator = ' ', const bool emptyElements = true) const {
			Vector<String> result;

			uint32_t lastIDX = 0;
			for (uint32_t i = 0; i < size-1; i++) {

				if(i != 0 && data[i - 1] == separator && data[i] == separator) {
					if(emptyElements)
						result.Append("");
					lastIDX += 1;
					continue;
				}

				if((data[i] == separator && i != 0) ||
				   (data[i] != separator && i == size - 2)) {

					auto len = i - lastIDX;
					if (i == size - 2) {
						len += 1;
						i += 1;
					}
					char* prev = new char[len + 1];

					for (uint32_t k = lastIDX, ind = 0; k < i; k++, ind++)
						prev[ind] = data[k];
					
					prev[len] = '\0';
					result.Append(String(prev));
					lastIDX = i + 1;
				}
				else if (data[i] == separator && i == 0) {
					lastIDX += 1;
				}
			}

			return result;
		}
	};

	

	
}

