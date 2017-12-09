#ifndef BASE_HEAP_HPP
#define BASE_HEAP_HPP

#include <cstddef>


template<typename TKey, class TDerived>
class BaseHeap;

template<typename TKey, class TDerived>
class BaseNode;


template<typename TKey, class TDerived>
class BaseHeap
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	virtual KeyConstReference GetMin() const = 0;

	virtual KeyType ExtractMin() = 0;

	virtual void Insert(KeyConstReference) = 0;

	virtual void MeldOn(const TDerived&) = 0;

	virtual std::size_t Size() const = 0;

	virtual bool Empty() const = 0;

	virtual ~BaseHeap() noexcept = default;
};

template<typename TKey, class TDerived>
class BaseNode
{
public:
	explicit BaseNode(const TKey&);

	static inline bool Empty(const BaseNode<TKey, TDerived>*);
	static inline const TKey& Key(const BaseNode<TKey, TDerived>*);

	virtual ~BaseNode() noexcept = default;
protected:
	TKey _key;
};

#include "BaseHeap.tpp"

#endif //BASE_HEAP_HPP
