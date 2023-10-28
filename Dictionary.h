#pragma once
#include <type_traits>
#include <iostream>
#include <queue>
#include <format>
#include "Vector.h"
#include <functional>
using namespace std;
namespace DataContainers {
	template<typename K, typename V, typename = std::enable_if_t<std::is_arithmetic_v<K>>>
	class Pair
	{
	public:
		K Key;
		V Value;

		Pair(const K& key, const V& value) {
			this->Key = key;	
			this->Value = value;
		}

		Pair() { }
	};

	template<typename K, typename V>
	class Dictionary {
	private:
		class Node {
		public:
			Node* Left;
			Node* Right;
			Node* Parent;
			uint32_t Height;
			Pair<K, V> Data;

			Node() {
				Left = nullptr;
				Right = nullptr;
				Parent = nullptr;
				Height = 0;
			}

			Node(Pair<K, V> data, Node* parent = nullptr) : Node() {
				Data = data;
				Parent = parent;
			}

			void updateHeight(Node* node = nullptr) {
				if (node == nullptr)
					node = this;

				int leftHeight = getHeight(node->Left);
				int rightHeight = getHeight(node->Right);

				if (leftHeight > rightHeight)
					node->Height = leftHeight + 1;
				else
					node->Height = rightHeight + 1;

				if (node->Parent != nullptr)
					updateHeight(node->Parent);
	
				balance(node);
			}

			int getHeight(Node* node) {
				return node == nullptr ? -1 : node->Height;
			}

			int getBalance() {
				int leftHeight = getHeight(Left);
				int rightHeight = getHeight(Right);

				return  rightHeight - leftHeight;
			}

			static void swap(Node* first, Node*second) {
				std::swap(first->Data, second->Data);
				std::swap(first->Height, second->Height);
			}

			void rightRotate(Node* node = nullptr) {
				if (!node)
					node = this;

				Node::swap(node, node->Left);
				auto nodeRight = node->Right;

				node->Right = node->Left;
				node->Right->Parent = node;

				node->Left = node->Right->Left;
				if (node->Left != nullptr)
					node->Left->Parent = node;

				node->Right->Left = node->Right->Right;
				node->Right->Right = nodeRight;

				if (node->Right->Right != nullptr)
					node->Right->Right->Parent = node->Right;

				if (node->Right != nullptr)
					node->Right->updateHeight();
			}

			void leftRotate(Node* node = nullptr) {
				if (!node)
					node = this;

				Node::swap(node, node->Right);
				auto nodeRight = node->Left;

				node->Left = node->Right;
				node->Left->Parent = node;

				node->Right = node->Left->Right;
				if(node->Right != nullptr)
					node->Right->Parent = node;
				 
				node->Left->Right = node->Left->Left;
				node->Left->Left = nodeRight;

				if (node->Left->Left != nullptr)
					node->Left->Left->Parent = node->Left;

				if (node->Left != nullptr)
					node->Left->updateHeight();
			}

			void balance(Node* node = nullptr) {
				if (!node)
					node = this;
				const int balance = node->getBalance();
				if(balance <= -2) {
					if (node->Left->getBalance() == 1)
						node->Left->leftRotate();
					node->rightRotate();
					
				}else if(balance >= 2) {
					if (node->Right->getBalance() == -1)
						node->Right->rightRotate();

					node->leftRotate();
				}
				
			}

			
		};

		Node* root_;
		uint32_t size_;
	
		Node* getMin(Node* node = nullptr) {
			if (!node)
				node = root_;
			if (!node->Left)
				return node;
			return getMin(node->Left);
		}
		Node* getMax(Node* node = nullptr) {
			if (!node)
				node = root_;
			if (!node->Right)
				return node;
			return getMax(node->Right);
		}

		void filter(Vector<Pair<K, V>>& vector,
					const function<bool(Pair<K, V>)>& predicate,
					Node* node = nullptr) {
			if (!node)
				node = root_;

			if (node->Left)
				filter(vector, predicate, node->Left);

			if (predicate(node->Data))
				vector.pushBack(node->Data);

			if (node->Right)
				filter(vector, predicate, node->Right);
		}

		void forEach(Vector<K>& vector, Node* node = nullptr) const {
			if (!node)
				node = root_;

			if (node->Left)
				forEach(vector, node->Left);

			vector.pushBack(node->Data.Key);

			if (node->Right)
				forEach(vector, node->Right);
		}
	public:
		class DictionaryIterator {
		private:
			Vector<Node*> nodes_;

			void searchNodes(Vector<Node*>& vector, Node* node) const {
				if (node == nullptr)
					return;

				if (node->Left)
					searchNodes(vector, node->Left);

				vector.pushBack(node);

				if (node->Right)
					searchNodes(vector, node->Right);
			}
		public:
			uint32_t position;

			DictionaryIterator(Node* head) {
				searchNodes(nodes_, head);
				position = 0;
			}
			DictionaryIterator(uint32_t size) {
				position = size;
			}

			void operator++() {
				position += 1;
			}
			bool operator!=(const DictionaryIterator& iterator) const {
				return position != iterator.position;
			}
			Pair<K, V>& operator*() {
				return nodes_[position]->Data;
			}
		};

		Dictionary() {
			root_ = nullptr;
			size_ = 0;
		}
		~Dictionary() {
			clear();
		}

		void insert(K key, V value, Node* node = nullptr) {
			if(!root_) {
				Node* newNode = new Node(Pair<K, V>(key, value));
				root_ = newNode;
				size_++;
				return;
			}

			if (node == nullptr)
				node = root_;

			if(key < node->Data.Key) {
				if (node->Left == nullptr) {
					node->Left = new Node(Pair<K, V>(key, value), node);
					node->Left->updateHeight();
					size_++;
				}
				else
					return insert(key, value, node->Left);
			}
			else {
				if (node->Right == nullptr) {
					node->Right = new Node(Pair<K, V>(key, value), node);
					node->Right->updateHeight();
					size_++;


				}
				else
					return insert(key, value, node->Right);
			}
		}
		void erase(const K& key, Node* node = nullptr) {
			if (!node)
				node = root_;

			if (key < node->Data.Key)
				erase(key, node->Left);
			else if (key > node->Data.Key)
				erase(key, node->Right);
			else {
				if (node->Left != nullptr && node->Right != nullptr) {
					Node* maxLeft = getMax(node->Left);

					auto parent = maxLeft->Parent;
	
					if (parent->Left == node || parent->Left == maxLeft)
						parent->Left = nullptr;
					else
						node->Left->Parent = maxLeft;

					if (parent->Right == node || parent->Right == maxLeft)
						parent->Right = nullptr;
					else
						node->Right->Parent = maxLeft;

					maxLeft->Parent = node->Parent;
					maxLeft->Right = node->Right;
					maxLeft->Left = node->Left;
					maxLeft->Height = node->Height;

					if (node == root_)
						return;

					if(node->Parent->Left == node)
						node->Parent->Left = maxLeft;
					else
						node->Parent->Right = maxLeft;

					if(parent != node)
						parent->updateHeight();

					delete node;

				}
				else {
					auto child
							= (node->Left == nullptr) ? node->Right : node->Left;

					if(child == nullptr) {
						if(node->Parent == nullptr) {
							delete node;
							return;
						}

						if(node->Parent->Right == node)
							node->Parent->Right = nullptr;
						else
							node->Parent->Left = nullptr;

						node->Parent->updateHeight();
						delete node;
						node = nullptr;
					}
					else {
						child->Parent = node->Parent;
						child->Height = node->Height;
						if(node->Parent != nullptr) {
							if (node->Parent->Right == node)
								node->Parent->Right = child;
							else
								node->Parent->Left = child;
						}
						if(node == root_)
							root_ = child;

						child->updateHeight();
						delete node;
					}
				}
				size_--;
			}
			
		}
		void clear(Node* node = nullptr) {
			if (!node)
				node = root_;

			if(node->Left != nullptr)
				clear(node->Left);

			if(node->Right != nullptr)
				clear(node->Right);

			erase(node->Data.Key, node);
		}

		void forEach(const std::function<void(V& i)>& function, Node* node = nullptr) {
			if (!node)
				node = root_;

			if(node->Left != nullptr)
				forEach(function, node->Left);

			function(node->Data.Value);

			if(node->Right != nullptr)
				forEach(function, node->Right);
		}
		Vector<Pair<K, V>> filter(const std::function<bool(Pair<K, V>)>& predicate) const {
			auto result = Vector<Pair<K, V>>(size_);
			filter(result, predicate, root_);
			return result;
		}

		uint32_t size() const {
			return size_;
		}
		Vector<K> keys() const {
			auto result = Vector<K>(size_);
			forEach(result, root_);
			return result;
			
		}
		bool contains(const K& key, Node* node = nullptr) const {
			if (!node)
				node = root_;

			if(key == node->Data.Key)
				return true;

			if (node->Left && contains(key, node->Left))
				return true;

			if (node->Right && contains(key, node->Right))
				return true;

			return false;

		}
		V& At(const K& key, Node* node = nullptr) {
			if (node == nullptr)
				node = root_;
			if (key == node->Data.Key)
				return node->Data.Value;
	
			if (key < node->Data.Key)
				return At(key, node->Left);
			if (node->Right)
				return At(key, node->Right);
			throw std::invalid_argument("invalid key");
		

		}
		V& operator[](K key) {
			return At(key);
		}   

		// Iteration methods
		DictionaryIterator begin() const {
			return root_;
		}
		DictionaryIterator end() const {
			return size_;
		}
	};

	
}

		