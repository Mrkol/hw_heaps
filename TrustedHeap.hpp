#ifndef TRUSTED_HEAP_HPP
#define TRUSTED_HEAP_HPP

#include <vector>
#include <algorithm>
#include "IHeap.hpp"

template<typename TKey>
class TrustedHeap;

template<typename TKey>
std::ostream& operator<<(std::ostream&, const TrustedHeap<TKey>&);

template<typename TKey>
class TrustedHeap : public IHeap<TKey>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	KeyConstReference GetMin() const override;

	KeyType ExtractMin() override;

	void Insert(KeyConstReference) override;

	void MeldOn(IHeap<TKey>&) override;

	std::size_t Size() const override;

	bool Empty() const override;

	friend std::ostream& operator<<<>(std::ostream&, const TrustedHeap<TKey>&);

private:
	std::vector<TKey> _impl;
};

#include "TrustedHeap.tpp"

#endif //TRUSTED_HEAP_HPP
