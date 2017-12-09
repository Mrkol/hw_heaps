#ifndef HEAP_LIST_HPP
#define HEAP_LIST_HPP

#include <cstddef>
#include <vector>


template<class THeap>
class HeapList;

template<class THeap>
std::ostream& operator<<(std::ostream&, const HeapList<THeap>&);

template<class THeap>
class HeapList
{
public:
	using IndexType = std::size_t;
	using KeyType = typename THeap::KeyType;
	using KeyReference = typename THeap::KeyReference;
	using KeyConstReference = typename THeap::KeyConstReference;

	void AddHeap(KeyType);
	
	void InsertKey(IndexType, KeyType);

	KeyConstReference GetMin(IndexType) const;
	
	KeyType ExtractMin(IndexType);

	IndexType Size() const;

	std::size_t Empty(IndexType) const;
	
	void Meld(IndexType, IndexType);

	friend std::ostream& operator<<<>(std::ostream&, const HeapList<THeap>&);

private:
	std::vector<THeap> _heaps;
};

#include "HeapList.tpp"

#endif //HEAP_LIST_HPP
