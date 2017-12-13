#ifndef IHEAP_HPP
#define IHEAP_HPP

#include <cstddef>


template<typename TKey>
class IHeap;

template<typename TKey>
class IHeap
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	virtual KeyConstReference GetMin() const = 0;

	virtual KeyType ExtractMin() = 0;

	virtual void Insert(KeyConstReference) = 0;

	virtual void MeldOn(const IHeap<TKey>&) = 0;

	virtual std::size_t Size() const = 0;

	virtual bool Empty() const = 0;
};

#endif //IHEAP_HPP
