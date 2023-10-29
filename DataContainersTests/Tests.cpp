#include "pch.h"
#include "../../DataContainers/Vector.h"
#include "../../DataContainers/List.h"
#include "../../DataContainers/LinkedList.h"
#include "../../DataContainers/String.h"
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


bool EqualStrings(const char* first, const char* second) {
	return strcmp(first, second) == 0;
}

TEST(StringTest, operators) {
	String str1("Hello");
	String str2("-Hello");

	String tmp = str1 + str2;
	EXPECT_TRUE(EqualStrings(tmp.cStr(), "Hello-Hello"));
	EXPECT_TRUE(EqualStrings(str1 + "Hi", "HelloHi"));
}

TEST(StringTest, functions) {
	String str1("Hello");
	String str2("World!");
	EXPECT_TRUE(EqualStrings(str1.append(" ").append(str2), "Hello World!"));
	EXPECT_TRUE(EqualStrings(str2.reverse(), "!dlroW"));
}
