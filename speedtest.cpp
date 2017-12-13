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

struct Operation
{
	OpType type;
	long long param1;
	long long param2;
};

using TestData_t = std::vector<Operation>;

void GenerateTest(TestData_t& testData, const std::size_t size)
{
	std::vector<int> _valueData;
	_valueData.reserve(size);
	for (std::size_t i = 0; i < size; ++i) _valueData.push_back((long long)i);
	std::shuffle(_valueData.begin(), _valueData.end(), engine);

	auto it = _valueData.cbegin();

	testData.reserve(size);

	std::discrete_distribution<int> type({10, 10, 5, 5});

	HeapList<TrustedHeap<long long>> trusted;

	while (testData.size() < size) 
	{
		OpType optype = (OpType) type(engine);

		if (optype == AddHeap)
		{
			long long k = *it++;
			trusted.AddHeap(k);
			testData.push_back({optype, k, 0});
			continue;
		}

		if (trusted.Size() == 0) continue;
		std::uniform_int_distribution<long long> index(0, trusted.Size() - 1);
		
		if (optype == InsertKey)
		{
			long long k = *it++;
			long long i = index(engine);
			trusted.InsertKey(i, k);
			testData.push_back({optype, i, k});
			continue;
		}

		if (optype == ExtractMin)
		{
			long long i = index(engine);
			if (trusted.Empty(i)) continue;
			trusted.ExtractMin(i);
			testData.push_back({optype, i, 0});
			continue;
		}

		if (optype == Meld)
		{
			long long i = index(engine);
			long long j = index(engine);
			if (i == j) continue;
			trusted.Meld(i, j);
			testData.push_back({optype, i, j});
			continue;
		}
	}
}

template<class THeap>
void SpeedTest(const TestData_t& testData)
{
	using clock_t = std::chrono::high_resolution_clock;
	using timepoint_t = std::chrono::time_point<clock_t>;
	using ms_t = std::chrono::microseconds;

	std::cout << "Testing over " << testData.size() << " operations..." << std::endl;

	timepoint_t start(clock_t::now());

	HeapList<THeap> heaps;

	for (const auto& op : testData) 
	{
		switch (op.type)
		{
			case AddHeap:
				heaps.AddHeap(op.param1);
				break;

			case InsertKey:
				heaps.InsertKey(op.param1, op.param2);
				break;

			case ExtractMin:
				heaps.ExtractMin(op.param1);
				break;

			case Meld:
				heaps.Meld(op.param1, op.param2);
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
	long long sizes[] {1000ll, 10000ll, 100000ll};
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
