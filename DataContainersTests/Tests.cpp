#include "pch.h"
#include "../../DataContainers/Vector.h"
#include "../../DataContainers/List.h"
#include "../../DataContainers/LinkedList.h"

using namespace DataContainers;

TEST(VectorTest, ReadData) {
	const Vector<uint32_t> vector = { 1,2, 3, 4 };
	EXPECT_EQ(vector[0], 1);
	EXPECT_EQ(vector[1], 2);
	EXPECT_EQ(vector[2], 3);
	EXPECT_EQ(vector[3], 4);
	EXPECT_EQ(vector.size(), 4);
}

TEST(VectorTest, InsertData) {
	Vector<uint32_t> vector;

	for (uint32_t i = 0; i < 10; i++)
		vector.append(i);
	
	EXPECT_EQ(vector.size(), 10);
}

TEST(ListTest, Iteration) {
	List<uint32_t> list;

	for (uint32_t i = 0; i < 10; i++)
		list.pushBack(i);
	
	    
	int i = 0;
	for(const auto& elem : list) {
		EXPECT_EQ(elem, i);
		i++;
	}
}

TEST(LinkedListTest, Iteration) {
	LinkedList<uint32_t> list;

	for (uint32_t i = 0; i < 10; i++)
		list.PushBack(i);
	

	uint32_t i = 0;
	for (const auto& elem : list) {
		EXPECT_EQ(elem, i);
		i++;
	}
}