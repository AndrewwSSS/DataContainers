#pragma once
#include <type_traits>
#include <iostream>
#include <queue>


namespace DataContainers {
	template<typename type1, typename type2, typename = std::enable_if_t<std::is_arithmetic_v<type1>>>
	class Pair
	{
	public:
		type1 Key;
		type2 Value;

		Pair() {
			
		}

		Pair(type1 key, type2 value) {
			this->Key = key;	
			this->Value = value;
		}
	};

	template<typename type1, typename type2>
	class Dictionary {
	private:
		class Node {
		public:
			Node* Left;
			Node* Right;
			Node* Parent;
			unsigned int Height;
			Pair<type1, type2> Data;

			Node() {
				Left = nullptr;
				Right = nullptr;
				Parent = nullptr;
				Height = 0;
			}

			Node(Pair<type1, type2> data, Node* parent = nullptr) : Node() {
				Data = data;
				Parent = parent;
			}

			void UpdateHeight(Node* node = nullptr) {
				if (node == nullptr)
					node = this;

				int leftHeight = GetHeight(node->Left);
				int rightHeight = GetHeight(node->Right);

				if (leftHeight > rightHeight)
					node->Height = leftHeight + 1;
				else
					node->Height = rightHeight + 1;

				if (node->Parent != nullptr)
					UpdateHeight(node->Parent);
	
				Balance(node);
			}

			int GetHeight(Node* node) {
				return node == nullptr ? -1 : node->Height;
			}

			int GetBalance() {
				int leftHeight = GetHeight(Left);
				int rightHeight = GetHeight(Right);

				return  rightHeight - leftHeight;
			}

			static void Swap(Node* first, Node*second) {
				std::swap(first->Data, second->Data);
				std::swap(first->Height, second->Height);
			}

			void RightRotate(Node* node = nullptr) {
				if (!node)
					node = this;

				Node::Swap(node, node->Left);
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

			/*	if(node->Right->Right != nullptr)
					node->Right->Right->UpdateHeight();*/

				if (node->Right != nullptr)
					node->Right->UpdateHeight();

				/*if(node->Right->Left != nullptr)
					node->Right->Left->UpdateHeight();*/
	
				
			}

			void LeftRotate(Node* node = nullptr) {
				if (!node)
					node = this;

				Node::Swap(node, node->Right);
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
					node->Left->UpdateHeight();
			}

			void Balance(Node* node = nullptr) {
				if (!node)
					node = this;
				const int balance = node->GetBalance();
				if(balance <= -2) {
					if (node->Left->GetBalance() == 1)
						node->Left->LeftRotate();
					node->RightRotate();
					
				}else if(balance >= 2) {
					if (node->Right->GetBalance() == -1)
						node->Right->RightRotate();

					node->LeftRotate();
				}
				
			}

			
		};

		Node* root;
		unsigned int size;
		unsigned int spacing = 3;

		void printSpace(double n, Node* node) {

			while(n>0) {
				cout << "\t";
				n--;
			}

			if (!node)
				cout << " ";
			else
				cout << node->Data.Key << ": " << node->Height;
		}
	public:
		Dictionary() {
			root = nullptr;
			size = 0;
		}

		~Dictionary() {
			Clear();
		}

		void Insert(Pair<type1, type2> pair, Node* node = nullptr) {
			if(!root) {
				Node* newNode = new Node(pair);
				root = newNode;
				size++;

				return;
			}

			if (node == nullptr)
				node = root;

			if(pair.Key < node->Data.Key) {
				if (node->Left == nullptr)
				{
					node->Left = new Node(pair, node);
					node->Left->UpdateHeight();
					PrintFormat();
					cout << "\n\n\n\n\n";
					size++;
				}
				else
					return Insert(pair, node->Left);
			}
			else {
				if (node->Right == nullptr)
				{
					node->Right = new Node(pair, node);
					node->Right->UpdateHeight();
					PrintFormat();
					cout << "\n\n\n\n\n";
					size++;
				}
				else
					return Insert(pair, node->Right);
			}
		}

		Node* GetMin(Node* node = nullptr) {
			if (!node)
				node = root;
			if (!node->Left)
				return node;
			return GetMin(node->Left);
		}

		Node* GetMax(Node* node = nullptr) {
			if (!node)
				node = root;
			if (!node->Right)
				return node;
			return GetMax(node->Right);
		}

		void Delete(const type1& key, Node* node = nullptr) {
			if (!node)
				node = root;

			if (key < node->Data.Key)
				Delete(key, node->Left);
			else if (key > node->Data.Key)
				Delete(key, node->Right);
			else {
				if (node->Left != nullptr && node->Right != nullptr) {
					Node* maxLeft = GetMax(node->Left);

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

					if (node == root)
						return;

					if(node->Parent->Left == node)
						node->Parent->Left = maxLeft;
					else
						node->Parent->Right = maxLeft;

					if(parent != node)
						parent->UpdateHeight();

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

						node->Parent->UpdateHeight();
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
						if(node == root)
							root = child;

						child->UpdateHeight();
						delete node;
					}
				}
				size--;
			}
			
		}

		void Clear(Node* node = nullptr) {
			if (!node)
				node = root;

			if(node->Left != nullptr)
				Clear(node->Left);

			if(node->Right != nullptr)
				Clear(node->Right);

			Delete(node->Data.Key, node);
		}

		void ForEach(const function<void(type2 i)>& function, Node* node = nullptr) {
			if (!node)
				node = root;

			if(node->Left != nullptr)
				ForEach(function, node->Left);

			function(node->Data.Value);

			if(node->Right != nullptr)
				ForEach(function, node->Right);
		}

		unsigned int Size() const {
			return size;
		}

		bool Contains(const type1& key, Node* node = nullptr) const {
			if (!node)
				node = root;

			if(node->Left && Contains(key, node->Left))
				return true;

			if (node->Right && Contains(key, node->Right))
				return true;

			return key == node->Data.Key;
		}

		void PrintFormat(Node* node = nullptr) {
			if (!node)
				node = this->root;

			queue<Node*> treeLevel, temp;
			treeLevel.push(node);
			int counter = 0;
			int height = node->Height+1;

			//double elementsCount = pow(2, (height + 1)) - 1;
			int elementsCount = this->size+spacing;

			while (counter <= height) {
				Node* removed = treeLevel.front();
				treeLevel.pop();

				if (temp.empty()) {
					printSpace(elementsCount / pow(2, counter + 1),
						removed);
				}
				else {
					printSpace(elementsCount / pow(2, counter),
						removed);
				}

				if (removed == nullptr) {
					temp.push(nullptr);
					temp.push(nullptr);
				}
				else {
					temp.push(removed->Left);
					temp.push(removed->Right);
				}

				if (treeLevel.empty()) {
					cout << endl << endl;
					treeLevel = temp;
					while (!temp.empty())
						temp.pop();
					
					counter++;
				}
			}
		}

		Vector<type1> Keys() const {
			auto result = Vector<type1>(size);
			FindKeys(result, root);
			return result;
			
		}

		void FindKeys(Vector<type1>& vector, Node* node = nullptr) const {
			if (!node)
				node = root;

			if (node->Left)
				FindKeys(vector, node->Left);

			vector.PushBack(node->Data.Key);

			if (node->Right)
				FindKeys(vector, node->Right);
		}

		Vector<Pair<type1, type2>> Filter(const function<bool(Pair<type1, type2>)>& predicate) {
			auto result = Vector<Pair<type1, type2>>(size);
			Foo(result, predicate, root);
			return result;
		}

		void Foo(Vector<Pair<type1, type2>>& vector,
				 const function<bool(Pair<type1, type2>)>& predicate,
				 Node* node = nullptr) {
			if (!node)
				node = root;

			if (node->Left)
				Foo(vector, predicate, node->Left);

			if(predicate(node->Data))
				vector.PushBack(node->Data);

			if (node->Right)
				Foo(vector, predicate, node->Right);
		}

	};
}
