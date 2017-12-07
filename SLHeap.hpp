#include <cstddef>
#include <iostream>
#include "BaseHeap.hpp"


#pragma once


template<typename, class, class>
class SLHeap;

template<typename, class>
class SLNode;

template<typename>
class LeftistHeap;

template<typename>
class LeftistNode;

template<typename>
class ScewHeap;

template<typename>
class ScewNode;

template<typename TKey, class TDerived, class TNode>
std::ostream& operator<<(std::ostream&, const SLHeap<TKey, TDerived, TNode>&);

template<typename TKey, class TDerived>
std::ostream& operator<<(std::ostream&, const SLNode<TKey, TDerived>&);

template<typename TKey, class TDerived, class TNode>
class SLHeap : public BaseHeap<TKey, TDerived>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

	using NodeType = TNode;

	SLHeap();

	SLHeap(const SLHeap<TKey, TDerived, TNode>&);

	KeyConstReference GetMin() const override;

	KeyType ExtractMin() override;

	void Insert(KeyConstReference) override;

	void MeldOn(const TDerived&) override;

	~SLHeap() noexcept;

	friend std::ostream& operator<<<>(std::ostream&, const SLHeap<TKey, TDerived, TNode>&);
protected:
	static NodeType* _meld(NodeType*, NodeType*);

	NodeType* _root;
};

template<typename TKey, typename TDerived>
class SLNode : public BaseNode<TKey, TDerived>
{
public:
	explicit SLNode(const TKey&);
	SLNode(const SLNode<TKey, TDerived>&);

	SLNode<TKey, TDerived>& operator=(const SLNode<TKey, TDerived>&) = delete;

	virtual void RestoreInvariant() = 0;

	virtual ~SLNode() noexcept override;

	template<typename TKey_, class TDerived_, class TNode_>
	friend class SLHeap;

	friend std::ostream& operator<<<>(std::ostream&, const SLNode<TKey, TDerived>&);
protected:
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
class LeftistHeap : public SLHeap<TKey, LeftistHeap<TKey>, LeftistNode<TKey>>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;
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
class ScewHeap : public SLHeap<TKey, ScewHeap<TKey>, ScewNode<TKey>>
{
public:
	using KeyType = TKey;
	using KeyReference = TKey&;
	using KeyConstReference = const TKey&;

private:
};

#include "SLHeap.tpp"
