
#ifndef _ALT_LIST_HPP_
#define _ALT_LIST_HPP_

#include "../define.h"

struct TListNode
{
	TListNode *Next;
	TListNode *Prev;
};

template <typename T>
struct TListNodeValue : public TListNode
{
	T Value;
};

template <class T>
struct TListIter
{
public:
	typedef TListNodeValue<T> NODE;
	TListIter() { pNode = 0; }
	TListIter(NODE *Node) { pNode = Node; }
//	TListIter(T *Value) { pNode =  CONTAINING_RECORD_MY(Value, NODE, sizeof(TListNode)); }
	TListIter(T *Value) { pNode =  CONTAINING_RECORD_MY1(Value, NODE, Value); }
	T&  operator*() { return pNode->Value; }
	T* operator->() { return &pNode->Value; }

	T* Next() { NODE *p = (NODE*)pNode->Next; return &p->Value; }
	T* Prev() { NODE *p = (NODE*)pNode->Prev; return &p->Value; }

	bool operator==(const TListIter & Iter) const { return pNode==Iter.pNode; }
	bool operator!=(const TListIter & Iter) const { return pNode!=Iter.pNode; }
	operator bool() const { return pNode != 0; } //IsValid ?

	void Reset() { pNode = 0; }

	TListIter & operator++() //prefix ++i
	{
		pNode = (NODE*)pNode->Next;
		return *this;
	}
	TListIter & operator++(int n) //postfix i++
	{
		pNode = (NODE*)pNode->Next;
		return *this;
	}
	TListIter & operator+=(int n) //postfix i+=n
	{
		while (n-- > 0) pNode = (NODE*)pNode->Next;
		return *this;
	}
	TListIter operator+(int n) const
	{
		TListIter _Tmp = *this;
		_Tmp += n;
		return _Tmp;
	}
	TListIter & operator--() //prefix --i
	{
		pNode = (NODE*)pNode->Prev;
		return *this;
	}
	TListIter & operator--(int n) //postfix i--
	{
		pNode = (NODE*)pNode->Prev;
		return *this;
	}
	TListIter & operator-=(int n) //postfix i-=n
	{
//printf("[-= %d], ",n);
		while (n-- > 0) pNode = (NODE*)pNode->Prev;
		return *this;
	}
	TListIter operator-(int n) const
	{
//printf("[- %d], ",n);
		TListIter _Tmp = *this;
		_Tmp -= n;
		return _Tmp;
	}
//protected:
	NODE *pNode;
};

template<typename T>
struct compare_traits
{
	int operator()(const T a1, const T a2) const
	{
		return a1 != a2;
	}
};

template <typename T, typename compare = compare_traits<T> >
struct TList : public TListNode
{
public:
	typedef TListNodeValue<T> NODE;
	typedef TListIter<T> IT;
	TList()
	{
		Init();
	}
	TList(const TList & other)
	{
		Init();//Clear();
		if (other.Size())
		{
			for (IT it = other.Begin(); it != other.End(); ++it)
				Append(&*it);
		}
	}
	~TList()
	{
		Clear();
	}
	IT Begin() const { return (NODE*)pHead->Next; }
	IT End() const { return (NODE*)pHead; }
	int Size() const { return nSize; }
	bool Empty() const { return nSize == 0; }
	IT Find(const T & Value) const
	{
		IT Iter = Begin();
		while (Iter != End() &&
			//*Iter != Value)
			(compare()(*Iter, Value))!=0) //compare_not_eq
			++Iter;
		return Iter;
	}
	IT _Search(IT _First, IT _Last, const T & _Val) const
	{
		unsigned long _Count = 0;
		//_Distance(_First, _Last, _Count);
		IT First = _First;
		for (; First != _Last; ++First)
			++_Count;

		for (; 0 < _Count; )
		{       // divide and conquer, find half that contains answer
			unsigned long _Count2 = _Count / 2;
			IT _Mid = _First;
			//std::advance(_Mid, _Count2);
			unsigned long _Off = _Count2;
			for (; 0 < _Off; --_Off)
				++_Mid;

			//if (!(*_Mid < _Val)) //UPPER
			if (*_Mid < _Val) //LOWER
				_First = ++_Mid, _Count -= _Count2 + 1;
			else
				_Count = _Count2;
		}
		return (_First);		
	}
	IT Search(const T & Value) const
	{
		return _Search(Begin(), End(), Value);
	}
	IT AppendSort(const T &Value)
	{
		return InsertBefore(Search(Value), &Value);
	}
	
	IT operator[](int nIndex) const
	{
		if (nIndex==nSize-1)
		{
	//printf("[%d]==%d-1 ",nIndex,nSize-1);
			return End()-1;
		}
		if (nIndex>=nSize)
		{
	//printf("[%d]>=%d ",nIndex,nSize);
			return End();
		}
		if (nIndex>0)
		{
	//printf("[%d]>0 ",nIndex);
			return Begin()+(nIndex);
		}
	//printf("[%d]<=0 ",nIndex);
		return Begin()-(-nIndex);
	}
	//Cut [First,Last] and move to after Iter+[First,Last]
	int Splice(IT & First, IT & Last, TList & List, IT & Iter) //const,const,,const - clang
	{
		int n = 0;
		IT it=First;
		while (it != Last) it++,n++;
		if (n==0) return 0;

		NODE *tmp = (NODE*)Last.pNode->Prev;
		First.pNode->Prev->Next = Last.pNode;
		Last.pNode->Prev = First.pNode->Prev;
		
		nSize -= n;

		First.pNode->Prev = Iter.pNode;
		tmp->Next = Iter.pNode->Next;
		Iter.pNode->Next = First.pNode;
		tmp->Next->Prev = tmp;

		List.nSize += n;

//		First.pNode->Prev->Next = Last.pNode;
//		Last.pNode->Prev->Next = Iter.pNode;
//		Iter.pNode->Prev->Next = First.pNode;
//		NODE *Pnode = (NODE*)Iter.pNode->Prev;
//		Iter.pNode->Prev = Last.pNode->Prev;
//		Last.pNode->Prev = First.pNode->Prev;
//		First.pNode->Prev = Pnode;
		return n;
	}

	//Cut [First,Last] and move to after Iter+[First,Last]
	int _Splice(IT & First, IT & Last, TList & List, IT & Iter) //const,const,,const - clang
	{
		int n = 1;
	//	IT it=First;
	//	while (it != Last) it++,n++;
	//	if (n==0) return 0;

		NODE *tmp = (NODE*)Last.pNode->Prev;
		First.pNode->Prev->Next = Last.pNode;
		Last.pNode->Prev = First.pNode->Prev;
		
		nSize -= n;

		First.pNode->Prev = Iter.pNode;
		tmp->Next = Iter.pNode->Next;
		Iter.pNode->Next = First.pNode;
		tmp->Next->Prev = tmp;

		List.nSize += n;

//		First.pNode->Prev->Next = Last.pNode;
//		Last.pNode->Prev->Next = Iter.pNode;
//		Iter.pNode->Prev->Next = First.pNode;
//		NODE *Pnode = (NODE*)Iter.pNode->Prev;
//		Iter.pNode->Prev = Last.pNode->Prev;
//		Last.pNode->Prev = First.pNode->Prev;
//		First.pNode->Prev = Pnode;
		return n;
	}

	void Reverse()
	{
		if (2 <= Size())
		{
			IT Last = End();
			IT Next = ++Begin();
			while (Next != Last)
			{
				IT Before = Next;
				IT End = this->End();
				_Splice(Before, ++Next, *this, End);//Begin());
			}
		}
	}

	IT Append(const T *Value = 0)
	{
		NODE *Pnode = (NODE *)pHead;
		NODE *Newnode = new NODE;
		if (Value) Newnode->Value = *Value;
		if (nSize == 0) Pnode->Next = Newnode;
		Newnode->Next = Pnode;
		Newnode->Prev = Pnode->Prev;
		Newnode->Prev->Next = Newnode;
		Pnode->Prev = Newnode;
		++nSize;
		return Newnode;
	}
	IT InsertBefore(IT Iter, const T *Value = 0)
	{
		return InsertAfter(Iter-1, Value);
	}
//	IT InsertAfterNext(IT Iter, const T *Value = 0)
//	{
//		return InsertAfter(Iter+1, Value);
//	}
	IT InsertAfter(IT Iter, const T *Value = 0)
	{
		NODE *Pnode = Iter.pNode;
		NODE *Newnode = new NODE;
		if (Value) Newnode->Value = *Value;
		Newnode->Next = Pnode->Next;
		Newnode->Prev = Pnode;
		Pnode->Next->Prev = Newnode;
		Pnode->Next = Newnode;
		++nSize;
		return Newnode;
	}
//	IT push_back(IT Iter, const T *Value = 0)
//	{
//		NODE *Pnode = Iter.pNode;
//		NODE *Newnode = new NODE;
//		if (Value) Newnode->Value = *Value;
//		Newnode->Next = Pnode->Next->Next;
//		Newnode->Prev = Pnode->Next;
//		Pnode->Next->Next->Prev = Newnode;
//		Pnode->Next->Next = Newnode;
//		++Size;
//		return Newnode;
//	}
	IT Remove(IT First, IT Last)
	{
		IT it = First;
		while (it != Last)
			it = Remove(it)++;
		return First-1;
	}
	IT Remove(IT Iter)
	{
		NODE *Pnode = Iter.pNode;
		if (Pnode != pHead)
		{
			Pnode->Prev->Next = Pnode->Next;
			Pnode->Next->Prev = Pnode->Prev;
			NODE *ret = (NODE*)Pnode->Prev;
			delete Pnode;
			Pnode = ret;
			--nSize;
		}
		return Pnode;
	}
	IT Remove(const T & Value)
	{
		return Remove(Find(Value));
	}
	void Init()
	{
		pHead = this;
		pHead->Next = pHead;
		pHead->Prev = pHead;
		nSize = 0;
	}
	void Clear()
	{
		NODE *Pnext;
		NODE *Pnode = (NODE*)pHead->Next;
		for (;Pnode != (NODE*)pHead; Pnode = Pnext)
		{
			Pnext = (NODE*)Pnode->Next;
			delete Pnode;
		}
		Init();
	}
protected:
	TListNode *pHead;
	int nSize;
};

#endif

//#define TEST
#ifdef TEST
#include <stdio.h>
#include <stdlib.h>

int main()
{
	TList<long> List;
/////////////
	printf("Size %d\n", List.Size());
	printf("Fill\n");
	for (long i=0;i<20;++i)
	{
		printf("%d,",i);
		List.Append(&i);
	}
	printf("\n");
///////////
	printf("Show[-2]=%d,[-1]=%d,[0]=%d,[1]=%d %d\n", *List[-2],*List[-1],*List[-0],*List[1],List.Size());
#if 1
#if 0
///////////
	printf("Size %d\n", List.Size());
	printf("ShowList\n");
	for (auto it = List.Begin(); it!=List.End();++it)
	{
		printf("%d,", *it);
	}
	printf("\n");
///////////
	printf("Size %d\n", List.Size());
	printf("ShowListReverse\n");
	int j = 40;
	for (auto it = List.Begin(); j-- >0;++it)
	{
		printf("%d,", *it);
	}
	printf("\n");
///////////
	printf("Size %d\n", List.Size());
	printf("Show[]\n");
	for (long i=-20;i<20;++i)
	{
		printf("%d,", *(List[i]));
	}
	printf("\n");
///////////////
	long i = 66;
	List.InsertAfter(List[7],&i);

	printf("InsertAfter[7],%d Size %d\n", i,List.Size());
	printf("Show[]\n");
	for (long i=-20;i<20;++i)
	{
		printf("%d,", *(List[i]));
	}
	printf("\n");
////////////////
	List.Remove(List[3]);

	printf("Remove[3] Size %d\n", List.Size());
	printf("Show[]\n");
	for (long i=-20;i<20;++i)
	{
		printf("%d,", *(List[i]));
	}
	printf("\n");
///////////
	List.Remove(List[1], List[9]);

	printf("Remove[1],[9] Size %d\n", List.Size());
	printf("Show[]\n");
	for (long i=-20;i<20;++i)
	{
		printf("%d,", *(List[i]));
	}
	printf("\n");
#endif
/////////////
	TList<long> List2;
	printf("Splice [10] %d,[15] %d ", *List[10],*List[15]);
	List.Splice(List[10],List[15],List2,List2.End());

	printf("Size %d,Size2 %d\n", List.Size(),List2.Size());
	printf("Show[]\n");
	for (long i=0;i<20;++i)
	{
		printf("%d,", *(List[i]));
	}
	printf("\n");
	printf("Show2[]\n");
	for (long i=0;i<20;++i)
	{
		printf("%d,", *(List2[i]));
	}
	printf("\n");
#endif
return 0;
}
#endif
