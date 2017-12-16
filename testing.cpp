#include "testing.hpp"


std::ostream& operator<<(std::ostream& out, 
	const std::vector<Operation>& ops)
{
	out << "{" << std::endl;
	for (auto op : ops)
	{
		out << "\t";
		out << "(" << op.type << ", " << op.param1 << ", " << op.param2 << ")";
		out << std::endl;
	}
	out << "}";
	return out;
}

void GenerateTest(TestData& testData, const std::size_t size, 
	std::default_random_engine& engine)
{
	std::vector<long long> _valueData;
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
