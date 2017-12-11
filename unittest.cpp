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

namespace 
{
	template<typename THeap>
	::testing::AssertionResult 
		Matches(const HeapList<THeap>& heaps, const HeapList<TrustedHeap<long long>>& trusted)
	{
		bool success = true;
		success &= heaps.Size() == trusted.Size();

		for (size_t i = 0; i < trusted.Size(); ++i)
		{
			if (!heaps.Empty(i)) success &= heaps.GetMin(i) == trusted.GetMin(i);
		}

		if (success) return ::testing::AssertionSuccess();

		::testing::AssertionResult res = ::testing::AssertionFailure();

		res << "Expected:" << trusted << std::endl;

		res << "But got:" << heaps << std::endl;

		return res;
	}


	enum OpType
	{
		AddHeap,
		InsertKey,
		ExtractMin,
		Meld
	};

	class MainTestCase : public ::testing::TestWithParam<std::size_t>
	{
	protected:
		std::default_random_engine engine;

		virtual void SetUp()
		{
			std::size_t SIZE = GetParam();

			_valueData.reserve(SIZE);
			for (std::size_t i = 0; i < SIZE; ++i) _valueData.push_back((long long)i);
			std::shuffle(_valueData.begin(), _valueData.end(), engine);

			auto it = _valueData.cbegin();

			_testData.reserve(SIZE);
	
			std::uniform_int_distribution<int> type(0, 3);

			HeapList<TrustedHeap<long long>> trusted;

			for (size_t i = 0; i < SIZE; ++i) 
			{
				OpType optype = (OpType) type(engine);

				if (optype == AddHeap)
				{
					long long k = *it++;
					trusted.AddHeap(k);
					_testData.push_back(std::make_tuple(optype, k, 0));
					continue;
				}

				if (trusted.Size() == 0) continue;
				std::uniform_int_distribution<long long> index(0, trusted.Size() - 1);
				
				if (optype == InsertKey)
				{
					long long k = *it++;
					long long i = index(engine);
					trusted.InsertKey(i, k);
					_testData.push_back(std::make_tuple(optype, i, k));
					continue;
				}

				if (optype == ExtractMin)
				{
					long long i = index(engine);
					if (trusted.Empty(i)) continue;
					trusted.ExtractMin(i);
					_testData.push_back(std::make_tuple(optype, i, 0));
					continue;
				}

				if (optype == Meld)
				{
					long long i = index(engine);
					long long j = index(engine);
					if (i == j) continue;
					trusted.Meld(i, j);
					_testData.push_back(std::make_tuple(optype, i, j));
					continue;
				}
			}
		}

		std::vector<long long> _valueData;
		std::vector<std::tuple<OpType, long long, long long>> _testData;
	};

	std::ostream& operator<<(std::ostream& out, 
		const std::vector<std::tuple<OpType, long long, long long>>& vec)
	{
		out << "{" << std::endl;
		for (auto tup : vec)
		{
			out << "\t";
			out << "(" << std::get<0>(tup) << ", " << std::get<1>(tup) << ", " << std::get<2>(tup) << ")";
			out << std::endl;
		}
		out << "}";
		return out;
	}

	template<class THeap>
	void CommonHeapTests(const std::vector<std::tuple<OpType, long long, long long>>& testData)
	{

		HeapList<THeap> heaps;
		HeapList<TrustedHeap<long long>> trusted;

		for (const auto& op : testData) 
		{
			EXPECT_TRUE(Matches(heaps, trusted)) << testData;

			OpType optype = std::get<0>(op);
			long long param1 = std::get<1>(op);
			long long param2 = std::get<2>(op);

			switch (optype)
			{
				case AddHeap:
					heaps.AddHeap(param1);
					trusted.AddHeap(param1);
					break;

				case InsertKey:
					heaps.InsertKey(param1, param2);
					trusted.InsertKey(param1, param2);
					break;

				case ExtractMin:
					ASSERT_EQ(trusted.ExtractMin(param1), heaps.ExtractMin(param1))
						<< "Trusted: " << trusted << std::endl << "Actual: " << heaps << std::endl;
					break;

				case Meld:
					heaps.Meld(param1, param2);
					trusted.Meld(param1, param2);
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
                        ::testing::Values(1e3, 1e4, 1e5));
}


int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
