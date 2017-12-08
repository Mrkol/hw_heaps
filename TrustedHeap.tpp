#include "TrustedHeap.hpp"

template<typename TKey>
typename TrustedHeap<TKey>::KeyConstReference TrustedHeap<TKey>::GetMin() const
{
	return this->_impl[0];
}

template<typename TKey>
typename TrustedHeap<TKey>::KeyType TrustedHeap<TKey>::ExtractMin()
{
	KeyType copy = this->_impl[0];

	std::pop_heap(this->_impl.begin(), this->_impl.end(), std::greater<TKey>());
	this->_impl.pop_back();

	return copy;
}

template<typename TKey>
void TrustedHeap<TKey>::Insert(KeyConstReference key)
{
	this->_impl.push_back(key);
	std::push_heap(this->_impl.begin(), this->_impl.end(), std::greater<TKey>());
}

template<typename TKey>
void TrustedHeap<TKey>::MeldOn(const TrustedHeap<TKey>& other)
{
	this->_impl.reserve(this->_impl.size() + other._impl.size());
	for (auto key : other._impl) this->_impl.push_back(key);
	std::make_heap(this->_impl.begin(), this->_impl.end(), std::greater<TKey>());
}

template<typename TKey>
bool TrustedHeap<TKey>::Contains(KeyConstReference key) const
{
	return std::binary_search(this->_impl.begin(), this->_impl.end(), key);
}


template<typename TKey>
std::size_t TrustedHeap<TKey>::Size() const
{
	return this->_impl.size();
}

template<typename TKey>
bool TrustedHeap<TKey>::Empty() const
{
	return this->_impl.empty();
}

template<typename TKey>
std::ostream& operator<<(std::ostream& out, const TrustedHeap<TKey>& heap)
{
	for (auto node : heap._impl) out << node << " ";

	return out;
}
