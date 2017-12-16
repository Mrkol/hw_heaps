#ifndef BINOMIAL_HEAP_HPP
#define BINOMIAL_HEAP_HPP

#include <vector>
#include <iostream>
#include <iterator>
#include <list>
#include "IHeap.hpp"


template<typename TKey>
class BinomialHeap;

template<typename TKey>
class BinomialNode;

template<typename TKey>
std::ostream& operator<<(std::ostream&, const BinomialHeap<TKey>&);

template<typename TKey>
std::ostream& operator<<(std::ostream&, const BinomialNode<TKey>*);

template<typename TKey>
class BinomialHeap : public IHeap<TKey>
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

	void MeldOn(IHeap<TKey>&) override;

	std::size_t Size() const override;

	bool Empty() const override;

	~BinomialHeap() noexcept;

	friend std::ostream& operator<< <>(std::ostream&, const BinomialHeap<TKey>&);
private:
	void _copyState(const BinomialHeap<TKey>&);
	//NOTE: this uses up the list
	void _meld(std::list<NodeType*>&);

	static void _partialReminderMeld(NodeType*, NodeType*, NodeType*, NodeType*&, NodeType*&);

	static void _partialMeld(NodeType*, NodeType*, NodeType*&, NodeType*&);

	void _updateMinimum(typename std::list<NodeType*>::iterator&);

	std::list<NodeType*> _roots;
	std::size_t _size;

	typename std::list<NodeType*>::iterator _minimum;
};

template<typename TKey>
class BinomialNode
{
public:
	explicit BinomialNode(const TKey&);

	BinomialNode(const BinomialNode<TKey>&);

	BinomialNode<TKey>& operator=(const BinomialNode<TKey>&) = delete;

	~BinomialNode();

	static inline const TKey& Key(const BinomialNode<TKey>*);

	friend std::ostream& operator<< <>(std::ostream&, const BinomialNode<TKey>*);

	template<typename TKey_>
	friend class BinomialHeap;

private:
	TKey _key;
	std::list<BinomialNode*> _children;
};

#include "BinomialHeap.tpp"

#endif //BINOMIAL_HEAP_HPP
