// This software is provided "as is" without warranties as to performance or merchantability
// or any other warranties whether expressed or implied. Because of the various hardware
// and software environments into which software may be put, no warranty of fitness for a
// particular purpose is offered. Good data processing procedure dictates that any software
// be thoroughly tested with non-critical data before relying on it. You must assume the
// entire risk of using the software. In no event shall Motion Analysis Corp. be liable for
// any damages in connection with or arising out of the use of the software by any person
// whatsoever, including incidental, indirect, special or consequential damages, or any
// damages related to loss of use, revenue or profits, even if we have been advised of the
// possibility of such damages. Technical support is limited to those customers with an
// on-going Motion Analysis maintenance contract.


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% File: fifo.h
%%%
%%% Description:
%%%
%%% This class provides a templated, thread-safe FIFO data structure. Objects added
%%% to the structure must have the assignment operator defined and copy constructor
%%% defined. If the fifo is storing pointers, the user of the fifo is responsible
%%% for deallocating any dynamically allocated memory associated with the pointers
%%% being stored.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef __FIFO_H__
#define __FIFO_H__

//
// Standard headers
//
#include <windows.h>
#include <queue>

// How to deal with additions when the FIFO gets full
enum FifoReplaceStrategy
{
	kRemoveOldest = 0,  // if fifo is full, remove the oldest element before adding the new one
	kStopAdding         // if fifo is full, don't add a new element until there is room
};

template<class T>
class FIFO
{

public:

	//
	// Constructors
	//
	FIFO<T>				( unsigned long maxSize = 1024, FifoReplaceStrategy replaceStrategy = kStopAdding );

	//
	// Destructor
	// 
	~FIFO<T>			();

	//
	// Methods
	//
	void	Add			( const T& element );		// add a new element to the fifo
	bool	GetNext		( T& next );				// get the element at the front of the fifo, the item is removed
	bool	PeekNext	( T& next );				// get the element at the front of the fifo, the item is not removed
	void	Clear		();							// clear the fifo


	unsigned long 			Size			();		// number of elements in the fifo
	unsigned long			MaxSize			();		// maximum number of elements to hold
	FifoReplaceStrategy		ReplaceStrategy	();		// how additions are handled when the fifo is full
	bool					IsLocked		();		// are additions currently not allowed

	void	SetMaxSize			( unsigned long maxSize );					// set the maximum size of the fifo
	void	SetReplaceStrategy	( FifoReplaceStrategy replaceStrategy );	// set the replacement strategy of the fifo
	void	SetLocked			( bool locked );							// allow/disallow new additions

private:

	std::queue<T> mQ;	// queue of elements
	HANDLE mSemaphore;  // handle to semaphore to control access
	bool mLocked;		// if true, addition of new elements is not allowed

	unsigned long mMaxSize;					// maximum number of elements to hold
	FifoReplaceStrategy mReplaceStrategy;

	bool SemWait();
	bool SemRelease();
	void EmptyQ();
};



// Constructor
template<class T>
FIFO<T>::FIFO<T>( unsigned long maxSize, FifoReplaceStrategy replaceStrategy )
{
	mMaxSize = maxSize;
	mReplaceStrategy = replaceStrategy;
	mLocked = false;

	// Create semaphore to control access to our list
	mSemaphore = CreateSemaphore( NULL, 1, 1, NULL );
}

// Destructor
template<class T>
FIFO<T>::~FIFO<T>()
{
	if (SemWait())
	{
		EmptyQ();
		SemRelease();
	}
	CloseHandle( mSemaphore );
}

// Clear the fifo
template<class T>
void FIFO<T>::Clear()
{
	if (SemWait())
	{
		EmptyQ();
		SemRelease();
	}
}

// Get the number of elements currently in the fifo
template<class T>
unsigned long FIFO<T>::Size()
{
	unsigned long size = 0;

	if (SemWait())
	{
		size = mQ.size();
		SemRelease();		
	}

	return size;
}

// Get the maximum number of elements the fifo can hold at one time
template<class T>
unsigned long FIFO<T>::MaxSize()
{
	unsigned long maxSize = 0;

	if (SemWait())
	{
		maxSize = mMaxSize;
		SemRelease();
	}

	return maxSize;
}

// Get the replacement strategy used by the fifo
template<class T>
FifoReplaceStrategy FIFO<T>::ReplaceStrategy()
{
	FifoReplaceStrategy s = kStopAdding;

	if (SemWait())
	{
		s = mReplaceStrategy;
		SemRelease();
	}

	return s;
}

// Get the current locked state of the fifo
template<class T>
bool FIFO<T>::IsLocked()
{
	bool locked = true;

	if (SemWait())
	{
		locked = mLocked; 
		SemRelease();
	}

	return locked;
}



// If locked is true, the fifo will not accept any additions
template<class T>
void FIFO<T>::SetLocked( bool locked )
{
	if (SemWait())
	{
		mLocked = locked;
		SemRelease();
	}
}

// Set the maximum size of the fifo
template<class T>
void FIFO<T>::SetMaxSize( unsigned long maxSize )
{
	if (SemWait())
	{
		mMaxSize = maxSize;
		SemRelease();
	}
}

// Set the replacement strategy of the fifo
template<class T>
void FIFO<T>::SetReplaceStrategy( FifoReplaceStrategy replaceStrategy )
{
	if (SemWait())
	{
		mReplaceStrategy = replaceStrategy;
		SemRelease();
	}
}

// Add a new element to the fifo
template<class T>
void FIFO<T>::Add( const T& element )
{
	if (mMaxSize > 0 && !mLocked && SemWait())
	{
		// add the new element, since we know we have room, we don't have to do anything else
		if (mQ.size() < mMaxSize)
		{
			mQ.push( element );
		}
		else
		{
			// the fifo is full, need to check the replacement strategy
			// don't add the element if the replace strategy is kStopAdding
			if (mReplaceStrategy == kRemoveOldest)
			{
				// make room for the new element by deleting old ones
				while (mQ.size() >= mMaxSize)
				{
					mQ.pop();
				}

				// add the new element
				mQ.push( element );
			}
		}
		SemRelease();
	}
}

// Get the next item in the fifo, returns true if an item exists, false otherwise
// The item is removed from the fifo
template<class T>
bool FIFO<T>::GetNext( T& next )
{
	bool rc = false;

	if (SemWait())
	{
		if (mQ.empty() == false)
		{
			next = mQ.front();
			mQ.pop();
			rc = true;
		}

		SemRelease();
	}

	return rc;
}

// Get the next item in the fifo, returns true if an item exists, false otherwise
// The item is NOT removed from the fifo
template<class T>
bool FIFO<T>::PeekNext( T& next )
{
	bool rc = false;

	if (SemWait())
	{
		if (mQ.empty() == false)
		{
			next = mQ.front();
			rc = true;
		}

		SemRelease();
	}

	return rc;
}


// Remove all elements from the fifo
// It is the callers responsibility to acquire the semaphore beforehand
template<class T>
void FIFO<T>::EmptyQ()
{
	while (mQ.empty() == false)
	{
		mQ.pop();
	}
}


//
//  Method:  SemWait
//  Purpose: Wait for the state of the semaphore to be signaled
//  Returns: true if successful, false otherwise
//
template<class T>
bool FIFO<T>::SemWait()
{
	return (WaitForSingleObject( mSemaphore, INFINITE ) == WAIT_OBJECT_0);
}

//
//  Method:  SemRelease
//  Purpose: Release the semaphore so it's state is signaled
//  Returns: true if successful, false otherwise
//
template<class T>
bool FIFO<T>::SemRelease()
{
	return (ReleaseSemaphore( mSemaphore, 1, NULL ) != 0);
}

#endif