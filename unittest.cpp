#include <iostream>
#include <random>
#include <algorithm>
#include <tuple>
#include <chrono>
#include <iomanip>
#include "gtest/gtest.h"
#include "HeapList.hpp"
#include "SLHeap.hpp"
#include "BinomialHeap.hpp"
#include "TrustedHeap.hpp"
#include "HeapList.hpp"
#include "testing.hpp"

namespace 
{
	template<typename THeap>
	::testing::AssertionResult 
		Matches(const HeapList<THeap>& heaps, const HeapList<TrustedHeap<long long>>& trusted)
	{
		bool success = true;
		success &= heaps.Size() == trusted.Size();

		for (std::size_t i = 0; i < trusted.Size(); ++i)
		{
			if (!heaps.Empty(i)) success &= heaps.GetMin(i) == trusted.GetMin(i);
		}

		if (success) return ::testing::AssertionSuccess();

		::testing::AssertionResult res = ::testing::AssertionFailure();

		res << "Expected:" << trusted << std::endl;

		res << "But got:" << heaps << std::endl;

		return res;
	}

	class MainTestCase : public ::testing::TestWithParam<std::size_t>
	{
	protected:
		std::default_random_engine engine;
		TestData _testData;

		virtual void SetUp()
		{
			GenerateTest(_testData, GetParam(), engine);
		}
	};

	template<class THeap>
	void CommonHeapTests(const TestData& testData)
	{

		HeapList<THeap> heaps;
		HeapList<TrustedHeap<long long>> trusted;

		for (const auto& op : testData) 
		{
			ASSERT_TRUE(Matches(heaps, trusted)) << testData;

			switch (op.type)
			{
				case AddHeap:
					heaps.AddHeap(op.param1);
					trusted.AddHeap(op.param1);
					break;

				case InsertKey:
					heaps.InsertKey(op.param1, op.param2);
					trusted.InsertKey(op.param1, op.param2);
					break;

				case ExtractMin:
					ASSERT_EQ(trusted.ExtractMin(op.param1), heaps.ExtractMin(op.param1))
						<< "Trusted: " << trusted << std::endl << "Actual: " << heaps << std::endl;
					break;

				case Meld:
					heaps.Meld(op.param1, op.param2);
					trusted.Meld(op.param1, op.param2);
					break;
			}
		}
		ASSERT_TRUE(Matches(heaps, trusted));
	}

	template<class THeap>
	void CommonManualHeapTests()
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

		heaps.ExtractMin(0);
		heaps.ExtractMin(0);
		heaps.ExtractMin(0);
		heaps.ExtractMin(0);
		heaps.ExtractMin(0);

		heaps.InsertKey(0, 8);
		ASSERT_EQ(8, heaps.GetMin(0));
		heaps.ExtractMin(0);

		heaps.InsertKey(0, 3);
		ASSERT_EQ(3, heaps.GetMin(0));
		heaps.ExtractMin(0);
	}

	TEST_P(MainTestCase, TestLeftistHeap)
	{
		CommonManualHeapTests<LeftistHeap<long long>>();
		CommonHeapTests<LeftistHeap<long long>>(_testData);
	}

	TEST_P(MainTestCase, TestScewHeap)
	{
		CommonManualHeapTests<ScewHeap<long long>>();
		CommonHeapTests<ScewHeap<long long>>(_testData);
	}

	TEST_P(MainTestCase, TestBinomialHeap)
	{
		CommonManualHeapTests<BinomialHeap<long long>>();
		CommonHeapTests<BinomialHeap<long long>>(_testData);
	}

	TEST_P(MainTestCase, TestTrustedHeap)
	{
		CommonManualHeapTests<TrustedHeap<long long>>();
	}

	INSTANTIATE_TEST_CASE_P(MainTestCaseInstance, MainTestCase,
                        ::testing::Values(1e3, 1e4));
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
