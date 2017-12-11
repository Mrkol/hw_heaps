#include <iostream>
#include <vector>
#include <tuple>
#include <chrono>
#include <algorithm>
#include <random>
#include "HeapList.hpp"
#include "SLHeap.hpp"
#include "BinomialHeap.hpp"
#include "TrustedHeap.hpp"

std::default_random_engine engine;

enum OpType
{
	AddHeap,
	InsertKey,
	ExtractMin,
	Meld
};

using TestData_t = std::vector<std::tuple<OpType, long long, long long>>;

void GenerateTest(TestData_t& testData, const std::size_t size)
{
	std::vector<long long> _valueData;

	_valueData.reserve(size);
	for (std::size_t i = 0; i < size; ++i) _valueData.push_back((long long)i);
	std::shuffle(_valueData.begin(), _valueData.end(), engine);

	auto it = _valueData.cbegin();

	testData.reserve(size);

	std::uniform_int_distribution<int> type(0, 3);

	HeapList<TrustedHeap<long long>> trusted;

	while (testData.size() < size) 
	{
		OpType optype = (OpType) type(engine);

		if (optype == AddHeap)
		{
			long long k = *it++;
			trusted.AddHeap(k);
			testData.push_back(std::make_tuple(optype, k, 0));
			continue;
		}

		if (trusted.Size() == 0) continue;
		std::uniform_int_distribution<long long> index(0, trusted.Size() - 1);
		
		if (optype == InsertKey)
		{
			long long k = *it++;
			long long i = index(engine);
			trusted.InsertKey(i, k);
			testData.push_back(std::make_tuple(optype, i, k));
			continue;
		}

		if (optype == ExtractMin)
		{
			long long i = index(engine);
			if (trusted.Empty(i)) continue;
			trusted.ExtractMin(i);
			testData.push_back(std::make_tuple(optype, i, 0));
			continue;
		}

		if (optype == Meld)
		{
			long long i = index(engine);
			long long j = index(engine);
			if (i == j) continue;
			trusted.Meld(i, j);
			testData.push_back(std::make_tuple(optype, i, j));
			continue;
		}
	}
}

template<class THeap>
void SpeedTest(const std::vector<std::tuple<OpType, long long, long long>>& testData)
{
	using clock_t = std::chrono::high_resolution_clock;
	using timepoint_t = std::chrono::time_point<clock_t>;
	using ms_t = std::chrono::microseconds;

	std::cout << "Testing over " << testData.size() << " operations..." << std::endl;

	timepoint_t start(clock_t::now());

	HeapList<THeap> heaps;

	for (const auto& op : testData) 
	{
		OpType optype = std::get<0>(op);
		long long param1 = std::get<1>(op);
		long long param2 = std::get<2>(op);

		switch (optype)
		{
			case AddHeap:
				heaps.AddHeap(param1);
				break;

			case InsertKey:
				heaps.InsertKey(param1, param2);
				break;

			case ExtractMin:
				heaps.ExtractMin(param1);
				break;

			case Meld:
				heaps.Meld(param1, param2);
				break;
		}
	}
	heaps.Size();

	timepoint_t end(clock_t::now());

	long long elapsed = std::chrono::duration_cast<ms_t>(end - start).count();

	std::cout << "Finished. Elapsed time: " << elapsed << "ms" << std::endl;
}

int main()
{
	long long sizes[] {1000ll, 10000ll, 100000ll, 1000000ll, 10000000ll};
	for (auto size : sizes)
	{
		TestData_t data;
		GenerateTest(data, size);
		std::cout << "[ScewHeap] ";
		SpeedTest<ScewHeap<long long>>(data);
		std::cout << "[LeftistHeap] ";
		SpeedTest<ScewHeap<long long>>(data);
		std::cout << "[BinomialHeap] ";
		SpeedTest<ScewHeap<long long>>(data);
		std::cout << std::endl;
	}
}
