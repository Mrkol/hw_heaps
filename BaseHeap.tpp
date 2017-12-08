#include "BaseHeap.hpp" //this is needed for autocompletion to work :/

template<typename TKey, class TDerived>
BaseNode<TKey, TDerived>::BaseNode(const TKey& key)
	: _key(key)
{

}

template<typename TKey, class TDerived>
bool BaseNode<TKey, TDerived>::Empty(const BaseNode<TKey, TDerived>* node)
{
	return node == nullptr;
}

template<typename TKey, class TDerived>
const TKey& BaseNode<TKey, TDerived>::Key(const BaseNode<TKey, TDerived>* node)
{
	//this is made so that if someone tries to access a nullptr's key,
	//it'll always segfault at the same place. Hopefully. 
	//exceptions aren't used for performance's sake
	return node->_key;
}
