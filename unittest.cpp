#include <iostream>
#include <random>
#include "gtest/gtest.h"
#include "HeapList.hpp"
#include "SLHeap.hpp"
#include "BinomialHeap.hpp"
#include "TrustedHeap.hpp"
#include "HeapList.hpp"

namespace 
{
	std::default_random_engine engine;

	template<typename THeap>
	::testing::AssertionResult 
		Matches(const HeapList<THeap>& heaps, const HeapList<TrustedHeap<int>>& trusted)
	{
		bool success = true;
		success &= heaps.Size() == trusted.Size();

		for (size_t i = 0; i < trusted.Size(); ++i)
		{
			if (!heaps.Empty(i)) success &= heaps.GetMin(i) == trusted.GetMin(i);
		}

		if (success) return ::testing::AssertionSuccess();

		::testing::AssertionResult res = ::testing::AssertionFailure();

		res << "Expected:" << heaps << std::endl;

		res << "But got:" << trusted << std::endl;

		return res;
	}

	template<class THeap>
	void CommonHeapTests()
	{
		HeapList<THeap> heaps;

		heaps.AddHeap(10);
		heaps.InsertKey(0, -1);
		heaps.InsertKey(0, 4);
		heaps.InsertKey(0, 8);

		ASSERT_EQ(-1, heaps.GetMin(0));
		ASSERT_EQ(-1, heaps.ExtractMin(0));
		ASSERT_EQ(4, heaps.GetMin(0));

		heaps.AddHeap(-2);
		heaps.InsertKey(1, 13);
		heaps.InsertKey(1, -1);
		heaps.InsertKey(1, 5);

		heaps.Meld(0, 1);

		ASSERT_EQ(-2, heaps.GetMin(0));
		heaps.ExtractMin(0);
		ASSERT_EQ(-1, heaps.GetMin(0));
	}

	template<class THeap>
	void CommonRandomizedHeapTests() //this, ofcourse, expects an int THeap
	{
		const size_t COUNT = 50;
		const int RANGE = 10;

		HeapList<THeap> heaps;
		HeapList<TrustedHeap<int>> trusted;

		std::uniform_int_distribution<int> type(0, 3);
		std::uniform_int_distribution<int> value(-RANGE, RANGE);

		for (size_t i = 0; i < COUNT; ++i) 
		{
			EXPECT_TRUE(Matches(heaps, trusted));

			int optype = type(engine);

			if (optype == 0)
			{
				int k = value(engine);
				heaps.AddHeap(k);
				trusted.AddHeap(k);
				continue;
			}

			if (trusted.Size() == 0) continue;
			std::uniform_int_distribution<int> index(0, trusted.Size() - 1);
			
			if (optype == 1)
			{
				int k = value(engine);
				int i = index(engine);
				if (trusted.Contains(i, k)) continue;
				heaps.InsertKey(i, k);
				trusted.InsertKey(i, k);
				continue;
			}

			if (optype == 2)
			{
				int i = index(engine);
				if (trusted.Empty(i)) break;
				ASSERT_EQ(trusted.ExtractMin(i), heaps.ExtractMin(i))
					<< "Trusted: " << trusted << std::endl << "Actual: " << heaps << std::endl;
				continue;
			}

			if (optype == 3)
			{
				int i = index(engine);
				int j = index(engine);
				heaps.Meld(i, j);
				trusted.Meld(i, j);
				continue;
			}
		}
		EXPECT_TRUE(Matches(heaps, trusted));
	}

	TEST(MainTestCase, TestLeftistHeap)
	{
		CommonHeapTests<LeftistHeap<int>>();
		CommonRandomizedHeapTests<LeftistHeap<int>>();
	}

	TEST(MainTestCase, TestScewHeap)
	{
		CommonHeapTests<ScewHeap<int>>();
		CommonRandomizedHeapTests<ScewHeap<int>>();
	}

	TEST(MainTestCase, TestBinomialHeap)
	{
		CommonHeapTests<BinomialHeap<int>>();
		CommonRandomizedHeapTests<BinomialHeap<int>>();
	}

	TEST(MainTestCase, TestTrustedHeap)
	{
		CommonHeapTests<TrustedHeap<int>>();
	}
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
