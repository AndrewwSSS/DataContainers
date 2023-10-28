#pragma once
#include <iostream>
#include <functional>

using namespace std;

namespace DataContainers {
	template<typename type>
	class LinkedList {
	private:
		class Node
		{
		public:
			Node* Next;
			type Data;

			Node(type elem = type(), Node* pNext = nullptr) {
				this->Data = elem;
				this->Next = pNext;
			}
		};

		Node* head_;
		uint32_t size_;

		Node* NodeAt(int ind, int curr = 0, Node* elem = nullptr) {
#ifdef RECURSION
			if (ind == 0)
				return head;

			if (ind == curr)
				return elem;

			if (elem == nullptr) elem = head;

			if (ind != curr) {
				curr++;
				NodeAt(ind, curr, elem->pNext);
			}
#else
			int number = 0;
			Node* current = head_;

			while (current != nullptr) {
				if (number == ind)
					return current;

				current = current->Next;
				number++;
			}
			return nullptr;
#endif
		}
	public:
		struct LinkedListIterator {
			Node* node;
			LinkedListIterator() {
				node = nullptr;
			}
			LinkedListIterator(Node* node) {
				this->node = node;
			}
			void operator++() {
				node = node->Next;
			}
			bool operator!=(const LinkedListIterator& iterator) const {
				return node != iterator.node;
			}
			type& operator*() {
				return node->Data;
			}
		};

		LinkedListIterator begin() const {
			return head_;
		}
		LinkedListIterator end() const {
			return nullptr;
		}

		LinkedList() {
			size_ = 0;
			head_ = nullptr;
		}
		LinkedList(const LinkedList<type>& _arr) {
			head_ = nullptr;
			Node* tmp = _arr.head_;
			size_ = 0;
			for (size_t i = 0; i < _arr.getSize(); i++) {
				PushBack(tmp->Data);
				tmp = tmp->Next;
			}
		}
		~LinkedList() {
			clear();
		}

		void PushBack(const type& elem) {
			if (size_ == 0) {
				head_ = new Node(elem, head_);
				size_++;
				return;
			}

			auto current = NodeAt(size_ - 1);
			Node* NewElem = new Node(elem, current->Next);
			current->Next = NewElem;
			size_++;
		}
		void PushFront(const type& elem) {
			head_ = new Node(elem, head_);
			size_++;
		}
		void Insert(const type&, unsigned int ind) {
			assert(ind <= size_ && "Index out of range");

			if (ind == 0)
				return PushFront(elem);
			if (ind == size_ - 1)
				return PushBack(elem);

			auto current = NodeAt(ind - 1);
			Node* NewElem = new Node(elem, current->Next);
			current->Next = NewElem;
			size_++;
		}


		void ForEach(const function<void(type&)>& function) {
			Node* current = head_;
			while (current != nullptr) {
				function(current->Data);
				current = current->Next;
			}
		}
		LinkedList<type> Filter(const function<bool(type)>& function) {
			auto result = LinkedList<type>();
			Node* current = head_;
			while (current != nullptr) {
				if (function(current->Data))
					result.pushBack(current->Data);
				current = current->Next;
			}
			return result;
		}
		type Reduce(std::function<type(type, type)> function) {
			type startVal = head_->Data;
			Node* node = head_->Next;
			for (; node != nullptr;) {
				startVal = function(startVal, node->Data);
				node = node->Next;
			}
			return startVal;
		}


		type& operator[](unsigned int ind) {
			return At(ind);
		}

		type PopBack(Node* elem = nullptr) {
			Node* current = NodeAt(size_ - 1);
			type data = current->Data;
			current->Next = nullptr;

			delete current;
			size_--;
			return data;
		}
		type PopFront() {
			Node* tmp = head_;
			type data = tmp->Data;
			head_ = tmp->Next;
			delete tmp;
			size_--;
			return data;
		}
		type Pop(uint32_t ind) {
#ifdef RECURSION
			if (ind == 0)
				return PopFront();
			else if (pos == 0) elem = head;

			if (ind == pos - 1 || ind == 1) {
				type data = elem->pNext->data;
				Node* tmp = elem->pNext;

				elem->pNext = elem->pNext->pNext;
				size--;
				delete tmp;
				return data;
			}
			else {
				elem = elem->pNext;
				pos++;
				return popR(ind, elem, pos);
			}
#else 
			if (ind == 0) return PopFront();

			Node* interator = head_;

			for (size_t i = 0; i < ind - 1; i++)
				interator = interator->Next;

			type data = interator->Next->Data;
			Node* tmp = interator->Next;

			interator->Next = interator->Next->Next;
			size_--;
			delete tmp;
			return data;
#endif 
		}
		void Remove();  // TODO find implemetation in git


		LinkedList& operator=(const LinkedList<type>& _arr) {
			if (this == &_arr) return *this;
			Node* tmp = _arr.head_;
			size_ = 0;
			for (size_t i = 0; i < _arr.size_(); i++) {
				PushBack(tmp->Data);
				tmp = tmp->Next;
			}
			return *this;
		}
		friend ostream& operator<<(ostream&, const LinkedList<type>&);
		type& At(unsigned int ind, unsigned int curr = 0, Node* elem = nullptr) {
			assert(ind <= size_ && "Index out of range");
#ifdef RECURSION
			if (ind == 0)
				return head->data;

			if (ind == curr)
				return elem->data;

			if (elem == nullptr) elem = head;

			if (ind != curr) {
				curr++;
				At(ind, curr, elem->pNext);
			}
#else
			int number = 0;
			Node* current = head_;

			while (current != nullptr) {
				if (number == ind)
					return current->Data;

				current = current->Next;
				number++;
			}
			return nullptr;
#endif
		}
		unsigned int getSize() const { return size_; }
		bool empty() const {
			return size_ == 0;
		}
		void clear() {
			while (size_ != 0)
				PopFront();
		}
	};
}
