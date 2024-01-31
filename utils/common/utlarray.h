//====== Copyright � 1996-2005, Valve Corporation, All rights reserved. =======//
//
// Purpose:
//
// $NoKeywords: $
//
// A growable array class that maintains a free list and keeps elements
// in the same location
//=============================================================================//

#ifndef UTLVECTOR_H
#define UTLVECTOR_H

#ifdef _WIN32
#pragma once
#endif

#include <assert.h>
#include <string.h>
#include "utlmemory.h"
#include "utlblockmemory.h"

#define FOR_EACH_VEC( vecName, iteratorName ) \
	for ( int iteratorName = 0; iteratorName < vecName.Count(); iteratorName++ )

//-----------------------------------------------------------------------------
// The CUtlArray class:
// A growable array class which doubles in size by default.
// It will always keep all elements consecutive in memory, and may move the
// elements around in memory (via a PvRealloc) when elements are inserted or
// removed. Clients should therefore refer to the elements of the vector
// by index (they should *never* maintain pointers to elements in the vector).
//-----------------------------------------------------------------------------
template< class T, class A = CUtlMemory<T> >
class CUtlArray
{
	typedef A CAllocator;
public:
	typedef T ElemType_t;

	// constructor, destructor
	CUtlArray( int growSize = 0, int initSize = 0 );
	CUtlArray( T* pMemory, int allocationCount, int numElements = 0 );
	~CUtlArray();

	// Copy the array.
	CUtlArray<T, A>& operator=( const CUtlArray<T, A> &other );

	// element access
	T& operator[]( int i );
	const T& operator[]( int i ) const;
	T& Element( int i );
	const T& Element( int i ) const;
	T& Head();
	const T& Head() const;
	T& Tail();
	const T& Tail() const;

	// Gets the base address (can change when adding elements!)
	T* Base()								{ return m_Memory.Base(); }
	const T* Base() const					{ return m_Memory.Base(); }

	// Returns the number of elements in the vector
	// SIZE IS DEPRECATED!
	int Count() const;
	int Size() const;	// don't use me!

	// Is element index valid?
	bool IsValidIndex( int i ) const;
	static int InvalidIndex();

	// Adds an element, uses default constructor
	int AddToHead();
	int AddToTail();
	int InsertBefore( int elem );
	int InsertAfter( int elem );

	// Adds an element, uses copy constructor
	int AddToHead( const T& src );
	int AddToTail( const T& src );
	int InsertBefore( int elem, const T& src );
	int InsertAfter( int elem, const T& src );

	// Adds multiple elements, uses default constructor
	int AddMultipleToHead( int num );
	int AddMultipleToTail( int num, const T *pToCopy=NULL );
	int InsertMultipleBefore( int elem, int num, const T *pToCopy=NULL );	// If pToCopy is set, then it's an array of length 'num' and
	int InsertMultipleAfter( int elem, int num );

	// Calls RemoveAll() then AddMultipleToTail.
	void SetSize( int size );
	void SetCount( int count );

	// Calls SetSize and copies each element.
	void CopyArray( const T *pArray, int size );

	// Fast swap
	void Swap( CUtlArray< T, A > &vec );

	// Add the specified array to the tail.
	int AddVectorToTail( CUtlArray<T, A> const &src );

	// Finds an element (element needs operator== defined)
	int Find( const T& src ) const;

	bool HasElement( const T& src ) const;

	// Makes sure we have enough memory allocated to store a requested # of elements
	void EnsureCapacity( int num );

	// Makes sure we have at least this many elements
	void EnsureCount( int num );

	// Element removal
	void FastRemove( int elem );	// doesn't preserve order
	void Remove( int elem );		// preserves order, shifts elements
	bool FindAndRemove( const T& src );	// removes first occurrence of src, preserves order, shifts elements
	void RemoveMultiple( int elem, int num );	// preserves order, shifts elements
	void RemoveAll();				// doesn't deallocate memory

	// Memory deallocation
	void Purge();

	// Purges the list and calls delete on each element in it.
	void PurgeAndDeleteElements();

	// Compacts the vector to the number of elements actually in use
	void Compact();

	// Set the size by which it grows when it needs to allocate more memory.
	void SetGrowSize( int size )			{ m_Memory.SetGrowSize( size ); }

	int NumAllocated() const;	// Only use this if you really know what you're doing!

	void Sort( int (__cdecl *pfnCompare)(const T *, const T *) );

#ifdef DBGFLAG_VALIDATE
	void Validate( CValidator &validator, char *pchName );		// Validate our internal structures
#endif // DBGFLAG_VALIDATE

protected:
	// Can't copy this unless we explicitly do it!
	CUtlArray( CUtlArray const& vec ) { assert(0); }

	// Grows the vector
	void GrowVector( int num = 1 );

	// Shifts elements....
	void ShiftElementsRight( int elem, int num = 1 );
	void ShiftElementsLeft( int elem, int num = 1 );

	CAllocator m_Memory;
	int m_Size;

	// For easier access to the elements through the debugger
	// it's in release builds so this can be used in libraries correctly
	T *m_pElements;

	inline void ResetDbgInfo()
	{
		m_pElements = Base();
	}
};


// this is kind of ugly, but until C++ gets templatized typedefs in C++0x, it's our only choice
template < class T >
class CUtlBlockVector : public CUtlArray< T, CUtlBlockMemory< T, int > >
{
public:
	CUtlBlockVector( int growSize = 0, int initSize = 0 )
		: CUtlArray< T, CUtlBlockMemory< T, int > >( growSize, initSize ) {}
};

//-----------------------------------------------------------------------------
// The CUtlArrayFixed class:
// A array class with a fixed allocation scheme
//-----------------------------------------------------------------------------

//template< class BASE_UTLVECTOR, class MUTEX_TYPE = CThreadFastMutex >
template< class BASE_UTLVECTOR, class MUTEX_TYPE >
class CUtlArrayMT : public BASE_UTLVECTOR, public MUTEX_TYPE
{
	typedef BASE_UTLVECTOR BaseClass;
public:
	MUTEX_TYPE Mutex_t;

	// constructor, destructor
	CUtlArrayMT( int growSize = 0, int initSize = 0 ) : BaseClass( growSize, initSize ) {}
	CUtlArrayMT( typename BaseClass::ElemType_t* pMemory, int numElements ) : BaseClass( pMemory, numElements ) {}
};


//-----------------------------------------------------------------------------
// The CUtlArrayFixed class:
// A array class with a fixed allocation scheme
//-----------------------------------------------------------------------------
template< class T, size_t MAX_SIZE >
class CUtlArrayFixed : public CUtlArray< T, CUtlMemoryFixed<T, MAX_SIZE > >
{
	typedef CUtlArray< T, CUtlMemoryFixed<T, MAX_SIZE > > BaseClass;
public:

	// constructor, destructor
	CUtlArrayFixed( int growSize = 0, int initSize = 0 ) : BaseClass( growSize, initSize ) {}
	CUtlArrayFixed( T* pMemory, int numElements ) : BaseClass( pMemory, numElements ) {}
};


//-----------------------------------------------------------------------------
// The CUtlArrayFixed class:
// A array class with a fixed allocation scheme
//-----------------------------------------------------------------------------
template< class T, size_t MAX_SIZE >
class CUtlArrayFixedGrowable : public CUtlArray< T, CUtlMemoryFixedGrowable<T, MAX_SIZE > >
{
	typedef CUtlArray< T, CUtlMemoryFixedGrowable<T, MAX_SIZE > > BaseClass;

public:
	// constructor, destructor
	CUtlArrayFixedGrowable( int growSize = 0 ) : BaseClass( growSize, MAX_SIZE ) {}
};


//-----------------------------------------------------------------------------
// The CCopyableUtlVector class:
// A array class that allows copy construction (so you can nest a CUtlArray inside of another one of our containers)
//  WARNING - this class lets you copy construct which can be an expensive operation if you don't carefully control when it happens
// Only use this when nesting a CUtlArray() inside of another one of our container classes (i.e a CUtlMap)
//-----------------------------------------------------------------------------
template< class T >
class CCopyableUtlVector : public CUtlArray< T, CUtlMemory<T> >
{
	typedef CUtlArray< T, CUtlMemory<T> > BaseClass;
public:
	CCopyableUtlVector( int growSize = 0, int initSize = 0 ) : BaseClass( growSize, initSize ) {}
	CCopyableUtlVector( T* pMemory, int numElements ) : BaseClass( pMemory, numElements ) {}
	virtual ~CCopyableUtlVector() {}
	CCopyableUtlVector( CCopyableUtlVector const& vec ) { CopyArray( vec.Base(), vec.Count() ); }
};

//-----------------------------------------------------------------------------
// constructor, destructor
//-----------------------------------------------------------------------------
template< typename T, class A >
inline CUtlArray<T, A>::CUtlArray( int growSize, int initSize )	:
	m_Memory(growSize, initSize), m_Size(0)
{
	ResetDbgInfo();
}

template< typename T, class A >
inline CUtlArray<T, A>::CUtlArray( T* pMemory, int allocationCount, int numElements )	:
	m_Memory(pMemory, allocationCount), m_Size(numElements)
{
	ResetDbgInfo();
}

template< typename T, class A >
inline CUtlArray<T, A>::~CUtlArray()
{
	Purge();
}

template< typename T, class A >
inline CUtlArray<T, A>& CUtlArray<T, A>::operator=( const CUtlArray<T, A> &other )
{
	int nCount = other.Count();
	SetSize( nCount );
	for ( int i = 0; i < nCount; i++ )
	{
		(*this)[ i ] = other[ i ];
	}
	return *this;
}


//-----------------------------------------------------------------------------
// element access
//-----------------------------------------------------------------------------
template< typename T, class A >
inline T& CUtlArray<T, A>::operator[]( int i )
{
	return m_Memory[ i ];
}

template< typename T, class A >
inline const T& CUtlArray<T, A>::operator[]( int i ) const
{
	return m_Memory[ i ];
}

template< typename T, class A >
inline T& CUtlArray<T, A>::Element( int i )
{
	return m_Memory[ i ];
}

template< typename T, class A >
inline const T& CUtlArray<T, A>::Element( int i ) const
{
	return m_Memory[ i ];
}

template< typename T, class A >
inline T& CUtlArray<T, A>::Head()
{
	assert( m_Size > 0 );
	return m_Memory[ 0 ];
}

template< typename T, class A >
inline const T& CUtlArray<T, A>::Head() const
{
	assert( m_Size > 0 );
	return m_Memory[ 0 ];
}

template< typename T, class A >
inline T& CUtlArray<T, A>::Tail()
{
	assert( m_Size > 0 );
	return m_Memory[ m_Size - 1 ];
}

template< typename T, class A >
inline const T& CUtlArray<T, A>::Tail() const
{
	assert( m_Size > 0 );
	return m_Memory[ m_Size - 1 ];
}


//-----------------------------------------------------------------------------
// Count
//-----------------------------------------------------------------------------
template< typename T, class A >
inline int CUtlArray<T, A>::Size() const
{
	return m_Size;
}

template< typename T, class A >
inline int CUtlArray<T, A>::Count() const
{
	return m_Size;
}


//-----------------------------------------------------------------------------
// Is element index valid?
//-----------------------------------------------------------------------------
template< typename T, class A >
inline bool CUtlArray<T, A>::IsValidIndex( int i ) const
{
	return (i >= 0) && (i < m_Size);
}


//-----------------------------------------------------------------------------
// Returns in invalid index
//-----------------------------------------------------------------------------
template< typename T, class A >
inline int CUtlArray<T, A>::InvalidIndex()
{
	return -1;
}


//-----------------------------------------------------------------------------
// Grows the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::GrowVector( int num )
{
	if (m_Size + num > m_Memory.NumAllocated())
	{
		m_Memory.Grow( m_Size + num - m_Memory.NumAllocated() );
	}

	m_Size += num;
	ResetDbgInfo();
}


//-----------------------------------------------------------------------------
// Sorts the vector
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::Sort( int (__cdecl *pfnCompare)(const T *, const T *) )
{
	typedef int (__cdecl *QSortCompareFunc_t)(const void *, const void *);
	if ( Count() <= 1 )
		return;

	if ( Base() )
	{
		qsort( Base(), Count(), sizeof(T), (QSortCompareFunc_t)(pfnCompare) );
	}
	else
	{
		assert( 0 );
		// this path is untested
		// if you want to sort vectors that use a non-sequential memory allocator,
		// you'll probably want to patch in a quicksort algorithm here
		// I just threw in this bubble sort to have something just in case...

		for ( int i = m_Size - 1; i >= 0; --i )
		{
			for ( int j = 1; j <= i; ++j )
			{
				if ( pfnCompare( &Element( j - 1 ), &Element( j ) ) < 0 )
				{
					swap( Element( j - 1 ), Element( j ) );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Makes sure we have enough memory allocated to store a requested # of elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::EnsureCapacity( int num )
{
	m_Memory.EnsureCapacity(num);
	ResetDbgInfo();
}


//-----------------------------------------------------------------------------
// Makes sure we have at least this many elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::EnsureCount( int num )
{
	if (Count() < num)
		AddMultipleToTail( num - Count() );
}


//-----------------------------------------------------------------------------
// Shifts elements
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::ShiftElementsRight( int elem, int num )
{
	assert( IsValidIndex(elem) || ( m_Size == 0 ) || ( num == 0 ));
	int numToMove = m_Size - elem - num;
	if ((numToMove > 0) && (num > 0))
		memmove( &Element(elem+num), &Element(elem), numToMove * sizeof(T) );
}

template< typename T, class A >
void CUtlArray<T, A>::ShiftElementsLeft( int elem, int num )
{
	assert( IsValidIndex(elem) || ( m_Size == 0 ) || ( num == 0 ));
	int numToMove = m_Size - elem - num;
	if ((numToMove > 0) && (num > 0))
	{
		memmove( &Element(elem), &Element(elem+num), numToMove * sizeof(T) );

#ifdef _DEBUG
		memset( &Element(m_Size-num), 0xDD, num * sizeof(T) );
#endif
	}
}


//-----------------------------------------------------------------------------
// Adds an element, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
inline int CUtlArray<T, A>::AddToHead()
{
	return InsertBefore(0);
}

template< typename T, class A >
inline int CUtlArray<T, A>::AddToTail()
{
	return InsertBefore( m_Size );
}

template< typename T, class A >
inline int CUtlArray<T, A>::InsertAfter( int elem )
{
	return InsertBefore( elem + 1 );
}

template< typename T, class A >
int CUtlArray<T, A>::InsertBefore( int elem )
{
	// Can insert at the end
	assert( (elem == Count()) || IsValidIndex(elem) );

	GrowVector();
	ShiftElementsRight(elem);
	Construct( &Element(elem) );
	return elem;
}


//-----------------------------------------------------------------------------
// Adds an element, uses copy constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
inline int CUtlArray<T, A>::AddToHead( const T& src )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count()) ) );
	return InsertBefore( 0, src );
}

template< typename T, class A >
inline int CUtlArray<T, A>::AddToTail( const T& src )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count()) ) );
	return InsertBefore( m_Size, src );
}

template< typename T, class A >
inline int CUtlArray<T, A>::InsertAfter( int elem, const T& src )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count()) ) );
	return InsertBefore( elem + 1, src );
}

template< typename T, class A >
int CUtlArray<T, A>::InsertBefore( int elem, const T& src )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || (&src < Base()) || (&src >= (Base() + Count()) ) );

	// Can insert at the end
	assert( (elem == Count()) || IsValidIndex(elem) );

	GrowVector();
	ShiftElementsRight(elem);
	CopyConstruct( &Element(elem), src );
	return elem;
}


//-----------------------------------------------------------------------------
// Adds multiple elements, uses default constructor
//-----------------------------------------------------------------------------
template< typename T, class A >
inline int CUtlArray<T, A>::AddMultipleToHead( int num )
{
	return InsertMultipleBefore( 0, num );
}

template< typename T, class A >
inline int CUtlArray<T, A>::AddMultipleToTail( int num, const T *pToCopy )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || !pToCopy || (pToCopy + num < Base()) || (pToCopy >= (Base() + Count()) ) );

	return InsertMultipleBefore( m_Size, num, pToCopy );
}

template< typename T, class A >
int CUtlArray<T, A>::InsertMultipleAfter( int elem, int num )
{
	return InsertMultipleBefore( elem + 1, num );
}


template< typename T, class A >
void CUtlArray<T, A>::SetCount( int count )
{
	RemoveAll();
	AddMultipleToTail( count );
}

template< typename T, class A >
inline void CUtlArray<T, A>::SetSize( int size )
{
	SetCount( size );
}

template< typename T, class A >
void CUtlArray<T, A>::CopyArray( const T *pArray, int size )
{
	// Can't insert something that's in the list... reallocation may hose us
	assert( (Base() == NULL) || !pArray || (Base() >= (pArray + size)) || (pArray >= (Base() + Count()) ) );

	SetSize( size );
	for( int i=0; i < size; i++ )
	{
		(*this)[i] = pArray[i];
	}
}

template< typename T, class A >
void CUtlArray<T, A>::Swap( CUtlArray< T, A > &vec )
{
	m_Memory.Swap( vec.m_Memory );
	swap( m_Size, vec.m_Size );
	swap( m_pElements, vec.m_pElements );
}

template< typename T, class A >
int CUtlArray<T, A>::AddVectorToTail( CUtlArray const &src )
{
	assert( &src != this );

	int base = Count();

	// Make space.
	AddMultipleToTail( src.Count() );

	// Copy the elements.
	for ( int i=0; i < src.Count(); i++ )
	{
		(*this)[base + i] = src[i];
	}

	return base;
}

template< typename T, class A >
inline int CUtlArray<T, A>::InsertMultipleBefore( int elem, int num, const T *pToInsert )
{
	if( num == 0 )
		return elem;

	// Can insert at the end
	assert( (elem == Count()) || IsValidIndex(elem) );

	GrowVector(num);
	ShiftElementsRight(elem, num);

	// Invoke default constructors
	for (int i = 0; i < num; ++i)
		Construct( &Element(elem+i) );

	// Copy stuff in?
	if ( pToInsert )
	{
		for ( int i=0; i < num; i++ )
		{
			Element( elem+i ) = pToInsert[i];
		}
	}

	return elem;
}


//-----------------------------------------------------------------------------
// Finds an element (element needs operator== defined)
//-----------------------------------------------------------------------------
template< typename T, class A >
int CUtlArray<T, A>::Find( const T& src ) const
{
	for ( int i = 0; i < Count(); ++i )
	{
		if (Element(i) == src)
			return i;
	}
	return -1;
}

template< typename T, class A >
bool CUtlArray<T, A>::HasElement( const T& src ) const
{
	return ( Find(src) >= 0 );
}


//-----------------------------------------------------------------------------
// Element removal
//-----------------------------------------------------------------------------
template< typename T, class A >
void CUtlArray<T, A>::FastRemove( int elem )
{
	assert( IsValidIndex(elem) );

	Destruct( &Element(elem) );
	if (m_Size > 0)
	{
		memcpy( &Element(elem), &Element(m_Size-1), sizeof(T) );
		--m_Size;
	}
}

template< typename T, class A >
void CUtlArray<T, A>::Remove( int elem )
{
	Destruct( &Element(elem) );
	ShiftElementsLeft(elem);
	--m_Size;
}

template< typename T, class A >
bool CUtlArray<T, A>::FindAndRemove( const T& src )
{
	int elem = Find( src );
	if ( elem != -1 )
	{
		Remove( elem );
		return true;
	}
	return false;
}

template< typename T, class A >
void CUtlArray<T, A>::RemoveMultiple( int elem, int num )
{
	assert( elem >= 0 );
	assert( elem + num <= Count() );

	for (int i = elem + num; --i >= elem; )
		Destruct(&Element(i));

	ShiftElementsLeft(elem, num);
	m_Size -= num;
}

template< typename T, class A >
void CUtlArray<T, A>::RemoveAll()
{
	for (int i = m_Size; --i >= 0; )
	{
		Destruct(&Element(i));
	}

	m_Size = 0;
}


//-----------------------------------------------------------------------------
// Memory deallocation
//-----------------------------------------------------------------------------

template< typename T, class A >
inline void CUtlArray<T, A>::Purge()
{
	RemoveAll();
	m_Memory.Purge();
	ResetDbgInfo();
}


template< typename T, class A >
inline void CUtlArray<T, A>::PurgeAndDeleteElements()
{
	for( int i=0; i < m_Size; i++ )
	{
		delete Element(i);
	}
	Purge();
}

template< typename T, class A >
inline void CUtlArray<T, A>::Compact()
{
	m_Memory.Purge(m_Size);
}

template< typename T, class A >
inline int CUtlArray<T, A>::NumAllocated() const
{
	return m_Memory.NumAllocated();
}

#endif // CCVECTOR_H
