#include "SLHeap.hpp" //this is needed for autocompletion to work :/

template<typename TKey, class TDerived, class TNode>
SLHeap<TKey, TDerived, TNode>::SLHeap()
	: BaseHeap<TKey, TDerived>(), _root(nullptr), _size(0)
{

}

template<typename TKey, class TDerived, class TNode>
SLHeap<TKey, TDerived, TNode>::SLHeap(const SLHeap<TKey, TDerived, TNode>& other)
	: BaseHeap<TKey, TDerived>(other), _size(other._size)
{
	if (other._root != nullptr) _root = new TNode(*other._root);
	else _root = nullptr;
}

template<typename TKey, class TDerived, class TNode>
SLHeap<TKey, TDerived, TNode>& 
	SLHeap<TKey, TDerived, TNode>::operator=(const SLHeap<TKey, TDerived, TNode>& other)
{
	if (this == &other) return *this;
	
	if (this->_root != nullptr) delete this->_root;

	if (other._root != nullptr) _root = new TNode(*other._root);
	else _root = nullptr;

	this->_size = other._size;

	return *this;
}

template<typename TKey, class TDerived, class TNode>
SLHeap<TKey, TDerived, TNode>::~SLHeap() noexcept
{
	if (this->_root != nullptr) delete this->_root;
}

template<typename TKey, class TDerived, class TNode>
const TKey& SLHeap<TKey, TDerived, TNode>::GetMin() const
{
	//OH THANKS C++! NAME LOOKUP IS CONVINIENT AS ALWASY, I SEE
	using BN = BaseNode<TKey, TNode>;
	//if someone tries too get an empty heap's minimum, it's considered to be undefined behaviour
	return BN::Key(this->_root);
}

template<typename TKey, class TDerived, class TNode>
TKey SLHeap<TKey, TDerived, TNode>::ExtractMin()
{
	using BN = BaseNode<TKey, TNode>;

	//undefined behaviour for empty heaps as well
	TNode* root = this->_root;

	this->_root = _meld(this->_root->_left, this->_root->_right);

	TKey key = BN::Key(root);

	//guess how much time I spent trying to find this segfault?
	root->_left = nullptr;
	root->_right = nullptr;
	delete root;

	--this->_size;
	return key;
}

template<typename TKey, class TDerived, class TNode>
void SLHeap<TKey, TDerived, TNode>::Insert(KeyConstReference key) 
{
	this->_root = _meld(this->_root, new TNode(key));
	++this->_size;
}

template<typename TKey, class TDerived, class TNode>
void SLHeap<TKey, TDerived, TNode>::MeldOn(const TDerived& other)
{
	using BN = BaseNode<TKey, TNode>;
	if (BN::Empty(other._root)) return;

	TNode* copy = new TNode(*other._root);
	this->_root = _meld(this->_root, copy);

	this->_size += other._size;
}

template<typename TKey, class TDerived, class TNode>
TNode* SLHeap<TKey, TDerived, TNode>::_meld(TNode* left, TNode* right)
{
	using BN = BaseNode<TKey, TNode>;

	if (BN::Empty(left)) return right;
	if (BN::Empty(right)) return left;

	if (BN::Key(left) > BN::Key(right)) std::swap(left, right);

	left->_right = _meld(left->_right, right);

	left->RestoreInvariant();

	return left;
}

template<typename TKey, class TDerived, class TNode>
std::size_t SLHeap<TKey, TDerived, TNode>::Size() const
{
	return this->_size;
}

template<typename TKey, class TDerived, class TNode>
bool SLHeap<TKey, TDerived, TNode>::Empty() const
{
	return this->_size == 0;
}

template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::SLNode(const TKey& key)
	: BaseNode<TKey, TDerived>(key), _left(nullptr), _right(nullptr)
{

}

template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::SLNode(const SLNode<TKey, TDerived>& other)
	: BaseNode<TKey, TDerived>(other)
{
	//not using BaseNode<TKey, TDerived>::Empty because 
	//what's happening here is more clear this way
	this->_left = other._left == nullptr ? nullptr : new TDerived(*other._left);
	this->_right = other._right == nullptr ? nullptr : new TDerived(*other._right);
}

template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::~SLNode() noexcept
{
	//same
	if (this->_left != nullptr) delete this->_left;
	if (this->_right != nullptr) delete this->_right;
}

template<typename TKey>
LeftistNode<TKey>::LeftistNode(const TKey& key)
	: SLNode<TKey, LeftistNode<TKey>>(key), _rank(1)
{

}

template<typename TKey>
int LeftistNode<TKey>::Rank(const LeftistNode<TKey>* node)
{
	using BN = BaseNode<TKey, LeftistNode<TKey>>;
	
	if (BN::Empty(node)) return 0;

	return node->_rank;
}

template<typename TKey>
void LeftistNode<TKey>::RestoreInvariant()
{
	using BN = BaseNode<TKey, LeftistNode<TKey>>;

	if (BN::Empty(this->_left))
	{
		std::swap(this->_left, this->_right);
		return;
	}

	if (this->_left->_rank < this->_right->_rank)
	{
		std::swap(this->_left, this->_right);
	}

	this->_rank = Rank(this->_right) + 1;
}


template<typename TKey>
ScewNode<TKey>::ScewNode(const TKey& key)
	: SLNode<TKey, ScewNode<TKey>>(key)
{

}

template<typename TKey>
void ScewNode<TKey>::RestoreInvariant()
{
	std::swap(this->_left, this->_right);
}

template<typename TKey, class TDerived, class TNode>
std::ostream& operator<<(std::ostream& out, const SLHeap<TKey, TDerived, TNode>& heap)
{
	out << "Min: " << heap.GetMin() << std::endl;
	
	out << *heap._root << std::endl;

	return out;
}

template<typename TKey, class TDerived>
std::ostream& operator<<(std::ostream& out, const SLNode<TKey, TDerived>& node)
{
	using BN = BaseNode<TKey, TDerived>;

	if (BN::Empty(&node))
	{
		out << "()"; 
	}
	else
	{
		out << "(" << BN::Key(&node) << ": ";
		out << *node._left;
		out << *node._right;
		out << ")";
	}

	return out;
}
