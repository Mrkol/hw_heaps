#include <iostream>
#include <tuple>
#include <chrono>
#include "HeapList.hpp"
#include "SLHeap.hpp"
#include "BinomialHeap.hpp"
#include "TrustedHeap.hpp"
#include "testing.hpp"

template<class THeap>
void SpeedTest(const TestData& testData)
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
	std::default_random_engine engine;
	std::size_t sizes[] {1000, 10000, 100000};
	for (auto size : sizes)
	{
		TestData data;
		GenerateTest(data, size, engine);
		std::cout << "[ScewHeap] ";
		SpeedTest<ScewHeap<long long>>(data);
		std::cout << "[LeftistHeap] ";
		SpeedTest<LeftistHeap<long long>>(data);
		std::cout << "[BinomialHeap] ";
		SpeedTest<BinomialHeap<long long>>(data);
		std::cout << std::endl;
	}
}
