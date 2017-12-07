#pragma once


template<typename THeap>
class HeapList
{
public:
	using IndexType = typename THeap::IndexType;
	using KeyType = typename THeap::KeyType;
	using KeyReference = typename THeap::KeyReference;
	using KeyConstReference = typename THeap::KeyConstReference;
	using KeyPointer = typename THeap::KeyPointer;
	using KeyConstPointer = typename THeap::KeyConstPointer;

	void AddHeap(KeyType);
	
	void InsertKey(IndexType, KeyType);

	KeyConstReference GetMin(IndexType) const;
	
	KeyType ExtractMin(IndexType);
	
	void Meld(THeap&, THeap&);
};
