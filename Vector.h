#pragma once
#include <cassert>
#include <functional>
#include <iostream>

namespace DataContainers {
	template<typename T>
	class Vector
	{
	private:
		T* data_;
		uint32_t size_;
		uint32_t capacity_;

		void setCapacity(int value = -1) {
			if (value == -1)
				capacity_ *= 2;
			else if (capacity_ == 0)
				capacity_ = 8;

			T* newData = new T[capacity_];

			if (size_ != 0) {
				for (uint32_t i = 0; i < size_; i++)
					newData[i] = data_[i];

				delete[] data_;
			}
			data_ = newData;
		}
	public:
		Vector(uint32_t capacity = 8) {
			size_ = 0;
			capacity_ = capacity;
			data_ = new T[capacity];
		}
		Vector(const Vector<T>& list) : Vector(list.size()) {
			for (uint32_t i = 0; i < list.size(); i++)
				pushBack(list[i]);
		}
		Vector(const std::initializer_list<T> _arr) : Vector(_arr.size()) {
			for (const auto& elem : _arr)
				pushBack(elem);
		}
		~Vector() {
			clear();
		}

		void append(const std::initializer_list<T> _arr) {
			for (const auto& elem : _arr)
				pushBack(elem);
		}
		void append(const T& data) {
			pushBack(data);
		}

		void pushBack(const T& elem) {
			if (size_ == capacity_)
				setCapacity();

			data_[size_] = elem;
			size_++;
		}
		void pushBack(const Vector<T>& vector) {
			if (size_ + vector.size() > capacity_)
				setCapacity(size_ + vector.size());

			for (size_t i = size_; i < size_ + vector.size(); i++)
				data_[i] = vector[i - size_];

			size_ += vector.size();
		}
		void pushFront(const T& elem) {
			if (size_ == capacity_)
				setCapacity();


			T* newData = new T[capacity_];
			newData[0] = elem;

			for (uint32_t i = 0; i < size_; i++)
				newData[i + 1] = data_[i];

			delete[] data_;
			data_ = newData;
			size_++;
		}
		void remove(uint32_t ind) {
			assert(ind < size_ && "Index out of range");

			T* newData = new T[capacity_];

			for (uint32_t i = 0; i < ind; i++)
				newData[i] = data_[i];


			for (uint32_t i = ind + 1; i < size_; i++)
				newData[i] = data_[i - 1];


			delete[] data_;
			data_ = newData;
			size_--;
		}
		T popBack() {
			assert(size_ > 0);

			T elem = data_[size_ - 1];
			size_--;
			return elem;
		}
		T popFront() {
			T* newData = new T[capacity_];

			for (uint32_t i = 0; i < size_ - 1; i++)
				newData[i] = data_[i + 1];

			T result = data_[0];
			delete[] data_;
			data_ = newData;
			size_--;
			return result;
		}
		T Pop(uint32_t ind) {
			assert(ind < size_ && "Index out of range");
			if (ind == 0)
				return popFront();
			if (ind == size_ - 1)
				return popBack();

			T* newData = new T[capacity_];

			for (uint32_t i = 0; i < ind; i++)
				newData[i] = data_[i];

			T result = data_[ind];

			for (uint32_t i = ind + 1; i < size_; i++)
				newData[i] = data_[i - 1];

			delete[] data_;
			data_ = newData;
			size_--;
			return result;
		}
		void insert(const T& elem, uint32_t ind) {

			if (ind == 0)
				return pushFront(elem);
			if (ind == size_ - 1)
				return pushBack(elem);
			if (size_ == capacity_)
				setCapacity();

			T* newData = new T[capacity_];

			for (uint32_t i = 0; i < ind; i++)
				newData[i] = data_[i];

			newData[ind] = elem;

			for (uint32_t i = ind; i < size_; i++)
				newData[i + 1] = data_[i];


			delete[] data_;
			data_ = newData;

			size_++;
		}
		T& At(uint32_t ind) const {
			assert(ind < capacity_ && "Index out of range");
			return data_[ind];
		}

	
		void forEach(const std::function<void(T)>& function) {
			for (size_t i = 0; i < size_; i++)
				function(data_[i]);
		}
		Vector<T> filter(const std::function<bool(T)>& predicate) {
			Vector<T> result;
			for (size_t i = 0; i < size_; i++) {
				if (predicate(data_[i]))
					result.pushBack(data_[i]);
			}
			return result;
		}
		T reduce(std::function<T(T, T)> func) {
			T startVal = data_[0];
			for (size_t i = 1; i < size_; i++) {
				startVal = func(startVal, data_[i]);
			}
			return startVal;
		}

		T& operator[](uint32_t ind) const {
			return At(ind);
		}


		// iteration methods
		T* begin() const {
			return data_;
		}
		T* end() const {
			return data_ + size_;
		}


		void clear() {
			delete[] data_;
			data_ = nullptr;
			size_ = 0;
		}
		bool isEmpty() const {
			return size_ == 0;
		}
		uint32_t size() const {
			return size_;
		}
	};
}
