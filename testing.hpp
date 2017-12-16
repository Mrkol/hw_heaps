#include <vector>
#include <iostream>
#include <random>
#include "HeapList.hpp"
#include "TrustedHeap.hpp"


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

typedef std::vector<Operation> TestData;

std::ostream& operator<<(std::ostream&, 
	const std::vector<Operation>&);

void GenerateTest(TestData&, const std::size_t,	std::default_random_engine&);
