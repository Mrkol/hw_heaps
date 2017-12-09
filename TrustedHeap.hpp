#ifndef TRUSTED_HEAP_HPP
#define TRUSTED_HEAP_HPP

#include <vector>
#include <algorithm>
#include "BaseHeap.hpp"

template<typename TKey>
class TrustedHeap;

template<typename TKey>
std::ostream& operator<<(std::ostream&, const TrustedHeap<TKey>&);

template<typename TKey>
class TrustedHeap : BaseHeap<TKey, TrustedHeap<TKey>>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	KeyConstReference GetMin() const override;

	KeyType ExtractMin() override;

	void Insert(KeyConstReference) override;

	void MeldOn(const TrustedHeap<TKey>&) override;

	std::size_t Size() const override;

	bool Empty() const override;

	friend std::ostream& operator<<<>(std::ostream&, const TrustedHeap<TKey>&);

private:
	std::vector<int> _impl;
};

#include "TrustedHeap.tpp"

#endif //TRUSTED_HEAP_HPP
