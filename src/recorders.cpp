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
%%% File: recorders.cpp
%%%
%%% Description:
%%%
%%% Implementation for EVaRT data recorders.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "recorders.h"

//
// Class to record TRC data from EVaRT
//

// Constructor
TrcRecorder::TrcRecorder( unsigned long maxSize ) : RecorderBase<TrcFrameWrapper>(maxSize)
{}

// Destructor
TrcRecorder::~TrcRecorder()
{}

// Set the marker list
void TrcRecorder::SetMarkerList( const MarkerListWrapper& list )
{
	mMarkerList = list;
}

// Write current data to the specified stream
void TrcRecorder::Output( std::ostream& os, bool header )
{
	int i;

	if (header)
	{
		os << "Marker Names" << std::endl;

		for (i = 0; i < mMarkerList.Size(); i++)
		{
			os << mMarkerList.Name(i) << std::endl;
		}

		os << std::endl << std::endl;
	}

	TrcFrameWrapper f;
	Point3 pt;

	while (mFifo.Size() > 0)
	{
		if (mFifo.GetNext(f))
		{
			os << "Frame #" << f.Frame()+1 << ",X,Y,Z" << std::endl;	
			for (i = 0; i < f.Size(); i++)
			{
				f.GetMarkerLocation(i,pt);

				os << mMarkerList.Name(i) << "," << pt[0] << "," << pt[1] << "," << pt[2] << std::endl;
			}
		}
	}
}



//
// Class to record HTR2 or GTR data from EVaRT
//

// Constructor
SegmentRecorder::SegmentRecorder( unsigned long maxSize ) : RecorderBase<SegmentFrameWrapper>(maxSize)
{}

// Destructor
SegmentRecorder::~SegmentRecorder()
{}

// Set the skeletal hierarchy
void SegmentRecorder::SetHierarchy( const HierarchyWrapper& hierarchy )
{
	mHierarchy = hierarchy;
}

// Write current data to the specified stream
void SegmentRecorder::Output( std::ostream& os, bool header )
{
	int i;

	if (header)
	{
		os << "CHILD,PARENT" << std::endl;

		for (i = 0; i < mHierarchy.Size(); i++)
		{
			os << mHierarchy.Name(i) << "," << mHierarchy.NameOfParent(i) << std::endl;
		}

		os << std::endl << std::endl;
	}

	SegmentFrameWrapper f;
	SegmentInfo seg;

	while (mFifo.Size() > 0)
	{
		if (mFifo.GetNext(f))
		{
			os << "Frame #" << f.Frame()+1 << ",X,Y,Z,aX,aY,aZ,Length" << std::endl;	
			for (i = 0; i < f.Size(); i++)
			{
				f.GetSegmentInfo(i,seg);

				os << mHierarchy.Name(i) << "," << 
					seg[0] << "," << seg[1] << "," << seg[2] << "," << 
					seg[3] << "," << seg[4] << "," << seg[5] << "," <<
					seg[6] << std::endl;
			}
		}
	}
}




//
// Class to record DOF data from EVaRT
//

// Constructor
DofRecorder::DofRecorder( unsigned long maxSize ) : RecorderBase<DofFrameWrapper>(maxSize)
{}

// Destructor
DofRecorder::~DofRecorder()
{}

// Set the DOF names
void DofRecorder::SetDofNames( const DofNamesWrapper& names )
{
	mDofNames = names;
}

// Write current data to the specified stream
void DofRecorder::Output( std::ostream& os, bool header )
{
	int i;

	if (header)
	{
		os << "Frame #,";

		for (i = 0; i < mDofNames.Size(); i++)
		{
			os << mDofNames.Name(i) << ",";
		}

		os << std::endl;
	}

	DofFrameWrapper f;
	double value;

	while (mFifo.Size() > 0)
	{
		if (mFifo.GetNext(f))
		{
			os << f.Frame()+1 << ",";	
			for (i = 0; i < f.Size(); i++)
			{
				f.GetDofValue(i,value);

				os << value << ",";
			}
			os << std::endl;
		}
	}
}
