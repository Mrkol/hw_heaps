#include "HeapList.hpp" //this is needed for autocompletion and static analysis to work :/

template<class THeap>
void HeapList<THeap>::AddHeap(KeyType key)
{
	THeap heap;
	heap.Insert(key);
	_heaps.push_back(heap);
}
	
template<class THeap>
void HeapList<THeap>::InsertKey(IndexType index, KeyType key)
{
	_heaps[index].Insert(key);
}

template<class THeap>
typename HeapList<THeap>::KeyConstReference HeapList<THeap>::GetMin(IndexType index) const
{
	return _heaps[index].GetMin();
}
	
template<class THeap>
typename HeapList<THeap>::KeyType HeapList<THeap>::ExtractMin(IndexType index)
{
	return _heaps[index].ExtractMin();
}

template<class THeap>
void HeapList<THeap>::Meld(IndexType first, IndexType second)
{
	_heaps[first].MeldOn(_heaps[second]);
	_heaps.erase(_heaps.begin() + second);
}

template<class THeap>
bool HeapList<THeap>::Contains(IndexType index, KeyConstReference key) const
{
	return _heaps[index].Contains(key);
}

template<class THeap>
typename HeapList<THeap>::IndexType HeapList<THeap>::Size() const
{
	return _heaps.size();
}

template<class THeap>
std::size_t HeapList<THeap>::Empty(IndexType index) const
{
	return _heaps[index].Empty();
}

template<class THeap>
std::ostream& operator<<(std::ostream& out, const HeapList<THeap>& heaps)
{
	for (auto& heap : heaps._heaps)
	{
		out << heap << std::endl;
	}
	return out;
}
