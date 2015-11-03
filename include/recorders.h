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
%%% These classes implement recording functionality for the various data types streaming
%%% from EVaRT.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef __RECORDERS_H__
#define __RECORDERS_H__

#include "recorderbase.h"
#include "wrappers.h"


//
// Class to record TRC data from EVaRT
//
class TrcRecorder : public RecorderBase<TrcFrameWrapper>
{
public:

	//
	// Constructor
	//
	TrcRecorder( unsigned long maxSize = 1024 );

	//
	// Destructor
	//
	virtual ~TrcRecorder();

	void SetMarkerList( const MarkerListWrapper& list );

	virtual void Output( std::ostream& os, bool header = false );

protected:

	MarkerListWrapper mMarkerList;
};


//
// Class to record HTR2 and GTR data from EVaRT
//
class SegmentRecorder : public RecorderBase<SegmentFrameWrapper>
{
public:

	//
	// Constructor
	//
	SegmentRecorder( unsigned long maxSize = 1024 );

	//
	// Destructor
	//
	virtual ~SegmentRecorder();

	void SetHierarchy( const HierarchyWrapper& hierarchy );

	virtual void Output( std::ostream& os, bool header = false );

protected:

	HierarchyWrapper	mHierarchy;
};


//
// Class to record DOF data from EVaRT
//
class DofRecorder : public RecorderBase<DofFrameWrapper>
{
public:

	//
	// Constructor
	//
	DofRecorder( unsigned long maxSize = 1024 );

	//
	// Destructor
	//
	virtual ~DofRecorder();

	void SetDofNames( const DofNamesWrapper& names );

	virtual void Output( std::ostream& os, bool header = false );

protected:

	DofNamesWrapper		mDofNames;
};

#endif