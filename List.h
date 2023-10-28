#pragma once
#include <iostream>
#include <functional>

using namespace std;

namespace DataContainers {

	template<typename T>
	class List {
	private:
		class Node {
		public:
			Node* Previous;
			Node* Next;
			T Data;

			Node() {
				Next = nullptr;
				Previous = nullptr;
			}

			Node(T data) : Node() {
				this->Data = data;
			} 

			Node(Node* previous, Node* next) {
				this->Previous = previous;
				this->Next = next;
			}

			Node(Node* previous, Node* next, T data) : Node(previous, next) {
				this->Data = data;
			}

			~Node() { }

		};

		Node* head_;
		Node* tail_; 
		uint32_t size_;

		Node* NodeAt(int ind, uint32_t curr = 0, Node* node = nullptr) {

			assert(ind >= 0 && ind <= size_ - 1 && "Index out of range");

			if (ind == 0)
				return head_;
			if (ind == size_ - 1)
				return tail_;

		#ifdef RECURSION
			if (curr == ind)
				return node;

			if (node == nullptr) {
				if (size / 2 >= ind) {
					node = head;
					curr = 0;
				}
				else {
					node = tail;
					curr = size - 1;
				}
			}


			if (size / 2 >= ind) {
				node = node->Next;
				curr++;
			}
			else {
				node = node->Previous;
				curr--;
			}

			NodeAt(ind, curr, node);
		#else
			Node* current = nullptr;

			if (size_ / 2 >= ind) {
				current = this->head_;
				for (size_t i = 0; i < ind + 1; i++)
					current = current->Next;
				return current;
			}
			
			current = this->tail_;
			for (size_t i = ind; ind != i; i--)
				current = current->Previous;
			
			return current;
		#endif
		}
	public:
		class ListIterator {
		public:
			Node* node;

			ListIterator() {
				node = nullptr;
			}
			ListIterator(Node* node) {
				this->node = node;
			}

			void operator++() {
				node = node->Next;
			}
			bool operator!=(const ListIterator& iterator) const {
				return node != iterator.node;
			}
			T& operator*() {
				return node->Data;
			}
		};

		ListIterator begin() const {
			return head_;
		}
		ListIterator end() const {
			return nullptr;
		}

		List() {
			this->head_ = nullptr;
			this->tail_ = nullptr;
			size_ = 0;
		}
		List(const List<T>& list) {
			for (uint32_t i = 0; i < list.Size(); i++)
				pushBack(list[i]);
		}
		~List() {
			Clear();
		}
	
		void pushBack(const T& elem) {
			if (head_ == nullptr) {
				auto node = new Node(elem);
				head_ = node;
				size_++;
				return;
			}

			Node* current = (tail_ == nullptr) ? head_ : tail_;
			Node* newElem = new Node(current, nullptr, elem);
			current->Next = newElem;
			tail_ = newElem;

			size_++;
		}
		void pushBack(const List<T>& lst) {
			for (size_t i = 0; i < lst.Size(); i++)
				pushBack(lst[i]);
		}
		void pushFront(const T& elem) {
			if (head_ == nullptr) {
				auto node = new Node(elem);
				head_ = node;
				size_++;
				return;
			}

			Node* current = head_;
			Node* newElem = new Node(nullptr, current, elem);

			current->Previous = newElem;
			if (tail_ == nullptr)
				tail_ = current;
			head_ = newElem;

			size_++;
		}
		void insert(const T& elem, uint32_t ind) {
			assert(ind <= size_ - 1 && "Index out of range");
			Node* current = NodeAt(ind);

			Node* prev = current->Previous;
			Node* newElem = new Node(prev, current, elem);

			prev->Next = newElem;
			current->Previous = newElem;
		}

		void append(const T& elem) {
			pushBack(elem);
		}

		void ForEach(const function<void(T)>& function) {
			Node* current = head_;
			do {
				function(current->Data);
				current = current->Next;
			} while (current != nullptr);
		}
		List<T> Filter(const function<bool(T)>& predicate) {
			auto result = List<T>();
			if (size_ == 0)
				return result;

			Node* current = head_;
			do {
				if (predicate(current->Data))
					result.pushBack(current->Data);
				current = current->Next;
			} while (current != nullptr);
			return result;

		}
		T Reduce(std::function<T(T, T)> func) {
			T startVal = head_->Data;
			Node* node = head_->Next;
			for (;node != nullptr;) {
				startVal = func(startVal, node->Data);
				node = node->Next;
			}
			return startVal;
		}

		T PopBack() {
			assert(size_ > 0 && "List is empty");

			if (size_ == 1) {
				size_--;
				T& data = head_->Data;
				head_ = nullptr;
				return data;
			}

			if (size_ == 2) {
				size_--;
				T& data = tail_->Data;
				tail_ = nullptr;
				head_->Next = nullptr;
				return data;
			}

			T& data = tail_->Data;

			auto current = tail_;
			current = current->Previous;
			current->Next = nullptr;
			tail_ = current;
			size_--;

			return data;
		}
		T PopFront() {
			assert(size_ > 0 && "List is empty");

			if (size_ == 1) {
				size_--;
				T data = head_->Data;
				delete head_;
				head_ = nullptr;
				return data;
			}

			T data = head_->Data;
			auto current = head_->Next;
			delete head_;
			current->Previous = nullptr;
			head_ = current;
			size_--;
			return data;
		}
		T Pop(uint32_t ind) {
			assert(ind >= 0 && ind <= size_ - 1 && "Index out of range");
			if (ind == 0)
				return PopFront();
			if (ind == size_ - 1)
				return PopBack();


			Node* current = NodeAt(ind);

			Node* prev = current->Previous;
			Node* next = current->Next;

			if (prev != nullptr)
				prev->Next = next;

			if (next != nullptr)
				next->Previous = prev;

			size_--;
			type data = current->Data;
			delete current;
			return data;
		}
		void Remove(uint32_t ind) {
			assert(ind < size_ && ind >= 0 && "Index out of range");

			Node* current = NodeAt(ind);

			Node* prev = current->Previous;
			Node* next = current->Next;


			if (prev != nullptr)
				prev->Next = next;

			if (next != nullptr)
				next->Previous = prev;

			if (ind == size_ - 1) {
				PopBack();
				return;
			}

			if (ind == 0) {
				PopFront();
				return;
			}

			delete current;

			if (size_ == 1)
				head_ = nullptr;

			size_--;
		}
		void Clear() {
			while (size_ > 0)
				Remove(0);
		}

		T At(uint32_t, uint32_t curr = 0, Node* node = nullptr) const {
			assert(ind >= 0 && ind <= size_ - 1 && "Invalid indition to insert");
#ifdef RECURSION

			if (ind == 0)
				return head->Data;
			if (ind == size - 1)
				return tail->Data;

			if (curr == ind)
				return node->Data;

			if (node == nullptr) {
				if (size / 2 >= ind) {
					node = head;
					curr = 0;
				}
				else {
					node = tail;
					curr = size - 1;
				}
			}


			if (size / 2 >= ind) {
				curr++;
				At(ind, curr, node->Next);
			}
			else {
				curr--;
				At(ind, curr, node->Previous);
			}


#else
			Node* current = nullptr;
			if (size_ / 2 >= ind) {
				current = head_;
				for (size_t i = 0; i < ind; i++)
					current = current->Next;
			}
			else {
				current = tail_;

				for (size_t i = size_ - 1; ind != i; i--)
					current = current->Previous;
			}
			return current->Data;
#endif
		}
		T operator[](uint32_t ind) const {
			return At(ind);
		}
		uint32_t Size() const {
			return this->size_;
		}
		bool Empty() const {
			return size_ == 0;
		}
	};
}
