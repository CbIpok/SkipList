#pragma once
#include "skip_list.h"
template <typename Key,
	typename Value,
	typename Compare = std::less<Key>,
	typename Alloc = std::allocator<std::pair<const Key, Value> > >
	class skip_list;

template<typename Key, typename Value, typename Compare, typename Alloc>
class SkipListIterator: public std::iterator<std::bidirectional_iterator_tag,std::pair<Key,Value&>>
{
public:
	//using skipList = skip_list<Key, Value>;
	using NodeIterator = typename skip_list<Key, Value, Compare, Alloc>::NodeIterator;
	using valueType = std::pair<Key, Value&>;

	SkipListIterator(NodeIterator begin);
	~SkipListIterator() = default;

	valueType operator* ()
	{
		return valueType(_curIterator->key, *_curIterator->value);
	}

	SkipListIterator operator++()
	{
		++_curIterator;
		return *this;
	}

	SkipListIterator operator++(int)
	{
		return SkipListIterator<Key,Value,Compare,Alloc>(_curIterator++);
	}

	bool operator==(const SkipListIterator& rhs) const
	{
		return _curIterator == rhs.getNodeIterator();
	}

	bool operator!=(SkipListIterator rhs) const//&&&&&&&&&&&&
	{
		return _curIterator != rhs.getNodeIterator();
	}
	valueType* operator-> ()
	{
		return &valueType(_curIterator->key, *_curIterator->value);
	}
	inline NodeIterator getNodeIterator()
	{
		return _curIterator;
	}
private:
	NodeIterator _curIterator;
};

template<typename Key, typename Value, typename Compare, typename Alloc>
inline SkipListIterator<Key, Value, Compare, Alloc>::SkipListIterator(NodeIterator begin):
	_curIterator(begin)
{

}




