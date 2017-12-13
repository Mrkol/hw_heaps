#include "SLHeap.hpp" //this is needed for autocompletion to work :/

template<typename TKey, class TNode>
SLHeap<TKey, TNode>::SLHeap()
	: _root(nullptr), _size(0)
{

}

template<typename TKey, class TNode>
SLHeap<TKey, TNode>::SLHeap(const SLHeap<TKey, TNode>& other)
	: _size(other._size)
{
	if (other._root != nullptr) _root = new TNode(*other._root);
	else _root = nullptr;
}

template<typename TKey, class TNode>
SLHeap<TKey, TNode>& SLHeap<TKey, TNode>::operator=(const SLHeap<TKey, TNode>& other)
{
	if (this == &other) 
		return *this;
	
	if (_root != nullptr) 
		delete _root;

	_root = other._root == nullptr ? nullptr : new TNode(*other._root);

	_size = other._size;

	return *this;
}

template<typename TKey, class TNode>
SLHeap<TKey, TNode>::~SLHeap() noexcept
{
	if (_root != nullptr) 
		delete _root;
}

template<typename TKey, class TNode>
const TKey& SLHeap<TKey, TNode>::GetMin() const
{
	//if someone tries too get an empty heap's minimum, it's considered to be undefined behaviour
	return NodeType::Key(_root);
}

template<typename TKey, class TNode>
TKey SLHeap<TKey, TNode>::ExtractMin()
{
	//undefined behaviour for empty heaps as well
	TNode* root = _root;

	_root = _meld(_root->_left, _root->_right);

	TKey key = NodeType::Key(root);

	//guess how much time I spent trying to find this segfault?
	root->_left = nullptr;
	root->_right = nullptr;
	delete root;

	--_size;
	return key;
}

template<typename TKey, class TNode>
void SLHeap<TKey, TNode>::Insert(KeyConstReference key) 
{
	_root = _meld(_root, new TNode(key));
	++_size;
}

template<typename TKey, class TNode>
void SLHeap<TKey, TNode>::MeldOn(const IHeap<TKey>& other)
{
	const SLHeap<TKey, TNode>& casted = dynamic_cast<const SLHeap<TKey, TNode>&>(other);

	if (casted._root == nullptr)
		return;

	TNode* copy = new TNode(*casted._root);
	_root = _meld(_root, copy);

	_size += casted._size;
}

template<typename TKey, class TNode>
TNode* SLHeap<TKey, TNode>::_meld(TNode* left, TNode* right)
{
	if (left == nullptr) return right;
	if (right == nullptr) return left;

	if (NodeType::Key(left) > NodeType::Key(right)) std::swap(left, right);

	left->_right = _meld(left->_right, right);

	left->RestoreInvariant();

	return left;
}

template<typename TKey, class TNode>
std::size_t SLHeap<TKey, TNode>::Size() const
{
	return _size;
}

template<typename TKey, class TNode>
bool SLHeap<TKey, TNode>::Empty() const
{
	return _size == 0;
}



template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::SLNode(const TKey& key)
	: _key(key), _left(nullptr), _right(nullptr)
{

}

template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::SLNode(const SLNode<TKey, TDerived>& other)
	: _key(other._key)
{
	_left = other._left == nullptr ? nullptr : new TDerived(*other._left);
	_right = other._right == nullptr ? nullptr : new TDerived(*other._right);
}

template<typename TKey, class TDerived>
const TKey& SLNode<TKey, TDerived>::Key(const SLNode<TKey, TDerived>* node)
{
	//if called on nullptr, behaviour is undefined
	return node->_key;
}

template<typename TKey, class TDerived>
SLNode<TKey, TDerived>::~SLNode() noexcept
{
	if (_left != nullptr) 
		delete _left;

	if (_right != nullptr) 
		delete _right;
}

template<typename TKey>
LeftistNode<TKey>::LeftistNode(const TKey& key)
	: SLNode<TKey, LeftistNode<TKey>>(key), _rank(1)
{

}

template<typename TKey>
int LeftistNode<TKey>::Rank(const LeftistNode<TKey>* node)
{	
	if (node == nullptr) return 0;

	return node->_rank;
}

template<typename TKey>
void LeftistNode<TKey>::RestoreInvariant()
{
	if (Rank(this->_left) < Rank(this->_right))
		std::swap(this->_left, this->_right);

	_rank = Rank(this->_right) + 1;
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

template<typename TKey, class TNode>
std::ostream& operator<<(std::ostream& out, const SLHeap<TKey, TNode>& heap)
{
	out << "Min: " << heap.GetMin() << std::endl;
	
	out << heap._root << std::endl;

	return out;
}

template<typename TKey, class TDerived>
std::ostream& operator<<(std::ostream& out, const SLNode<TKey, TDerived>* node)
{
	if (node == nullptr)
	{
		out << "()"; 
	}
	else
	{
		out << "(" << SLNode<TKey, TDerived>::Key(node) << ": ";
		out << node->_left;
		out << node->_right;
		out << ")";
	}

	return out;
}
