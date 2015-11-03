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
%%% File: recorders.h
%%%
%%% Description:
%%%
%%% This file provides classes which are used for recording data from EVaRT. The
%%% recorders buffer frames of data, and then they can output their data to a file.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef __RECORDER_BASE_H__
#define __RECORDER_BASE_H__

// Disable linker warning about truncation to 255 characters in debug info with std::string
#pragma warning (disable: 4786)

#include "fifo.h"
#include <iostream>

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: RecorderBase
%%%
%%% Description:
%%%
%%% This is a base class for recorders. Common functionality is implemented.
%%%
%%% Usage Notes:
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

template<class F>
class RecorderBase
{
public:

	//
	// Constructor
	//
	RecorderBase( unsigned long maxSize = 1024 );

	//
	// Destructor
	//
	~RecorderBase();

	unsigned long Size	();								// number of frames currently in recorder
	void SetMaxSize		( unsigned long maxSize );      // set max size after creation
	void Enable			( bool enabled		);			// enable/disable recorder
	void Add			( const F& element	);			// add a new element to the recorder
	void Start			();								// start recording
	void Stop			();								// stop recording

	virtual void Output( std::ostream& os, bool header = false ) = 0;	// output recorded data to the output stream

protected:

	FIFO<F>	mFifo;
	bool	mEnabled;
	bool	mRecording;
};


// RecorderBase constructor
template<class F>
RecorderBase<F>::RecorderBase<F>( unsigned long maxSize ) : mFifo( maxSize )
{
	mEnabled = true;
	mRecording = false;
}

// RecorderBase destructor
template<class F>
RecorderBase<F>::~RecorderBase<F>()
{
	mFifo.Clear();	
}


// Number of frames in recorder
template<class F>
unsigned long RecorderBase<F>::Size()
{
	return mFifo.Size();
}

// Set the max size
template<class F>
void RecorderBase<F>::SetMaxSize( unsigned long maxSize )
{
	mFifo.SetMaxSize( maxSize );
}

// Enable/disable recording
template<class F>
void RecorderBase<F>::Enable( bool enabled )
{
	mEnabled = enabled;
}

// Start recording
template<class F>
void RecorderBase<F>::Start()
{
	if (mEnabled)
	{
		// Clear any old data
		mFifo.Clear();
		mFifo.SetLocked(false);

		mRecording = true;
	}
}

// Stop recording
template<class F>
void RecorderBase<F>::Stop()
{
	if (mEnabled)
	{
		// prevent additions
		mFifo.SetLocked(true);

		mRecording = false;
	}
}

// Add a new frame of data to the recorder
template<class F>
void RecorderBase<F>::Add( const F& element )
{
	if (mEnabled && mRecording)
	{
		mFifo.Add( element );
	}
}

#endif