#pragma once
#include <functional>
#include <memory>
#include"SkipListIterator.h"
#include "ConstSkipListIterator.h"
#include <random>
#include <list>
#include <array>
#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename Key,
	typename Value,
	typename Compare = std::less<Key>,
	typename Alloc = std::allocator<std::pair<const Key, Value> > >
	class skip_list {
	// ...
	public:
		struct Node;
		typedef SkipListIterator<Key, Value, Compare, Alloc> iterator;
		typedef ConstSkipListIterator<Key, Value, Compare, Alloc> const_iterator;
		typedef std::pair<const Key, Value> value_type;
		using ValueIterator = typename std::list<Value>::iterator;
		using NodeIterator = typename std::list<Node>::iterator;
		skip_list();
		explicit skip_list(const Compare &comp, const Alloc &a = Alloc());
		skip_list(const skip_list &another);

		skip_list &operator=(const skip_list &another);

		iterator begin()
		{
			return iterator(std::next(_skipList[0].begin()));
		}
		const_iterator cbegin() const;
		iterator end()
		{
			return iterator(_skipList[0].end());
		}
		const_iterator cend() const;

		bool empty() const;
		size_t size() const;
		Value &operator[](const Key &key);
		Value &at(const Key &key);
		//const Value &at(const Key &key);

		std::pair<iterator, bool> insert(const value_type &)
		{

		}

		void erase(iterator position);
		size_t erase(const Key &key);
		void erase(iterator first, iterator last);

		void swap(skip_list &another);
		void clear();

		iterator find(const Key &key);
		const_iterator find(const Key &key) const;

		struct Node
		{
			Node(ValueIterator valueIterator, Key k) :
				value(valueIterator),
				key(k)
			{
			}
			Node() = default;
			NodeIterator down;
			NodeIterator up;
			size_t height;
			Key key;
			ValueIterator value;
		};



	private:
		//_countOfLayers => 1
		static const size_t _countOfLayers = 4;
		std::list<Value> _values;
		std::array<std::list<Node>, _countOfLayers> _skipList;
		NodeIterator _skipListBegin;



		bool  _skipDown(NodeIterator& nodeIterator, size_t& layer)
		{
			if (layer > 1)
			{
				nodeIterator = nodeIterator->down;
				layer--;
				return true;
			}
			return false;
		}

		void _createNode(NodeIterator& prevNodeIterator, size_t& layer, std::array<NodeIterator, _countOfLayers>& prevNodes, Key key)
		{
			do
			{
				prevNodes[layer - 1] = prevNodeIterator;
			} while (_skipDown(prevNodeIterator, layer));
			std::srand(std::time(nullptr));
			int height = std::rand() % _countOfLayers;
			_values.push_back(Value());
			ValueIterator value = std::prev(_values.end());
			NodeIterator prev = _skipList[0].insert(std::next(prevNodes[0]), Node(value, key));
			prev->height = height;
			for (size_t i = 1; i <= height; i++)
			{
				NodeIterator  cur = _skipList[i].insert(std::next(prevNodes[i]), Node(value, key));
				cur->down = prev;
				prev->up = cur;
				prev = cur;
			}
		}

		NodeIterator _getNode(const Key& key, bool getPrev = false)
		{
			size_t layer = _countOfLayers;
			std::array<NodeIterator, _countOfLayers> prevNodes;
			NodeIterator curNode = _skipListBegin;
			bool isFoundPrev = false;
			while (!isFoundPrev)
			{
				if (std::next(curNode) != _skipList[layer - 1].end() && key <= std::next(curNode)->key)
				{
					if (key == std::next(curNode)->key)
					{
						if (!getPrev)
							return std::next(curNode);
						else
						{
							while (_skipDown(curNode, layer));
							return std::next(curNode);
						}
					}
					++curNode;
				}
				else
				{
					prevNodes[layer - 1] = curNode;
					isFoundPrev = !_skipDown(curNode, layer);
				}
			}
			if (getPrev)
			{
				return std::next(curNode);
			}
			bool isElementFound = !((std::next(curNode) == _skipList[layer - 1].end()) || std::next(curNode)->key != key);
			if (!isElementFound)
			{
				_createNode(curNode, layer, prevNodes, key);
			}
			return std::next(curNode);
		}

};

template <typename K, typename V, typename C, typename A>
inline bool operator==(const skip_list<K, V, C, A> &x, const skip_list<K, V, C, A> &y) {
	return true;
}

template <typename K, typename V, typename C, typename A>
inline bool operator!=(const skip_list<K, V, C, A> &x, const skip_list<K, V, C, A> &y) {
	return false;
}

template<typename Key, typename Value, typename Compare, typename Alloc>
inline skip_list<Key, Value, Compare, Alloc>::skip_list()
{
	_skipList[0].push_back(Node());
	NodeIterator prevNode = std::prev(_skipList[0].end());
	for (size_t i = 1; i < _countOfLayers; i++)
	{
		_skipList[i].push_back(Node());
		NodeIterator curNode = (std::prev(_skipList[i].end()));
		curNode->down = prevNode;
		std::swap(curNode, prevNode);
	}
	_skipListBegin = std::prev(_skipList[_countOfLayers - 1].end());

}

template<typename Key, typename Value, typename Compare, typename Alloc>
inline Value & skip_list<Key, Value, Compare, Alloc>::operator[](const Key & key)
{
	return *_getNode(key)->value;
}

template<typename Key, typename Value, typename Compare, typename Alloc>
inline void skip_list<Key, Value, Compare, Alloc>::erase(iterator position)
{
	erase((*position).first);
}

template<typename Key, typename Value, typename Compare, typename Alloc>
inline size_t skip_list<Key, Value, Compare, Alloc>::erase(const Key & key)
{
	NodeIterator cur = _getNode(key, true);
	if (cur == _skipList[0].end())
	{
		return 0;
	}
	else
	{
		size_t height = cur->height;
		_values.erase(cur->value);
		for (size_t i = 0; i < height; i++)
		{
			NodeIterator next = cur->up;
			_skipList[i].erase(cur);
			cur = next;
		}
	}
	return 0;
}

template<typename Key, typename Value, typename Compare, typename Alloc>
inline void skip_list<Key, Value, Compare, Alloc>::erase(iterator first, iterator last)
{
	//std::cout << typeid(iterator).name();
	auto i = first++;
	while (first !=last)
	{
		erase(i);
		i = first++;
	}
}
