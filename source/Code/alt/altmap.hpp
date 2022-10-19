
#ifndef _ALT_MAP_HPP_
#define _ALT_MAP_HPP_

#include "../define.h"
//#include <map>
//TMapNode
//TMap
#include <new>

		// TEMPLATE FUNCTION swap (from <algorithm>)
template<class _Ty> inline
	void swap(_Ty& _Left, _Ty& _Right)
	{	// exchange values stored at _Left and _Right
	_Ty _Tmp = _Left;
	_Left = _Right, _Right = _Tmp;
	}

//functional
template<class _Arg1, class _Arg2, class _Result>
	struct binary_function
	{
		typedef _Arg1 first_argument_type;
		typedef _Arg2 second_argument_type;
		typedef _Result result_type;
	};

template<class _Ty>
	struct less : public binary_function<_Ty, _Ty, bool>
	{
		bool operator()(const _Ty& _Left, const _Ty& _Right) const { return (_Left < _Right); }
	};

//xmemory
	#define _CPOINTER_X(T, A)	typename A::template rebind<T>::other::const_pointer
	#define _CREFERENCE_X(T, A)	typename A::template rebind<T>::other::const_reference
	#define _POINTER_X(T, A)	typename A::template rebind<T>::other::pointer
	#define _REFERENCE_X(T, A)	typename A::template rebind<T>::other::reference

	template<class _Ty> inline
	_Ty *_Allocate(size_t _Count, _Ty *) { return ((_Ty *)operator new(_Count * sizeof (_Ty))); }

	template<class _Ty, class _Tv> inline
	void _Construct(_Ty *_Ptr, const _Tv& _Val) { new ((void *)_Ptr) _Ty(_Val); }

	template<class _Ty> inline
	void _Destroy(_Ty *_Ptr) { _Ptr->~_Ty(); }

//	template<> inline
//	void _Destroy(char *) {}

//	template<> inline
//	void _Destroy(wchar_t *) {}

	template<class _Ty>
	struct _Allocator_base
	{
		typedef _Ty value_type;
	};

	template<class _Ty>
	struct _Allocator_base<const _Ty>
	{
		typedef _Ty value_type;
	};

template<class _Ty>
class allocator	: public _Allocator_base<_Ty>
	{
public:
	typedef _Allocator_base<_Ty> _Mybase;
	typedef typename _Mybase::value_type value_type;

	typedef value_type *pointer;
	typedef value_type & reference;
	typedef const value_type *const_pointer;
	typedef const value_type & const_reference;

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	template<class _Other>
	struct rebind
	{
		typedef allocator<_Other> other;
	};

	pointer address(reference _Val) const {	return (&_Val);	}
	const_pointer address(const_reference _Val) const { return (&_Val); }

	allocator() {}
	allocator(const allocator<_Ty>&) {}
	template<class _Other>
	allocator(const allocator<_Other>&) {}
	template<class _Other>
	allocator<_Ty>& operator=(const allocator<_Other>&) { return (*this); }
	void deallocate(pointer _Ptr, size_type) { operator delete(_Ptr); }
	pointer allocate(size_type _Count) { return (_Allocate(_Count, (pointer)0)); }
	pointer allocate(size_type _Count, const void *) { return (allocate(_Count)); }
	void construct(pointer _Ptr, const _Ty& _Val) { _Construct(_Ptr, _Val); }
	void destroy(pointer _Ptr) { _Destroy(_Ptr); }

	size_t max_size() const
		{
		size_t _Count = (size_t)(-1) / sizeof(_Ty);
		return (0 < _Count ? _Count : 1);
		}
	};

	template<class _Ty, class _Other> inline
	bool operator==(const allocator<_Ty>&, const allocator<_Other>&) { return (true); }

	template<class _Ty, class _Other> inline
	bool operator!=(const allocator<_Ty>&, const allocator<_Other>&) { return (false); }

template<>
class allocator<void>
	{
public:
	typedef void _Ty;

	typedef _Ty *pointer;
	typedef const _Ty *const_pointer;
	typedef _Ty value_type;

	template<class _Other>
	struct rebind
	{
		typedef allocator<_Other> other;
	};

	allocator() {}
	allocator(const allocator<_Ty>&) {}

	template<class _Other>
	allocator(const allocator<_Other>&) {}

	template<class _Other>
	allocator<_Ty>& operator=(const allocator<_Other>&) { return (*this); }
	};

//utility
		// TEMPLATE STRUCT pair
template<class _Ty1, class _Ty2>
	struct pair
	{
	typedef pair<_Ty1, _Ty2> _Myt;
	typedef _Ty1 first_type;
	typedef _Ty2 second_type;

	pair()
		: first(_Ty1()), second(_Ty2())
		{
		}

	pair(const _Ty1& _Val1, const _Ty2& _Val2)
		: first(_Val1), second(_Val2)
		{
		}

	template<class _Other1,	class _Other2>
		pair(const pair<_Other1, _Other2>& _Right)
		: first(_Right.first), second(_Right.second)
		{
		}

//	void swap(_Myt& _Right)
//		{
//		std::swap(first, _Right.first);
//		std::swap(second, _Right.second);
//		}

	_Ty1 first;
	_Ty2 second;
	};


//xutility
		// ITERATOR TAGS
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag	{};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct _Int_iterator_tag {};

		// POINTER ITERATOR TAGS
struct _Nonscalar_ptr_iterator_tag {};
struct _Scalar_ptr_iterator_tag {};

		// TEMPLATE CLASS iterator
template<class _Category, class _Ty, class _Diff = ptrdiff_t, class _Pointer = _Ty *, class _Reference = _Ty&>
	struct iterator
	{
		typedef _Category iterator_category;
		typedef _Ty value_type;
		typedef _Diff difference_type;
		typedef _Diff distance_type;
		typedef _Pointer pointer;
		typedef _Reference reference;
	};

template<class _Ty, class _Diff, class _Pointer, class _Reference>
	struct _Bidit : public iterator<bidirectional_iterator_tag, _Ty, _Diff, _Pointer, _Reference>
	{
	};

template<class _Ty, class _Diff, class _Pointer, class _Reference>
	struct _Ranit : public iterator<random_access_iterator_tag, _Ty, _Diff, _Pointer, _Reference>
	{
	};

struct _Outit : public iterator<output_iterator_tag, void, void, void, void>
	{
	};

		// TEMPLATE CLASS iterator_traits
template<class _Iter>
	struct iterator_traits
	{
		typedef typename _Iter::iterator_category iterator_category;
		typedef typename _Iter::value_type value_type;
		typedef typename _Iter::difference_type difference_type;
		typedef difference_type distance_type;
		typedef typename _Iter::pointer pointer;
		typedef typename _Iter::reference reference;
	};

template<class _Ty>
	struct iterator_traits<_Ty *>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef _Ty *pointer;
		typedef _Ty& reference;
	};

template<class _Ty>
	struct iterator_traits<const _Ty *>
	{
		typedef random_access_iterator_tag iterator_category;
		typedef _Ty value_type;
		typedef ptrdiff_t difference_type;
		typedef ptrdiff_t distance_type;
		typedef const _Ty *pointer;
		typedef const _Ty& reference;
	};

//TMap only
//_Tree only
		// TEMPLATE CLASS _Tree_nod
template<class _Traits>
	class _Tree_nod	: public _Traits
	{
protected:
	struct _Node;
	friend struct _Node;

	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename _Traits::value_type value_type;
	typedef typename allocator_type::template rebind<_Node>::other::pointer _Genptr;

	struct _Node
	{
		_Node()
			: _Isnode(false)
			{
			}

		_Node(_Genptr _Larg, _Genptr _Parg, _Genptr _Rarg, char _Carg)
			: _Left(_Larg), _Parent(_Parg), _Right(_Rarg), _Color(_Carg), _Isnode(true)
			{
			}

		_Genptr _Left;
		_Genptr _Right;
		_Genptr _Parent;
		char _Color;
		char _Isnode;
	};

	struct _NodeValue : public _Node
		{
		_NodeValue(_Genptr _Larg, _Genptr _Parg, _Genptr _Rarg,
			const value_type& _Val, char _Carg)
			: _Node(_Larg, _Parg, _Rarg, _Carg),
				_Myval(_Val)
			{
			}

		value_type _Myval;
		};

	_Tree_nod(const key_compare& _Parg, allocator_type _Al)
		: _Traits(_Parg), _Alnod(_Al)
		{
		}

	typename allocator_type::template rebind<_NodeValue>::other _Alnod;
	};

		// TEMPLATE CLASS _Tree_ptr
template<class _Traits>
	class _Tree_ptr	: public _Tree_nod<_Traits>
	{
protected:
	typedef typename _Tree_nod<_Traits>::_Node _Node;
	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename allocator_type::template rebind<_Node>::other::pointer _Nodeptr;

	_Tree_ptr(const key_compare& _Parg, allocator_type _Al)
		: _Tree_nod<_Traits>(_Parg, _Al), _Alptr(_Al)
		{
		}

	typename allocator_type::template rebind<_Nodeptr>::other _Alptr;
	};

		// TEMPLATE CLASS _Tree_val
template<class _Traits>
	class _Tree_val	: public _Tree_ptr<_Traits>
	{
protected:
	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::key_compare key_compare;

	_Tree_val(const key_compare& _Parg, allocator_type _Al)
		: _Tree_ptr<_Traits>(_Parg, _Al), _Alval(_Al)
		{
		}

	allocator_type _Alval;
	};

		// TEMPLATE CLASS _Tree
template<class _Traits>
	class _Tree : public _Tree_val<_Traits>
	{
public:
	typedef typename _Tree_nod<_Traits>::_NodeValue _NodeValue; //+++

	typedef _Tree<_Traits> _Myt;
	typedef _Tree_val<_Traits> _Mybase;
	typedef typename _Traits::key_type key_type;
	typedef typename _Traits::mapped_type mapped_type;
	typedef typename _Traits::key_compare key_compare;
	typedef typename _Traits::value_compare value_compare;
	typedef typename _Traits::value_type value_type;
	typedef typename _Traits::allocator_type allocator_type;
	typedef typename _Traits::_ITptr _ITptr;
	typedef typename _Traits::_IReft _IReft;

protected:
	typedef typename _Tree_nod<_Traits>::_Genptr _Genptr;
	typedef typename _Tree_nod<_Traits>::_Node _Node;

	enum _Redbl {_Black, _Red};
	typedef _POINTER_X(_Node, allocator_type) _Nodeptr;
	typedef _REFERENCE_X(_Nodeptr, allocator_type) _Nodepref;
	typedef _CREFERENCE_X(key_type, allocator_type) _Keyref;
	typedef _CREFERENCE_X(mapped_type, allocator_type) _Mapref;
	typedef _REFERENCE_X(char, allocator_type) _Charref;
	typedef _REFERENCE_X(value_type, allocator_type) _Vref;

	static _Charref _Color(_Nodeptr _Pnode) { return ((_Charref)(*_Pnode)._Color); }
	static _Charref _Isnode(_Nodeptr _Pnode) { return ((_Charref)(*_Pnode)._Isnode); }
	static _Keyref _Key(_Nodeptr _Pnode) { return (_Mybase::_Kfn(_Myval(_Pnode))); }
	static _Mapref _Map(_Nodeptr _Pnode) { return (_Mybase::_Tfn(_Myval(_Pnode))); }
	static _Nodepref _Left(_Nodeptr _Pnode) { return ((_Nodepref)(*_Pnode)._Left); }
	static _Nodepref _Parent(_Nodeptr _Pnode) { return ((_Nodepref)(*_Pnode)._Parent); }
	static _Nodepref _Right(_Nodeptr _Pnode) { return ((_Nodepref)(*_Pnode)._Right); }
	static _Vref _Myval(_Nodeptr _Pnode) { return ((_Vref)(*((_NodeValue *)_Pnode))._Myval); }
public:
	typedef typename allocator_type::size_type size_type;
	typedef typename allocator_type::difference_type _Dift;
	typedef _Dift difference_type;
	typedef _POINTER_X(value_type, allocator_type) _Tptr;
	typedef _CPOINTER_X(value_type, allocator_type) _Ctptr;
	typedef _REFERENCE_X(value_type, allocator_type) _Reft;
	typedef _Tptr pointer;
	typedef _Ctptr const_pointer;
	typedef _Reft reference;
	typedef _CREFERENCE_X(value_type, allocator_type) const_reference;

		// CLASS const_iterator
	class const_iterator;
	friend class const_iterator;

	class const_iterator : public _Bidit<value_type, _Dift, _Ctptr, const_reference>
	{
	public:
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Dift difference_type;
		typedef _Ctptr pointer;
		typedef const_reference reference;

		const_iterator(const mapped_type *Mapped)
			: _Ptr(CONTAINING_RECORD_MY1(((unsigned char*)Mapped-sizeof(key_type)), _NodeValue, _Myval))
			{
				//_Ptr = (unsigned char*)_Ptr-sizeof(key_type);
			}

		const_iterator()
			: _Ptr(0)
			{
			}

 #define _TREE_CONST_ITERATOR(ppnode)	const_iterator(ppnode)

		const_iterator(_Nodeptr _Pnode)
			: _Ptr(_Pnode)
			{
			}

		const_reference operator*() const
			{
			return (_Myval(_Ptr));
			}

		_Ctptr operator->() const
			{
			return (&**this);
			}

		const_iterator& operator++()
			{
			_Inc();
			return (*this);
			}

		const_iterator operator++(int)
			{
			const_iterator _Tmp = *this;
			++*this;
			return (_Tmp);
			}

		const_iterator& operator--()
			{
			_Dec();
			return (*this);
			}

		const_iterator operator--(int)
			{
			const_iterator _Tmp = *this;
			--*this;
			return (_Tmp);
			}

		bool operator==(const const_iterator& _Right) const
			{
			return (_Ptr == _Right._Ptr);
			}

		bool operator!=(const const_iterator& _Right) const
			{
			return (!(*this == _Right));
			}

		void _Dec()
			{
			if (!_Ptr) return;
				// move to node with next smaller value
			if (!_Isnode(_Ptr))
				_Ptr = _Right(_Ptr);	// end() ==> rightmost
			else if (_Isnode(_Left(_Ptr)))
				_Ptr = _Max(_Left(_Ptr));	// ==> largest of left subtree
			else
				{	// climb looking for left subtree
				_Nodeptr _Pnode;
				while (_Isnode(_Pnode = _Parent(_Ptr))
					&& _Ptr == _Left(_Pnode))
					_Ptr = _Pnode;	// ==> parent while left subtree
				_Ptr = _Pnode;	// ==> parent (tail if begin())
				}
			}

		void _Inc()
			{
			if (!_Ptr) return;
				// move to node with next larger value
			if (!_Isnode(_Ptr))
				_Ptr = _Left(_Ptr);	// end() ==> leftmost
			else if (_Isnode(_Right(_Ptr)))
				_Ptr = _Min(_Right(_Ptr));	// ==> smallest of right subtree
			else
				{	// climb looking for right subtree
				_Nodeptr _Pnode;
				while (_Isnode(_Pnode = _Parent(_Ptr))
					&& _Ptr == _Right(_Pnode))
					_Ptr = _Pnode;	// ==> parent while right subtree
				_Ptr = _Pnode;	// ==> parent (head if end())
				}
			}

		_Nodeptr _Mynode() const
			{
			return (_Ptr);
			}

		operator bool() const
			{
			return _Ptr != 0 && _Isnode(_Ptr);
			}
	protected:
		_Nodeptr _Ptr;
		};

		// CLASS iterator
	class iterator;
	friend class iterator;

	class iterator : public const_iterator
	{
	public:
		typedef bidirectional_iterator_tag iterator_category;
		typedef _Dift difference_type;
		typedef _ITptr pointer;
		typedef _IReft reference;

		iterator(const mapped_type *Mapped)
			: const_iterator(Mapped) //_Ptr(CONTAINING_RECORD_MY1(((unsigned char*)Mapped-sizeof(key_type)), _NodeValue, _Myval))
			{
				//_Ptr = (unsigned char*)_Ptr-sizeof(key_type);
			}

		iterator()
			{
			}

 #define _TREE_ITERATOR(ppnode)	iterator(ppnode)

		iterator(_Nodeptr _Pnode)
			: const_iterator(_Pnode)
			{
			}

		reference operator*() const
			{
			return ((reference)**(const_iterator *)this);
			}

		pointer operator->() const
			{
			return (&**this);
			}

		iterator& operator++()
			{
			++(*(const_iterator *)this);
			return (*this);
			}

		iterator operator++(int)
			{
			iterator _Tmp = *this;
			++*this;
			return (_Tmp);
			}

		iterator& operator--()
			{
			--(*(const_iterator *)this);
			return (*this);
			}

		iterator operator--(int)
			{
			iterator _Tmp = *this;
			--*this;
			return (_Tmp);
			}
		};


//	typedef reverse_iterator<iterator> reverse_iterator;
//	typedef reverse_iterator<const_iterator> const_reverse_iterator;
	typedef pair<iterator, bool> _Pairib;
	typedef pair<iterator, iterator> _Pairii;
	typedef pair<const_iterator, const_iterator> _Paircc;

	explicit _Tree(const key_compare& _Parg, const allocator_type& _Al)
		: _Mybase(_Parg, _Al)
		{
		_Init();
		}

//	_Tree(const value_type *_First, const value_type *_Last,
//		const key_compare& _Parg, const allocator_type& _Al)
//		: _Mybase(_Parg, _Al)
//		{	// construct tree from [_First, _Last) array
//		_Init();
//		_TRY_BEGIN
//		insert(_First, _Last);
//		_CATCH_ALL
//		_Tidy();
//		_RERAISE;
//		_CATCH_END
//		}

	_Tree(const _Myt& _Right)
		: _Mybase(_Right.key_comp(), _Right.get_allocator())
		{
		_Init();
		_Copy(_Right);
		}

	~_Tree()
		{
		clear();
		}

	_Myt& operator=(const _Myt& _Right)
		{
		if (this != &_Right)
			{
			clear();
			this->comp = _Right.comp;
			_Copy(_Right);
			}
		return (*this);
		}

	iterator begin() { return (_TREE_ITERATOR(_Lmost())); }
	const_iterator begin() const { return (_TREE_CONST_ITERATOR(_Lmost()));	}
	iterator end() { return (_TREE_ITERATOR(_Myhead)); }
	const_iterator end() const { return (_TREE_CONST_ITERATOR(_Myhead)); }

	iterator rbegin() { return (_TREE_ITERATOR(_Rmost())); }
	const_iterator rbegin() const { return (_TREE_CONST_ITERATOR(_Rmost())); }
	iterator rend() { return (_TREE_ITERATOR(_Myhead)); }
	const_iterator rend() const { return (_TREE_CONST_ITERATOR(_Myhead)); }

//	reverse_iterator rbegin() { return (reverse_iterator(end())); }
//	const_reverse_iterator rbegin() const { return (const_reverse_iterator(end()));	}
//	reverse_iterator rend()	{ return (reverse_iterator(begin())); }
//	const_reverse_iterator rend() const { return (const_reverse_iterator(begin())); }

	size_type size() const { return (_Mysize); }
	size_type max_size() const { return (this->_Alval.max_size()); }
	bool empty() const { return (size() == 0); }

	allocator_type get_allocator() const { return (this->_Alval); }
	key_compare key_comp() const { return (this->comp); }
	value_compare value_comp() const { return (value_compare(key_comp())); }

	iterator insert(const value_type& _Val)
		{	// try to insert node with value _Val
		_Nodeptr _Trynode = _Myroot;
		_Nodeptr _Wherenode = _Myhead;
		bool _Addleft = true;	// add to left of head if tree empty
		while (_Isnode(_Trynode))
			{	// look for leaf to insert before (_Addleft) or after
			_Wherenode = _Trynode;
			_Addleft = this->comp(this->_Kfn(_Val), _Key(_Trynode));
			_Trynode = _Addleft ? _Left(_Trynode) : _Right(_Trynode);
			}

		if (this->_Multi)
			return _Insert(_Addleft, _Wherenode, _Val);
		else
			{	// insert only if unique
			iterator _Where = _TREE_ITERATOR(_Wherenode);
			if (!_Addleft)
				;	// need to test if insert after is okay
			else if (_Where == begin())
				return _Insert(true, _Wherenode, _Val);
			else
				--_Where;	// need to test if insert before is okay

			if (this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val)))
				return _Insert(_Addleft, _Wherenode, _Val);
			else
				return _Myhead;
			}
		}

//???
	iterator insert(iterator _Where,
		const value_type& _Val)
		{	// try to insert node with value _Val using _Where as a hint
		iterator _Next;

		if (size() == 0)
			return (_Insert(true, _Myhead, _Val));	// insert into empty tree
		else if (this->_Multi)
			{	// insert even if duplicate
			if (_Where == begin())
				{	// insert at beginning if before first element
				if (!this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val)))
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (_Where == end())
				{	// insert at end if after last element
				if (!this->comp(this->_Kfn(_Val), _Key(_Rmost())))
					return (_Insert(false, _Rmost(), _Val));
				}
			else if (!this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val))
				&& !this->comp(this->_Kfn(_Val),
					_Key((--(_Next = _Where))._Mynode())))
				{	// insert before _Where
				if (!_Isnode(_Right(_Next._Mynode())))
					return (_Insert(false, _Next._Mynode(), _Val));
				else
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (!this->comp(this->_Kfn(_Val), _Key(_Where._Mynode()))
				&& (++(_Next = _Where) == end()
					|| !this->comp(_Key(_Next._Mynode()),
						this->_Kfn(_Val))))
				{	// insert after _Where
				if (!_Isnode(_Right(_Where._Mynode())))
					return (_Insert(false, _Where._Mynode(), _Val));
				else
					return (_Insert(true, _Next._Mynode(), _Val));
				}
			}
		else
			{	// insert only if unique
			if (_Where == begin())
				{	// insert at beginning if before first element
				if (this->comp(this->_Kfn(_Val), _Key(_Where._Mynode())))
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (_Where == end())
				{	// insert at end if after last element
				if (this->comp(_Key(_Rmost()), this->_Kfn(_Val)))
					return (_Insert(false, _Rmost(), _Val));
				}
			else if (this->comp(this->_Kfn(_Val), _Key(_Where._Mynode()))
				&& this->comp(_Key((--(_Next = _Where))._Mynode()),
					this->_Kfn(_Val)))
				{	// insert before _Where
				if (!_Isnode(_Right(_Next._Mynode())))
					return (_Insert(false, _Next._Mynode(), _Val));
				else
					return (_Insert(true, _Where._Mynode(), _Val));
				}
			else if (this->comp(_Key(_Where._Mynode()), this->_Kfn(_Val))
				&& (++(_Next = _Where) == end()
					|| this->comp(this->_Kfn(_Val),
						_Key(_Next._Mynode()))))
				{	// insert after _Where
				if (!_Isnode(_Right(_Where._Mynode())))
					return (_Insert(false, _Where._Mynode(), _Val));
				else
					return (_Insert(true, _Next._Mynode(), _Val));
				}
			}

		return (insert(_Val));	// try usual insert if all else fails
		}

	template<class _Iter>
		void insert(_Iter _First, _Iter _Last)
		{	// insert [_First, _Last) one at a time
		for (; _First != _Last; ++_First)
			insert(*_First);
		}

	iterator erase(iterator _Where)
		{	// erase element at _Where
		_Nodeptr _Erasednode = _Where._Mynode();	// node to erase
		++_Where;	// save successor iterator for return

		_Nodeptr _Fixnode;	// the node to recolor as needed
		_Nodeptr _Fixnodeparent;	// parent of _Fixnode (which may be nil)
		_Nodeptr _Pnode = _Erasednode;

		if (!_Isnode(_Left(_Pnode)))
			_Fixnode = _Right(_Pnode);	// must stitch up right subtree
		else if (!_Isnode(_Right(_Pnode)))
			_Fixnode = _Left(_Pnode);	// must stitch up left subtree
		else
			{	// two subtrees, must lift successor node to replace erased
			_Pnode = _Where._Mynode();	// _Pnode is successor node
			_Fixnode = _Right(_Pnode);	// _Fixnode is its only subtree
			}

		if (_Pnode == _Erasednode)
			{	// at most one subtree, relink it
			_Fixnodeparent = _Parent(_Erasednode);
			if (_Isnode(_Fixnode))
				_Parent(_Fixnode) = _Fixnodeparent;	// link up

			if (_Myroot == _Erasednode)
				_Myroot = _Fixnode;	// link down from root
			else if (_Left(_Fixnodeparent) == _Erasednode)
				_Left(_Fixnodeparent) = _Fixnode;	// link down to left
			else
				_Right(_Fixnodeparent) = _Fixnode;	// link down to right

			if (_Lmost() == _Erasednode)
				_Lmost() = !_Isnode(_Fixnode)
					? _Fixnodeparent	// smallest is parent of erased node
					: _Min(_Fixnode);	// smallest in relinked subtree

			if (_Rmost() == _Erasednode)
				_Rmost() = !_Isnode(_Fixnode)
					? _Fixnodeparent	// largest is parent of erased node
					: _Max(_Fixnode);	// largest in relinked subtree
			}
		else
			{	// erased has two subtrees, _Pnode is successor to erased
			_Parent(_Left(_Erasednode)) = _Pnode;	// link left up
			_Left(_Pnode) = _Left(_Erasednode);	// link successor down

			if (_Pnode == _Right(_Erasednode))
				_Fixnodeparent = _Pnode;	// successor is next to erased
			else
				{	// successor further down, link in place of erased
				_Fixnodeparent = _Parent(_Pnode);	// parent is successor's
				if (_Isnode(_Fixnode))
					_Parent(_Fixnode) = _Fixnodeparent;	// link fix up
				_Left(_Fixnodeparent) = _Fixnode;	// link fix down
				_Right(_Pnode) = _Right(_Erasednode);	// link successor down
				_Parent(_Right(_Erasednode)) = _Pnode;	// link right up
				}

			if (_Myroot == _Erasednode)
				_Myroot = _Pnode;	// link down from root
			else if (_Left(_Parent(_Erasednode)) == _Erasednode)
				_Left(_Parent(_Erasednode)) = _Pnode;	// link down to left
			else
				_Right(_Parent(_Erasednode)) = _Pnode;	// link down to right

			_Parent(_Pnode) = _Parent(_Erasednode);	// link successor up
			swap(_Color(_Pnode), _Color(_Erasednode));	// recolor it
			}

		if (_Color(_Erasednode) == _Black)
			{	// erasing black link, must recolor/rebalance tree
			for (; _Fixnode != _Myroot && _Color(_Fixnode) == _Black;
				_Fixnodeparent = _Parent(_Fixnode))
				if (_Fixnode == _Left(_Fixnodeparent))
					{	// fixup left subtree
					_Pnode = _Right(_Fixnodeparent);
					if (_Color(_Pnode) == _Red)
						{	// rotate red up from right subtree
						_Color(_Pnode) = _Black;
						_Color(_Fixnodeparent) = _Red;
						_Lrotate(_Fixnodeparent);
						_Pnode = _Right(_Fixnodeparent);
						}

					if (!_Isnode(_Pnode))
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Color(_Left(_Pnode)) == _Black
						&& _Color(_Right(_Pnode)) == _Black)
						{	// redden right subtree with black children
						_Color(_Pnode) = _Red;
						_Fixnode = _Fixnodeparent;
						}
					else
						{	// must rearrange right subtree
						if (_Color(_Right(_Pnode)) == _Black)
							{	// rotate red up from left sub-subtree
							_Color(_Left(_Pnode)) = _Black;
							_Color(_Pnode) = _Red;
							_Rrotate(_Pnode);
							_Pnode = _Right(_Fixnodeparent);
							}

						_Color(_Pnode) = _Color(_Fixnodeparent);
						_Color(_Fixnodeparent) = _Black;
						_Color(_Right(_Pnode)) = _Black;
						_Lrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
						}
					}
				else
					{	// fixup right subtree
					_Pnode = _Left(_Fixnodeparent);
					if (_Color(_Pnode) == _Red)
						{	// rotate red up from left subtree
						_Color(_Pnode) = _Black;
						_Color(_Fixnodeparent) = _Red;
						_Rrotate(_Fixnodeparent);
						_Pnode = _Left(_Fixnodeparent);
						}
					if (!_Isnode(_Pnode))
						_Fixnode = _Fixnodeparent;	// shouldn't happen
					else if (_Color(_Right(_Pnode)) == _Black
						&& _Color(_Left(_Pnode)) == _Black)
						{	// redden left subtree with black children
						_Color(_Pnode) = _Red;
						_Fixnode = _Fixnodeparent;
						}
					else
						{	// must rearrange left subtree
						if (_Color(_Left(_Pnode)) == _Black)
							{	// rotate red up from right sub-subtree
							_Color(_Right(_Pnode)) = _Black;
							_Color(_Pnode) = _Red;
							_Lrotate(_Pnode);
							_Pnode = _Left(_Fixnodeparent);
							}

						_Color(_Pnode) = _Color(_Fixnodeparent);
						_Color(_Fixnodeparent) = _Black;
						_Color(_Left(_Pnode)) = _Black;
						_Rrotate(_Fixnodeparent);
						break;	// tree now recolored/rebalanced
						}
					}

			_Color(_Fixnode) = _Black;	// ensure stopping node is black
			}

		this->_Alnod.destroy((_NodeValue*)_Erasednode);	// destroy, free erased node
		this->_Alnod.deallocate((_NodeValue*)_Erasednode, 1);

		if (0 < _Mysize)
			--_Mysize;

		return (_Where);	// return successor iterator
		}

	iterator erase(iterator _First, iterator _Last)
		{	// erase [_First, _Last)
		if (_First == begin() && _Last == end())
			{	// erase all
			clear();
			return (begin());
			}
		else
			{	// partial erase, one at a time
			while (_First != _Last)
				erase(_First++);
			return (_First);
			}
		}

	size_type erase(const key_type& _Keyval)
		{
		size_type _Num = 0;
		iterator _Where = find(_Keyval);
		while (_Where != end() && _Keyval == _Key(_Where._Mynode()))
			{
			erase(_Where++);
			++_Num;
			}
		return (_Num);
		}

	void clear()
		{
		if (_Mysize != 0)
			{
			_Erase(_Myroot);
			_Init();
			}
		}

	iterator find_near(const key_type& _Keyval)
		{
		iterator _Where = lower_bound(_Keyval);
		if (_Where == end() || this->comp(_Keyval, _Key(_Where._Mynode())))
			{
				if (_Where == end()) return end();
				if (this->comp(_Key(_Where._Mynode()), _Keyval)) return _Where;
				iterator _Next = _Where;
				_Next--;
				return (_Next == end() ? _Where : _Next);
			}
		return _Where;
		}

	iterator find_near(const key_type& _Keyval) const
		{
		const_iterator _Where = lower_bound(_Keyval);
		if (_Where == end() || this->comp(_Keyval, _Key(_Where._Mynode())))
			{
				if (_Where == end()) return end();
				if (this->comp(_Key(_Where._Mynode()), _Keyval)) return _Where;
				const_iterator _Next = _Where;
				_Next--;
				return (_Next == end() ? _Where : _Next);
			}
		return _Where;
		}

	iterator find(const key_type& _Keyval)
		{
		iterator _Where = lower_bound(_Keyval);
		return (_Where == end() || this->comp(_Keyval, _Key(_Where._Mynode()))
			? end() : _Where);
		}

	const_iterator find(const key_type& _Keyval) const
		{
		const_iterator _Where = lower_bound(_Keyval);
		return (_Where == end() || this->comp(_Keyval, _Key(_Where._Mynode()))
			? end() : _Where);
		}

	size_type count(const key_type& _Keyval) const
		{
		size_type _Num = 0;
		const_iterator _Where = find(_Keyval);
		while (_Where != end() && _Keyval == _Key(_Where._Mynode()))
			{
			_Where++;
			++_Num;
			}
		return (_Num);
		}

	iterator lower_bound(const key_type& _Keyval)
		{
		return (_TREE_ITERATOR(_Lbound(_Keyval)));
		}

	const_iterator lower_bound(const key_type& _Keyval) const
		{
		return (_TREE_CONST_ITERATOR(_Lbound(_Keyval)));
		}

	iterator upper_bound(const key_type& _Keyval)
		{
		return (_TREE_ITERATOR(_Ubound(_Keyval)));
		}

	const_iterator upper_bound(const key_type& _Keyval) const
		{
		return (_TREE_CONST_ITERATOR(_Ubound(_Keyval)));
		}

//	void swap(_Myt& _Right)
//		{
//		if (get_allocator() == _Right.get_allocator())
//			{
//			std::swap(this->comp, _Right.comp);
//			std::swap(_Myhead, _Right._Myhead);
//			std::swap(_Mysize, _Right._Mysize);
//			}
//		else
//			{
//			_Myt _Tmp = *this; *this = _Right, _Right = _Tmp;
//			}
//		}

protected:
	void _Copy(const _Myt& _Right)
		{	// copy entire tree from _Right
//		_Root() = _Copy(_Right._Root(), _Myhead);
//		_Mysize = _Right.size();
//		if (!_Isnode(_Root()))
//			{	// nonempty tree, look for new smallest and largest
//			_Lmost() = _Min(_Root());
//			_Rmost() = _Max(_Root());
//			}
//		else
//			_Lmost() = _Myhead, _Rmost() = _Myhead;	// empty tree

			if (_Right.size())
			{
				const_iterator _Where = _Right.begin();
				while (_Where != _Right.end())
				{
					insert(_Myval(_Where._Mynode())); //multi
					_Where++;
				}
			}
		}

//	_Nodeptr _Copy(_Nodeptr _Rootnode, _Nodeptr _Wherenode)
//		{	// copy entire subtree, recursively
//		_Nodeptr _Newroot = _Myhead;	// point at nil node
//
//		if (!_Isnode(_Rootnode))
//			{	// copy a node, then any subtrees
//			_Nodeptr _Pnode = _Buynode(_Myhead, _Wherenode, _Myhead,
//				_Myval(_Rootnode), _Color(_Rootnode));
//			if (_Isnode(_Newroot))
//				_Newroot = _Pnode;	// memorize new root
//
//			_TRY_BEGIN
//			_Left(_Pnode) = _Copy(_Left(_Rootnode), _Pnode);
//			_Right(_Pnode) = _Copy(_Right(_Rootnode), _Pnode);
//			_CATCH_ALL
//			_Erase(_Newroot);	// subtree copy failed, bail out
//			_RERAISE;
//			_CATCH_END
//			}
//
//		return (_Newroot);	// return newly constructed tree
//		}

	void _Erase(_Nodeptr _Rootnode)
		{	// free entire subtree, recursively
			if (_Isnode(_Rootnode))
			{
				if (_Isnode(_Left(_Rootnode)))
					_Erase(_Left(_Rootnode));

				if (_Isnode(_Right(_Rootnode)))
					_Erase(_Right(_Rootnode));

				if (_Myroot == _Rootnode)
					_Myroot = _Myhead;

				if (_Left(_Parent(_Rootnode)) == _Rootnode)
					_Left(_Parent(_Rootnode)) = _Myhead;
				else	_Right(_Parent(_Rootnode)) = _Myhead;

				if (_Left(_Myhead) == _Rootnode)
				{
					iterator _Where = _TREE_ITERATOR(_Rootnode);
					_Where++;
					_Lmost() = _Where._Mynode();
				}

				if (_Right(_Myhead) == _Rootnode)
				{
					iterator _Where = _TREE_ITERATOR(_Rootnode);
					_Where--;
					_Lmost() = _Where._Mynode();
				}
				--_Mysize;
				this->_Alnod.destroy((_NodeValue*)_Rootnode);
				this->_Alnod.deallocate((_NodeValue*)_Rootnode, 1);
			}
		}

	void _Init()
		{
		_Myhead = _Buynode();
		//_Isnode(_Myhead) = false;
		_Myroot = _Myhead;
		_Root() = _Myhead;
		_Lmost() = _Myhead, _Rmost() = _Myhead;
		_Mysize = 0;
		}

	iterator _Insert(bool _Addleft, _Nodeptr _Wherenode,
		const value_type& _Val)
		{	// add node with value next to _Wherenode, to left if _Addnode
		_Nodeptr _Newnode = _Buynode(_Myhead, _Wherenode, _Myhead,
			_Val, _Red);

		++_Mysize;
		if (_Wherenode == _Myhead)
			{	// first node in tree, just set head values
			_Myroot = _Newnode;
			_Lmost() = _Newnode, _Rmost() = _Newnode;
			}
		else if (_Addleft)
			{	// add to left of _Wherenode
			_Left(_Wherenode) = _Newnode;
			if (_Wherenode == _Lmost())
				_Lmost() = _Newnode;
			}
		else
			{	// add to right of _Wherenode
			_Right(_Wherenode) = _Newnode;
			if (_Wherenode == _Rmost())
				_Rmost() = _Newnode;
			}

		for (_Nodeptr _Pnode = _Newnode; _Color(_Parent(_Pnode)) == _Red; )
			if (_Parent(_Pnode) == _Left(_Parent(_Parent(_Pnode))))
				{	// fixup red-red in left subtree
				_Wherenode = _Right(_Parent(_Parent(_Pnode)));
				if (_Color(_Wherenode) == _Red)
					{	// parent has two red children, blacken both
					_Color(_Parent(_Pnode)) = _Black;
					_Color(_Wherenode) = _Black;
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Pnode = _Parent(_Parent(_Pnode));
					}
				else
					{	// parent has red and black children
					if (_Pnode == _Right(_Parent(_Pnode)))
						{	// rotate right child to left
						_Pnode = _Parent(_Pnode);
						_Lrotate(_Pnode);
						}
					_Color(_Parent(_Pnode)) = _Black;	// propagate red up
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Rrotate(_Parent(_Parent(_Pnode)));
					}
				}
			else
				{	// fixup red-red in right subtree
				_Wherenode = _Left(_Parent(_Parent(_Pnode)));
				if (_Color(_Wherenode) == _Red)
					{	// parent has two red children, blacken both
					_Color(_Parent(_Pnode)) = _Black;
					_Color(_Wherenode) = _Black;
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Pnode = _Parent(_Parent(_Pnode));
					}
				else
					{	// parent has red and black children
					if (_Pnode == _Left(_Parent(_Pnode)))
						{	// rotate left child to right
						_Pnode = _Parent(_Pnode);
						_Rrotate(_Pnode);
						}
					_Color(_Parent(_Pnode)) = _Black;	// propagate red up
					_Color(_Parent(_Parent(_Pnode))) = _Red;
					_Lrotate(_Parent(_Parent(_Pnode)));
					}
				}

		_Color(_Myroot) = _Black;	// root is always black
		return (_TREE_ITERATOR(_Newnode));
		}

	_Nodeptr _Lbound(const key_type& _Keyval) const
		{	// find leftmost node not less than _Keyval
		_Nodeptr _Pnode = _Myroot;
		_Nodeptr _Wherenode = _Myhead;	// end() if search fails

		while (_Isnode(_Pnode))
			if (this->comp(_Key(_Pnode), _Keyval))
				_Pnode = _Right(_Pnode);	// descend right subtree
			else
				{	// _Pnode not less than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}

		return (_Wherenode);	// return best remembered candidate
		}

	_Nodeptr& _Lmost() { return (_Left(_Myhead)); }
	_Nodeptr& _Lmost() const { return (_Left(_Myhead)); }

	void _Lrotate(_Nodeptr _Wherenode)
		{	// promote right node to root of subtree
		_Nodeptr _Pnode = _Right(_Wherenode);
		_Right(_Wherenode) = _Left(_Pnode);

		if (_Isnode(_Left(_Pnode)))
			_Parent(_Left(_Pnode)) = _Wherenode;
		_Parent(_Pnode) = _Parent(_Wherenode);

		if (_Wherenode == _Myroot)
			_Myroot = _Pnode;
		else if (_Wherenode == _Left(_Parent(_Wherenode)))
			_Left(_Parent(_Wherenode)) = _Pnode;
		else
			_Right(_Parent(_Wherenode)) = _Pnode;

		_Left(_Pnode) = _Wherenode;
		_Parent(_Wherenode) = _Pnode;
		}

	static _Nodeptr _Max(_Nodeptr _Pnode)
		{	// return rightmost node in subtree at _Pnode
		while (_Isnode(_Right(_Pnode)))
			_Pnode = _Right(_Pnode);
		return (_Pnode);
		}

	static _Nodeptr _Min(_Nodeptr _Pnode)
		{	// return leftmost node in subtree at _Pnode
		while (_Isnode(_Left(_Pnode)))
			_Pnode = _Left(_Pnode);
		return (_Pnode);
		}

	_Nodeptr& _Rmost() { return (_Right(_Myhead)); }
	_Nodeptr& _Rmost() const { return (_Right(_Myhead)); }

	_Nodeptr& _Root() { return (_Parent(_Myhead)); }
	_Nodeptr& _Root() const	{ return (_Parent(_Myhead)); }

	void _Rrotate(_Nodeptr _Wherenode)
		{	// promote left node to root of subtree
		_Nodeptr _Pnode = _Left(_Wherenode);
		_Left(_Wherenode) = _Right(_Pnode);

		if (_Isnode(_Right(_Pnode)))
			_Parent(_Right(_Pnode)) = _Wherenode;
		_Parent(_Pnode) = _Parent(_Wherenode);

		if (_Wherenode == _Myroot)
			_Myroot = _Pnode;
		else if (_Wherenode == _Right(_Parent(_Wherenode)))
			_Right(_Parent(_Wherenode)) = _Pnode;
		else
			_Left(_Parent(_Wherenode)) = _Pnode;

		_Right(_Pnode) = _Wherenode;
		_Parent(_Wherenode) = _Pnode;
		}

	_Nodeptr _Ubound(const key_type& _Keyval) const
		{	// find leftmost node greater than _Keyval
		_Nodeptr _Pnode = _Myroot;
		_Nodeptr _Wherenode = _Myhead;	// end() if search fails

		while (_Isnode(_Pnode))
			if (this->comp(_Keyval, _Key(_Pnode)))
				{	// _Pnode greater than _Keyval, remember it
				_Wherenode = _Pnode;
				_Pnode = _Left(_Pnode);	// descend left subtree
				}
			else
				_Pnode = _Right(_Pnode);	// descend right subtree

		return (_Wherenode);	// return best remembered candidate
		}

	_Nodeptr _Buynode()
		{	// allocate a head/nil node
		_Nodeptr _Wherenode = &_Base;
		_Color(_Wherenode) = _Black;
		_Isnode(_Wherenode) = false;
		return (_Wherenode);
		}

	_Nodeptr _Buynode(_Nodeptr _Larg, _Nodeptr _Parg,
		_Nodeptr _Rarg, const value_type& _Val, char _Carg)
		{	// allocate a node with pointers, value, and color
		_Nodeptr _Wherenode = this->_Alnod.allocate(1);
//		_TRY_BEGIN
		new (_Wherenode) _NodeValue(_Larg, _Parg, _Rarg, _Val, _Carg);
//		_CATCH_ALL
//		this->_Alnod.deallocate(_Wherenode, 1);
//		_RERAISE;
//		_CATCH_END
		return (_Wherenode);
		}

//	void _Tidy()
//		{	// free all storage
//		erase(begin(), end());
//		this->_Alptr.destroy(&_Left(_Myhead));
//		this->_Alptr.destroy(&_Parent(_Myhead));
//		this->_Alptr.destroy(&_Right(_Myhead));
//		this->_Alnod.deallocate(_Myhead, 1);
//		_Myhead = 0, _Mysize = 0;
//		}

	_Node _Base;
	_Nodeptr _Myroot;
	_Nodeptr _Myhead;
	size_type _Mysize;
	};

//TMap
template<class _Ty1, class _Ty2>
	struct _Tmap_pair
	{
	typedef _Tmap_pair<_Ty1, _Ty2> _Myt;
	typedef _Ty1 first_type;
	typedef _Ty2 second_type;

	_Tmap_pair()
		: first(_Ty1()), second(_Ty2())
		{
		}

	_Tmap_pair(const _Ty1& _Val1, const _Ty2& _Val2)
		: first(_Val1), second(_Val2)
		{
		}

	_Tmap_pair(const _Ty1& _Val1, const _Ty2* _Val2 = nullptr)
		: first(_Val1)//, second(_Val2)
		{
			if (_Val2) second = *_Val2;
		}

	template<class _Other1, class _Other2>
		_Tmap_pair(const _Tmap_pair<_Other1, _Other2>& _Right)
		: first(_Right.first), second(_Right.second)
		{
		}

//	void swap(_Myt& _Right)
//		{
//		std::swap(first, _Right.first);
//		std::swap(second, _Right.second);
//		}

	_Ty1 first;
	_Ty2 second;
	};

//TMap
template<class _Kty,	// key type
	class _Ty,	// mapped type
	class _Pr,	// comparator predicate type
	class _Alloc,	// actual allocator type (should be value allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
	class _Tmap_traits
	{
public:
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;
	typedef _Tmap_pair<const _Kty, _Ty> value_type;
	typedef _Pr key_compare;
	typedef typename _Alloc::template rebind<value_type>::other allocator_type;
	typedef _POINTER_X(value_type, allocator_type) _ITptr;
	typedef _REFERENCE_X(value_type, allocator_type) _IReft;

	enum {_Multi = _Mfl};

	_Tmap_traits()
		: comp()
		{
		}

	_Tmap_traits(_Pr _Parg)
		: comp(_Parg)
		{
		}

	class value_compare : public binary_function<value_type, value_type, bool>
		{
		friend class _Tmap_traits<_Kty, _Ty, _Pr, _Alloc, _Mfl>;

	public:
		bool operator()(const value_type& _Left, const value_type& _Right) const
			{
			return (comp(_Left.first, _Right.first));
			}

		value_compare(key_compare _Pred)
			: comp(_Pred)
			{
			}
	protected:
		key_compare comp;
		};

	static const _Kty& _Kfn(const value_type& _Val)	{ return (_Val.first); }
	static const _Ty& _Tfn(const value_type& _Val)	{ return (_Val.second); }

	_Pr comp;
	};

		// TEMPLATE CLASS map
template<class _Kty,
	class _Ty,
	class _Pr = less<_Kty>,
	class _Alloc = allocator<_Tmap_pair<const _Kty, _Ty> > >
	class map : public _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> >
	{
public:
	typedef map<_Kty, _Ty, _Pr, _Alloc> _Myt;
	typedef _Tree<_Tmap_traits<_Kty, _Ty, _Pr, _Alloc, false> > _Mybase;
	typedef _Kty key_type;
	typedef _Ty mapped_type;
	typedef _Ty referent_type;
	typedef _Pr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator IT; //iterator;
	typedef typename _Mybase::const_iterator const_iterator;
//	typedef typename _Mybase::reverse_iterator reverse_iterator;
//	typedef typename _Mybase::const_reverse_iterator const_reverse_iterator;
	typedef typename _Mybase::value_type value_type;

	map()
		: _Mybase(key_compare(), allocator_type())
		{
		}

	explicit map(const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{
		}

	map(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{
		}

	template<class _Iter>
		map(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct map from [_First, _Last), defaults
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}

	template<class _Iter>
		map(_Iter _First, _Iter _Last,	const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{	// construct map from [_First, _Last), comparator
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}

	template<class _Iter>
		map(_Iter _First, _Iter _Last, const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{	// construct map from [_First, _Last), comparator, and allocator
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}

	mapped_type& operator[](const key_type& _Keyval)
		{
		iterator _Where = this->lower_bound(_Keyval);
		if (_Where == this->end() || this->comp(_Keyval, this->_Key(_Where._Mynode())))
			_Where = this->insert(_Where,
				value_type(_Keyval, mapped_type()));
		return ((*_Where).second);
		}

		IT Insert(const key_type &Key, mapped_type Value)
		{
			return this->insert(_Tmap_pair<const key_type, mapped_type>(Key, Value));
		}

		IT Insert(const key_type &Key)
		{
			return this->insert(_Tmap_pair<const key_type, mapped_type>(Key, nullptr));
		}
	};

//template<class _Kty, class _Ty, class _Pr, class _Alloc> inline
//	void swap(map<_Kty, _Ty, _Pr, _Alloc>& _Left, map<_Kty, _Ty, _Pr, _Alloc>& _Right)
//	{
//		_Left.swap(_Right);
//	}

//TSet
template<class _Kty,	// key/value type
	class _Pr,	// comparator predicate type
	class _Alloc,	// actual allocator type (should be value allocator)
	bool _Mfl>	// true if multiple equivalent keys are permitted
	class _Tset_traits
	{
public:
	typedef _Kty key_type;
	typedef _Kty mapped_type;
	typedef _Kty value_type;
	typedef _Pr key_compare;
	typedef typename _Alloc::template rebind<value_type>::other allocator_type;
	typedef _POINTER_X(value_type, allocator_type) _ITptr;
	typedef _REFERENCE_X(value_type, allocator_type) _IReft;

	enum {_Multi = _Mfl};

	_Tset_traits()
		: comp()
		{
		}

	_Tset_traits(_Pr _Parg)
		: comp(_Parg)
		{
		}

	typedef key_compare value_compare;

	static const _Kty& _Kfn(const value_type& _Val)	{ return (_Val); }

	_Pr comp;
	};

		// TEMPLATE CLASS set
template<class _Kty,
	class _Pr = less<_Kty>,
	class _Alloc = allocator<_Kty> >
	class set
		: public _Tree<_Tset_traits<_Kty, _Pr, _Alloc, false> >
	{	// ordered red-black tree of key values, unique keys
public:
	typedef set<_Kty, _Pr, _Alloc> _Myt;
	typedef _Tree<_Tset_traits<_Kty, _Pr, _Alloc, false> > _Mybase;
	typedef _Kty key_type;
	typedef _Pr key_compare;
	typedef typename _Mybase::value_compare value_compare;
	typedef typename _Mybase::allocator_type allocator_type;
	typedef typename _Mybase::size_type size_type;
	typedef typename _Mybase::difference_type difference_type;
	typedef typename _Mybase::pointer pointer;
	typedef typename _Mybase::const_pointer const_pointer;
	typedef typename _Mybase::reference reference;
	typedef typename _Mybase::const_reference const_reference;
	typedef typename _Mybase::iterator IT; //iterator;
	typedef typename _Mybase::const_iterator const_iterator;
//	typedef typename _Mybase::reverse_iterator reverse_iterator;
//	typedef typename _Mybase::const_reverse_iterator const_reverse_iterator;
	typedef typename _Mybase::value_type value_type;

	set()
		: _Mybase(key_compare(), allocator_type())
		{
		}

	explicit set(const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{
		}

	set(const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{
		}

	template<class _Iter>
		set(_Iter _First, _Iter _Last)
		: _Mybase(key_compare(), allocator_type())
		{	// construct set from [_First, _Last), defaults
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}

	template<class _Iter>
		set(_Iter _First, _Iter _Last,
			const key_compare& _Pred)
		: _Mybase(_Pred, allocator_type())
		{	// construct set from [_First, _Last), comparator
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}

	template<class _Iter>
		set(_Iter _First, _Iter _Last,
			const key_compare& _Pred, const allocator_type& _Al)
		: _Mybase(_Pred, _Al)
		{	// construct set from [_First, _Last), defaults, and allocator
		for (; _First != _Last; ++_First)
			this->insert(*_First);
		}
	};

//template<class _Kty,
//	class _Pr,
//	class _Alloc> inline
//	void swap(set<_Kty, _Pr, _Alloc>& _Left,
//		set<_Kty, _Pr, _Alloc>& _Right)
//	{	// swap _Left and _Right sets
//	_Left.swap(_Right);
//	}

#endif