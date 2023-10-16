#include "pch.h"
#include "../../DataContainers/Vector.h"
using namespace DataContainers;

TEST(VectorTests, ReadData) {
	Vector<int> vector = { 1,2, 3, 4 };
	EXPECT_EQ(vector[0], 1);
	EXPECT_EQ(vector[1], 2);
	EXPECT_EQ(vector[2], 3);
	EXPECT_EQ(vector[3], 4);
	EXPECT_EQ(vector.Size(), 4);
}

TEST(VectorTests, InsertData) {
	Vector<int> vector;
	for (size_t i = 0; i < 10; i++) {
		vector.Append(i);
	}
	EXPECT_EQ(vector.Size(), 10);
}

TEST(VectorTests, DeleteData)
{
	
}