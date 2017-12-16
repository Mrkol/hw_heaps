#include "BinomialHeap.hpp" //this is needed for autocompletion to work :/

template<typename TKey>
BinomialHeap<TKey>::BinomialHeap()
	: _size(0)
{
	_roots.push_back(nullptr);
	_minimum = _roots.begin();
}

template<typename TKey>
BinomialHeap<TKey>::BinomialHeap(const BinomialHeap<TKey>& other)
	: _size(other._size)
{
	_copyState(other);
}

template<typename TKey>
BinomialHeap<TKey>& BinomialHeap<TKey>::operator =(const BinomialHeap<TKey>& other)
{
	if (this == &other) 
		return *this;

	for (auto ptr : _roots)
	{
		if (ptr != nullptr) 
			delete ptr;
	}

	_size = other._size;
	_roots.clear();

	_copyState(other);

	return *this;
}

template<typename TKey>
void BinomialHeap<TKey>::_copyState(const BinomialHeap<TKey>& other)
{
	for (auto ptr : other._roots)
	{
		if (ptr != nullptr) 
			_roots.push_back(new BinomialNode<TKey>(*ptr));
		else 
			_roots.push_back(nullptr);
	}

	_minimum = std::next(_roots.begin(),
		 std::distance<typename std::list<NodeType*>::const_iterator>(
		 	other._roots.begin(), 
		 	other._minimum));
}

template<typename TKey>
BinomialHeap<TKey>::~BinomialHeap() noexcept
{
	for (auto ptr : _roots)
	{
		if (ptr != nullptr)
			delete ptr;
	}
}

template<typename TKey>
const TKey& BinomialHeap<TKey>::GetMin() const
{
	return NodeType::Key(*_minimum);
}

template<typename TKey>
void BinomialHeap<TKey>::Insert(const TKey& key)
{
	std::list<BinomialHeap<TKey>::NodeType*> unitlist;
	unitlist.push_back(new BinomialHeap<TKey>::NodeType(key));

	_meld(unitlist);

	++_size;
}

template<typename TKey>
TKey BinomialHeap<TKey>::ExtractMin()
{
	std::list<BinomialHeap<TKey>::NodeType*> minchildren = (*_minimum)->_children;
	(*_minimum)->_children.clear();

	TKey key = NodeType::Key(*_minimum);
	delete *_minimum;
	*_minimum = nullptr;

	_meld(minchildren);

	--_size;

	return key;
}

template<typename TKey>
void BinomialHeap<TKey>::MeldOn(IHeap<TKey>& other)
{
	BinomialHeap<TKey>& casted = dynamic_cast<BinomialHeap<TKey>&>(other);

	_meld(casted._roots);
	_size += casted._size;

	casted._roots.clear();
	casted._roots.push_back(nullptr);
	casted._minimum = casted._roots.begin();
	casted._size = 0;
}

template<typename TKey>
std::size_t BinomialHeap<TKey>::Size() const
{
	return _size;
}

template<typename TKey>
bool BinomialHeap<TKey>::Empty() const
{
	return _size == 0;
}

template<typename TKey>
void BinomialHeap<TKey>::_meld(std::list<BinomialHeap<TKey>::NodeType*>& list)
{
	using NodeType = BinomialHeap<TKey>::NodeType;

	while (_roots.size() < list.size()) 
		_roots.push_back(nullptr);

	//this is needed for the last reminder to work out nicely
	_roots.push_back(nullptr);


	auto it1 = _roots.begin();
	auto it2 = list.begin();
	NodeType* curry = nullptr;

	while (it1 != _roots.end())
	{
		_partialReminderMeld(*it1, it2 != list.end() ? *it2 : nullptr, curry, *it1, curry);

		_updateMinimum(it1);

		++it1;
		if (it2 != list.end()) ++it2;
	}

	while(_roots.size() > 1 && _roots.back() == nullptr) 
		_roots.pop_back();
}

template<typename TKey>
void BinomialHeap<TKey>::_updateMinimum(typename std::list<BinomialHeap<TKey>::NodeType*>::iterator& it)
{
	if (*it == nullptr)
	{
		return;
	}
	else if (*_minimum == nullptr || NodeType::Key(*it) < NodeType::Key(*_minimum))
	{
		_minimum = it;
	}
}

template<typename TKey>
void BinomialHeap<TKey>::_partialReminderMeld(
		NodeType* left, 
		NodeType* right, 
		NodeType* rem,
		NodeType*& res,
		NodeType*& curr)
{
	if (right == nullptr)
		std::swap(right, rem);

	if (rem == nullptr)
	{
		_partialMeld(left, right, res, curr);
		return;
	}

	_partialMeld(rem, right, res, curr);
	res = left;
}

template<typename TKey>
void BinomialHeap<TKey>::_partialMeld(
		NodeType* left, 
		NodeType* right,
		NodeType*& res,
		NodeType*& curr)
{
	if (left == nullptr) 
		std::swap(left, right);

	if (right == nullptr)
	{
		res = left;
		curr = nullptr;
		return;
	}

	if (NodeType::Key(left) > NodeType::Key(right)) 
		std::swap(left, right);

	left->_children.push_back(right);

	res = nullptr;
	curr = left;
}

template<typename TKey>
BinomialNode<TKey>::BinomialNode(const TKey& key)
	: _key(key)
{

}

template<typename TKey>
BinomialNode<TKey>::BinomialNode(const BinomialNode<TKey>& other)
	: _key(other._key)
{
	for (auto ptr : other._children)
	{
		//there can and should be no empty children
		_children.push_back(new BinomialNode<TKey>(*ptr));
	}
}

template<typename TKey>
const TKey& BinomialNode<TKey>::Key(const BinomialNode<TKey>* node)
{
	return node->_key;
}


template<typename TKey>
BinomialNode<TKey>::~BinomialNode()
{
	for (auto ptr : _children)
	{
		if (ptr != nullptr) 
			delete ptr;
	}
}

template<typename TKey>
std::ostream& operator <<(std::ostream& out, const BinomialHeap<TKey>& heap)
{
	out << "Min: " << (*heap._minimum != nullptr ? heap.GetMin() : -228) << std::endl;

	for (auto ptr : heap._roots)
	{
		out << ptr << std::endl;
	}

	return out;
}

template<typename TKey>
std::ostream& operator <<(std::ostream& out, const BinomialNode<TKey>* node)
{
	if (node == nullptr)
	{
		out << "()"; 
	}
	else
	{
		out << "(" << BinomialNode<TKey>::Key(node) << ": ";
		for (auto ptr : node->_children)
		{
			out << ptr;
		}
		out << ")";
	}

	return out;
}
