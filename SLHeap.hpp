#ifndef SLHEAP_HPP
#define SLHEAP_HPP

#include <cstddef>
#include <iostream>
#include "IHeap.hpp"


template<typename, class>
class SLHeap;

template<typename, class>
class SLNode;

template<typename>
class LeftistNode;

template<typename>
class ScewNode;

template<typename TKey, class TNode>
std::ostream& operator<<(std::ostream&, const SLHeap<TKey, TNode>&);

template<typename TKey, class TDerived>
std::ostream& operator<<(std::ostream&, const SLNode<TKey, TDerived>*);

template<typename TKey, class TNode>
class SLHeap : public IHeap<TKey>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	using NodeType = TNode;

	SLHeap();

	SLHeap(const SLHeap<TKey, TNode>&);

	SLHeap<TKey, TNode>& operator=(const SLHeap<TKey, TNode>&);

	KeyConstReference GetMin() const override;

	KeyType ExtractMin() override;

	void Insert(KeyConstReference) override;

	void MeldOn(IHeap<TKey>&) override;

	std::size_t Size() const override;

	bool Empty() const override;

	~SLHeap() noexcept;

	friend std::ostream& operator<<<>(std::ostream&, const SLHeap<TKey, TNode>&);	

protected:
	static NodeType* _meld(NodeType*, NodeType*);

	NodeType* _root;

	std::size_t _size;
};

template<typename TKey, typename TDerived>
class SLNode
{
public:
	explicit SLNode(const TKey&);
	SLNode(const SLNode<TKey, TDerived>&);

	SLNode<TKey, TDerived>& operator=(const SLNode<TKey, TDerived>&) = delete;

	virtual void RestoreInvariant() = 0;

	virtual ~SLNode() noexcept;

	template<typename TKey_, class TNode_>
	friend class SLHeap;

	static inline const TKey& Key(const SLNode<TKey, TDerived>*);

	friend std::ostream& operator<<<>(std::ostream&, const SLNode<TKey, TDerived>*);
	
protected:
	TKey _key;

	TDerived* _left;
	TDerived* _right;
};

template<typename TKey>
class LeftistNode : public SLNode<TKey, LeftistNode<TKey>>
{
public:
	explicit LeftistNode(const TKey&);

	void RestoreInvariant() override;

	static inline int Rank(const LeftistNode<TKey>*);

	virtual ~LeftistNode() override = default;

private:
	int _rank;
};

template<typename TKey>
class ScewNode : public SLNode<TKey, ScewNode<TKey>>
{
public:
	explicit ScewNode(const TKey&);

	void RestoreInvariant() override;

	virtual ~ScewNode() override = default;
};

template<typename TKey>
using ScewHeap = SLHeap<TKey, ScewNode<TKey>>;

template<typename TKey>
using LeftistHeap = SLHeap<TKey, LeftistNode<TKey>>;

#include "SLHeap.tpp"

#endif //SLHEAP_HPP
