#include <iostream>
#include "gtest/gtest.h"
#include "HeapList.hpp"
#include "BaseHeap.hpp"
#include "SLHeap.hpp"
#include "BinomialHeap.hpp"

namespace 
{
	template<class TDerived>
	void CommonHeapTests()
	{
		TDerived heap1;
		heap1.Insert(4);
		heap1.Insert(10);
		heap1.Insert(3);
		heap1.Insert(2);
		heap1.Insert(6);

		ASSERT_EQ(2, heap1.GetMin()) << heap1;
		ASSERT_EQ(2, heap1.ExtractMin()) << heap1;
		ASSERT_EQ(3, heap1.GetMin()) << heap1;

		TDerived heap2;
		heap2.Insert(11);
		heap2.Insert(-1);
		heap2.Insert(-3);
		heap2.Insert(14);
		ASSERT_EQ(-3, heap2.GetMin()) << heap2;

		heap1.MeldOn(heap2);

		ASSERT_EQ(-3, heap1.GetMin()) << heap1;
		ASSERT_EQ(-3, heap1.ExtractMin()) << heap1;
		ASSERT_EQ(-1, heap1.ExtractMin()) << heap1;
		ASSERT_EQ(3, heap1.ExtractMin()) << heap1;
	}

	TEST(MainTestCase, TestLeftistHeap)
	{
		CommonHeapTests<LeftistHeap<int>>();
	}

	TEST(MainTestCase, TestScewHeap)
	{
		CommonHeapTests<ScewHeap<int>>();
	}

	TEST(MainTestCase, TestBinomialHeap)
	{
		CommonHeapTests<BinomialHeap<int>>();
	}
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
