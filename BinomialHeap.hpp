#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <vector>
#include <iostream>
#include <iterator>
#include <list>
#include "BaseHeap.hpp"


template<typename TKey>
class BinomialHeap;

template<typename TKey>
class BinomialNode;

template<typename TKey>
std::ostream& operator<<(std::ostream&, const BinomialNode<TKey>&);

template<typename TKey>
std::ostream& operator<<(std::ostream&, const BinomialHeap<TKey>&);

template<typename TKey>
class BinomialHeap : public BaseHeap<TKey, BinomialHeap<TKey>>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;
	using NodeType = BinomialNode<TKey>;

	BinomialHeap();

	BinomialHeap(const BinomialHeap<TKey>&);

	BinomialHeap<TKey>& operator=(const BinomialHeap<TKey>&);

	KeyConstReference GetMin() const override;

	KeyType ExtractMin() override;

	void Insert(KeyConstReference) override;

	void MeldOn(const BinomialHeap<TKey>&) override;
	
	bool Contains(KeyConstReference) const override;

	std::size_t Size() const override;

	bool Empty() const override;

	~BinomialHeap() noexcept override;

	friend std::ostream& operator<< <>(std::ostream&, const BinomialHeap<TKey>&);
private:
	//NOTE: this uses up the list
	void _meld(std::list<NodeType*>&);
	static void _partialReminderMeld(NodeType*&, NodeType*, NodeType*&);
	static NodeType* _partialMeld(NodeType*, NodeType*);
	void _updateMinimum(typename std::list<NodeType*>::iterator&);

	std::list<NodeType*> _roots;
	std::size_t _size;

	typename std::list<NodeType*>::iterator _minimum;
};

template<typename TKey>
class BinomialNode : public BaseNode<TKey, BinomialNode<TKey>>
{
public:
	explicit BinomialNode(const TKey&);

	BinomialNode(const BinomialNode<TKey>&);

	BinomialNode<TKey>& operator=(const BinomialNode<TKey>&) = delete;

	~BinomialNode() override;

	friend std::ostream& operator<< <>(std::ostream&, const BinomialNode<TKey>&);

	template<typename TKey_>
	friend class BinomialHeap;
private:
	std::list<BinomialNode*> _children;
};

#include "BinomialHeap.tpp"

#endif //BINOMIAL_HEAP_HPP
