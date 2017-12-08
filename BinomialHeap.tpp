#include "BinomialHeap.hpp" //this is needed for autocompletion to work :/

template<typename TKey>
BinomialHeap<TKey>::BinomialHeap()
	: BaseHeap<TKey, BinomialHeap<TKey>>(), _size(0)
{
	_roots.push_back(nullptr);
	_minimum = _roots.begin();
}

template<typename TKey>
BinomialHeap<TKey>::BinomialHeap(const BinomialHeap<TKey>& other)
	: BaseHeap<TKey, BinomialHeap<TKey>>(other)
{
	for (auto ptr : other._roots)
	{
		if (ptr != nullptr) this->_roots.push_back(new BinomialNode<TKey>(*ptr));
		else this->_roots.push_back(nullptr);
	}
	this->_minimum = std::next(this->_roots.begin(),
		 std::distance<typename std::list<NodeType*>::const_iterator>(other._roots.begin(), other._minimum));
	this->_size = other._size;
}

template<typename TKey>
BinomialHeap<TKey>& BinomialHeap<TKey>::operator=(const BinomialHeap<TKey>& other)
{
	for (auto ptr : this->_roots)
	{
		if (ptr != nullptr) delete ptr;
	}

	for (auto ptr : other._roots)
	{
		if (ptr != nullptr) this->_roots.push_back(new BinomialNode<TKey>(*ptr));
		else this->_roots.push_back(nullptr);
	}
	this->_minimum = std::next(this->_roots.begin(),
		 std::distance<typename std::list<NodeType*>::const_iterator>(other._roots.begin(), other._minimum));
	this->_size = other._size;

	return *this;
}

template<typename TKey>
BinomialHeap<TKey>::~BinomialHeap() noexcept
{
	for (auto ptr : this->_roots)
	{
		if (ptr != nullptr) delete ptr;
	}
}

template<typename TKey>
const TKey& BinomialHeap<TKey>::GetMin() const
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;

	return BN::Key(*this->_minimum);
}

template<typename TKey>
void BinomialHeap<TKey>::Insert(const TKey& key)
{
	std::list<BinomialHeap<TKey>::NodeType*> unitlist;
	unitlist.push_back(new BinomialHeap<TKey>::NodeType(key));

	_meld(unitlist);

	++this->_size;
}

template<typename TKey>
TKey BinomialHeap<TKey>::ExtractMin()
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;

	std::list<BinomialHeap<TKey>::NodeType*> minchildren = (*this->_minimum)->_children;
	(*this->_minimum)->_children.clear();

	TKey key = BN::Key(*this->_minimum);
	delete *this->_minimum;
	*this->_minimum = nullptr;

	this->_meld(minchildren);

	--this->_size;

	return key;
}

template<typename TKey>
void BinomialHeap<TKey>::MeldOn(const BinomialHeap<TKey>& other)
{
	using NodeType = BinomialHeap<TKey>::NodeType;
	std::list<NodeType*> copy;
	for (auto ptr : other._roots) copy.push_back(ptr != nullptr ? new NodeType(*ptr) : nullptr);
	this->_meld(copy);

	this->_size += other._size;
}

template<typename TKey>	
bool BinomialHeap<TKey>::Contains(KeyConstReference) const
{
	throw -1;
	return false;
}

template<typename TKey>
std::size_t BinomialHeap<TKey>::Size() const
{
	return this->_size;
}

template<typename TKey>
bool BinomialHeap<TKey>::Empty() const
{
	return this->_size == 0;
}

template<typename TKey>
void BinomialHeap<TKey>::_meld(std::list<BinomialHeap<TKey>::NodeType*>& list)
{
	using NodeType = BinomialHeap<TKey>::NodeType;

	while (this->_roots.size() < list.size()) 
		this->_roots.push_back(nullptr);

	//this is needed for the last reminder to work out nicely
	this->_roots.push_back(nullptr);


	auto it1 = this->_roots.begin();
	auto it2 = list.begin();
	NodeType* rem = nullptr;

	while (it1 != this->_roots.end())
	{
		_partialReminderMeld(*it1, it2 != list.end() ? *it2 : nullptr, rem);

		_updateMinimum(it1);

		++it1;
		if (it2 != list.end()) ++it2;
	}

	while(this->_roots.size() > 1 && this->_roots.back() == nullptr) 
		this->_roots.pop_back();
}

template<typename TKey>
void BinomialHeap<TKey>::_updateMinimum(typename std::list<BinomialHeap<TKey>::NodeType*>::iterator& it)
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;

	if (BN::Empty(*it)) return;
	else if (BN::Empty(*this->_minimum) || BN::Key(*it) < BN::Key(*_minimum))
	{
		this->_minimum = it;
	}
}

template<typename TKey>
void BinomialHeap<TKey>::_partialReminderMeld(
		BinomialHeap<TKey>::NodeType*& left_, 
		BinomialHeap<TKey>::NodeType* right,
		BinomialHeap<TKey>::NodeType*& rem_)
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;
	using NodeType = BinomialHeap<TKey>::NodeType;

	NodeType* left = left_;
	NodeType* rem = rem_;

	//Excessive nullptr assignment for clearness

	if (BN::Empty(right) && BN::Empty(rem))
	{
		left_ = left;
		rem_ = nullptr;
		return;
	}
	
	if (BN::Empty(left))
	{
		if (BN::Empty(right)) 
		{
			left_ = rem;
			rem_ = nullptr;
			return;
		}

		if (BN::Empty(rem))
		{
			left_ = right;
			rem_ = nullptr;
			return;
		}

		left_ = nullptr;
		rem_ = _partialMeld(rem, right);
		return;
	}

	if (BN::Empty(right))
	{
		left_ = nullptr;
		rem_ = _partialMeld(left, rem);
		return;
	}

	if (BN::Empty(rem))
	{
		left_ = nullptr;
		rem_ = _partialMeld(left, right);
		return;
	}
	
	left_ = _partialMeld(rem, right);
	rem_ = left;
}

template<typename TKey>
typename BinomialHeap<TKey>::NodeType* BinomialHeap<TKey>::_partialMeld(
		typename BinomialHeap<TKey>::NodeType* left, 
		typename BinomialHeap<TKey>::NodeType* right)
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;

	if (BN::Empty(left)) return right;
	if (BN::Empty(right)) return left;

	if (BN::Key(left) > BN::Key(right)) std::swap(left, right);

	left->_children.push_back(right);
	return left;
}

template<typename TKey>
BinomialNode<TKey>::BinomialNode(const TKey& key)
	: BaseNode<TKey, BinomialNode<TKey>>(key)
{

}

template<typename TKey>
BinomialNode<TKey>::BinomialNode(const BinomialNode<TKey>& other)
	: BaseNode<TKey, BinomialNode<TKey>>(other)
{
	for (auto ptr : other._children)
	{
		//there can and should be no empty children
		this->_children.push_back(new BinomialNode<TKey>(*ptr));
	}
}

template<typename TKey>
BinomialNode<TKey>::~BinomialNode()
{
	for (auto ptr : this->_children)
	{
		if (ptr != nullptr) delete ptr;
	}
}

template<typename TKey>
std::ostream& operator<<(std::ostream& out, const BinomialHeap<TKey>& heap)
{
	out << "Min: " << (*heap._minimum != nullptr ? heap.GetMin() : -228) << std::endl;

	for (auto ptr : heap._roots)
	{
		out << *ptr << std::endl;
	}

	return out;
}

template<typename TKey>
std::ostream& operator<<(std::ostream& out, const BinomialNode<TKey>& node)
{
	using BN = BaseNode<TKey, BinomialNode<TKey>>;

	if (BN::Empty(&node))
	{
		out << "()"; 
	}
	else
	{
		out << "(" << BN::Key(&node) << ": ";
		for (auto ptr : node._children)
		{
			out << *ptr;
		}
		out << ")";
	}

	return out;
}
