#include "TrustedHeap.hpp"

template<typename TKey>
typename TrustedHeap<TKey>::KeyConstReference TrustedHeap<TKey>::GetMin() const
{
	return _impl[0];
}

template<typename TKey>
typename TrustedHeap<TKey>::KeyType TrustedHeap<TKey>::ExtractMin()
{
	KeyType copy = _impl[0];

	std::pop_heap(_impl.begin(), _impl.end(), std::greater<TKey>());
	_impl.pop_back();

	return copy;
}

template<typename TKey>
void TrustedHeap<TKey>::Insert(KeyConstReference key)
{
	_impl.push_back(key);
	std::push_heap(_impl.begin(), _impl.end(), std::greater<TKey>());
}

template<typename TKey>
void TrustedHeap<TKey>::MeldOn(IHeap<TKey>& other)
{
	TrustedHeap<TKey>& casted = dynamic_cast<TrustedHeap<TKey>&>(other);
	
	_impl.reserve(_impl.size() + casted._impl.size());

	_impl.insert(_impl.end(), casted._impl.begin(), casted._impl.end());

	std::make_heap(_impl.begin(), _impl.end(), std::greater<TKey>());

	casted._impl.clear();
}

template<typename TKey>
std::size_t TrustedHeap<TKey>::Size() const
{
	return _impl.size();
}

template<typename TKey>
bool TrustedHeap<TKey>::Empty() const
{
	return _impl.empty();
}

template<typename TKey>
std::ostream& operator<<(std::ostream& out, const TrustedHeap<TKey>& heap)
{
	for (auto node : heap._impl) 
		out << node << " ";

	return out;
}
