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
%%% File: wrappers.cpp
%%%
%%% Description:
%%%
%%% Implementation of wrapper classes for EVaRT structures
%%%
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

//
// Standard includes
//
#include "wrappers.h"



//
// Wrapper for sHierarchy structure
//

// Default constructor
HierarchyWrapper::HierarchyWrapper( const sHierarchy* src )
{
	Copy( src );
}

// Copy constructor
HierarchyWrapper::HierarchyWrapper( const HierarchyWrapper& src )
{
	Copy( src );
}

// Destructor
HierarchyWrapper::~HierarchyWrapper()
{
	mSegmentNames.clear();
	mParents.clear();
}

// Set/Reset after creation
void HierarchyWrapper::Set( const sHierarchy* src )
{
	Copy( src );
}

// Get number of segments
int HierarchyWrapper::Size() const
{
	return (int) mSegmentNames.size();
}

// Get the segment name at index i
std::string HierarchyWrapper::Name( int i ) const
{
	std::string name = "";

	if (i >= 0 && i < mSegmentNames.size())
	{
		name = mSegmentNames[i];
	}

	return name;
}

// Get the parent value at index i
int HierarchyWrapper::Parent( int i ) const
{
	int parent = -2; // invalid parent

	if (i >= 0 && i < mParents.size())
	{
		parent = mParents[i];
	}

	return parent;
}

// Get the name of the parent of the segment at index i
std::string HierarchyWrapper::NameOfParent( int i ) const
{
	std::string name = "";
	int p = Parent(i);
	
	if (p == -1)
	{
		name = "GLOBAL";
	}
	else
	{
		name = Name(p);
	}

	return name;
}	

// Assignment operator from a sHierarchy*
HierarchyWrapper& HierarchyWrapper::operator = ( const sHierarchy* lhs )
{
	Copy( lhs );
	return *this;
}

// Assignment operator from a HierarchyWrapper object
HierarchyWrapper& HierarchyWrapper::operator = ( const HierarchyWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against sHierarchy*
bool HierarchyWrapper::operator == ( const sHierarchy* lhs ) const
{
	HierarchyWrapper tmp(lhs);
	return *this == tmp;
}

// Equality check against sHierarchy*
bool HierarchyWrapper::operator == ( const HierarchyWrapper& lhs ) const
{
	return (mSegmentNames == lhs.mSegmentNames && mParents == lhs.mParents);
}

// Inequality check against sHierarchy*
bool HierarchyWrapper::operator != ( const sHierarchy* lhs ) const
{
	return !(*this == lhs);
}

// Inequality check against a HierarchyWrapper object
bool HierarchyWrapper::operator != ( const HierarchyWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a sHierarchy*
void HierarchyWrapper::Copy( const sHierarchy* src )
{
	// clear any previous data
	mSegmentNames.clear();
	mParents.clear();

	// if the pointer is valid, fill up our lists
	if (src)
	{
		int count = src->nSegments;

		for (int i = 0; i < count; i++)
		{
			mSegmentNames.push_back( std::string(src->szSegmentNames[i]) );
			mParents.push_back( src->iParents[i] );
		}
	}
}

// Fill object with values from a HierarchyWrapper object
void HierarchyWrapper::Copy( const HierarchyWrapper& src )
{
	// clear any previous data
	mSegmentNames.clear();
	mParents.clear();
	
	// copy contents of source object
	for (int i = 0; i < src.Size(); i++)
	{
		mSegmentNames.push_back( src.Name(i) );
		mParents.push_back( src.Parent(i) );
	}
}



//
// Wrapper for sMarkerList structure
//

// Default constructor
MarkerListWrapper::MarkerListWrapper( const sMarkerList* src )
{
	Copy( src );
}

// Copy constructor
MarkerListWrapper::MarkerListWrapper( const MarkerListWrapper& src )
{
	Copy( src );
}

// Destructor
MarkerListWrapper::~MarkerListWrapper()
{
	mMarkerNames.clear();
}

// Set/Reset after creation
void MarkerListWrapper::Set( const sMarkerList* src )
{
	Copy( src );
}

// Get number of markers
int MarkerListWrapper::Size() const
{
	return (int) mMarkerNames.size();
}

// Get the marker name at index i
std::string MarkerListWrapper::Name( int i ) const
{
	std::string name = "";

	if (i >= 0 && i < mMarkerNames.size())
	{
		name = mMarkerNames[i];
	}

	return name;
}

// Assignment operator from a sMarkerList*
MarkerListWrapper& MarkerListWrapper::operator = ( const sMarkerList* lhs )
{
	Copy( lhs );
	return *this;
}

// Assignment operator from a MarkerListWrapper object
MarkerListWrapper& MarkerListWrapper::operator = ( const MarkerListWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against sMarkerList*
bool MarkerListWrapper::operator == ( const sMarkerList* lhs ) const
{
	MarkerListWrapper tmp(lhs);
	return *this == tmp;
}

// Equality check against sMarkerList*
bool MarkerListWrapper::operator == ( const MarkerListWrapper& lhs ) const
{
	return (mMarkerNames == lhs.mMarkerNames);
}

// Inequality check against sMarkerList*
bool MarkerListWrapper::operator != ( const sMarkerList* lhs ) const
{
	return !(*this == lhs);
}

// Inequality check against a MarkerListWrapper object 
bool MarkerListWrapper::operator != ( const MarkerListWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a sMarkerList*
void MarkerListWrapper::Copy( const sMarkerList* src )
{
	// clear any previous data
	mMarkerNames.clear();

	// if the pointer is valid, fill up our list
	if (src)
	{
		int count = src->nMarkers;

		for (int i = 0; i < count; i++)
		{
			mMarkerNames.push_back( std::string(src->szMarkerNames[i]) );
		}
	}
}

// Fill object with values from a MarkerListWrapper object
void MarkerListWrapper::Copy( const MarkerListWrapper& src )
{
	// clear any previous data
	mMarkerNames.clear();
	
	// copy contents of source object
	for (int i = 0; i < src.Size(); i++)
	{
		mMarkerNames.push_back( src.Name(i) );
	}
}


//
// Wrapper for sDofNames structure
//

// Default constructor
DofNamesWrapper::DofNamesWrapper( const sDofNames* src )
{
	Copy( src );
}

// Copy constructor
DofNamesWrapper::DofNamesWrapper( const DofNamesWrapper& src )
{
	Copy( src );
}

// Destructor
DofNamesWrapper::~DofNamesWrapper()
{
	mDofNames.clear();
}

// Set/Reset after creation
void DofNamesWrapper::Set( const sDofNames* src )
{
	Copy( src );
}

// Get number of degrees of freedom
int DofNamesWrapper::Size() const
{
	return (int) mDofNames.size();
}

// Get the DOF name at index i
std::string DofNamesWrapper::Name( int i ) const
{
	std::string name = "";

	if (i >= 0 && i < mDofNames.size())
	{
		name = mDofNames[i];
	}

	return name;
}

// Assignment operator from a sDofNames*
DofNamesWrapper& DofNamesWrapper::operator = ( const sDofNames* lhs )
{
	Copy( lhs );
	return *this;
}

// Assignment operator from a DofNamesWrapper object
DofNamesWrapper& DofNamesWrapper::operator = ( const DofNamesWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against sDofNames*
bool DofNamesWrapper::operator == ( const sDofNames* lhs ) const
{
	DofNamesWrapper tmp(lhs);
	return *this == tmp;
}

// Equality check against sDofNames*
bool DofNamesWrapper::operator == ( const DofNamesWrapper& lhs ) const
{
	return (mDofNames == lhs.mDofNames);
}

// Inequality check against sDofNames*
bool DofNamesWrapper::operator != ( const sDofNames* lhs ) const
{
	return !(*this == lhs);
}

// Inequality check against a DofNamesWrapper object 
bool DofNamesWrapper::operator != ( const DofNamesWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a sDofNames*
void DofNamesWrapper::Copy( const sDofNames* src )
{
	// clear any previous data
	mDofNames.clear();

	// if the pointer is valid, fill up our list
	if (src)
	{
		int count = src->nDOFs;

		for (int i = 0; i < count; i++)
		{
			mDofNames.push_back( std::string(src->szNames[i]) );
		}
	}
}

// Fill object with values from a DofNamesWrapper object
void DofNamesWrapper::Copy( const DofNamesWrapper& src )
{
	// clear any previous data
	mDofNames.clear();
	
	// copy contents of source object
	for (int i = 0; i < src.Size(); i++)
	{
		mDofNames.push_back( src.Name(i) );
	}
}



//
// Wrapper for sTrcFrame structure
//

// Default constructor
TrcFrameWrapper::TrcFrameWrapper( const sTrcFrame* src, int count )
{
	mMarkers = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src, count );
}

// Copy constructor
TrcFrameWrapper::TrcFrameWrapper( const TrcFrameWrapper& src )
{
	mMarkers = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src );
}

// Destructor
TrcFrameWrapper::~TrcFrameWrapper()
{
	FreeMemory();
}

// Set/Reset after creation
void TrcFrameWrapper::Set( const sTrcFrame* src, int count )
{
	Copy( src, count );
}

// Get number of markers in this frame
int TrcFrameWrapper::Size() const
{
	return mCount;
}

// Get frame number for this frame
int TrcFrameWrapper::Frame() const
{
	return mFrame;
}

// Get the 3-D coordinates of the marker at the given index
void TrcFrameWrapper::GetMarkerLocation( int i, Point3 loc ) const
{
	loc[0] = loc[1] = loc[2] = XEMPTY;

	if (i >= 0 && i < mCount && mMarkers)
	{
		loc[0] = mMarkers[i][0];
		loc[1] = mMarkers[i][1];
		loc[2] = mMarkers[i][2];
	}
}


// Assignment operator from a TrcFrameWrapper object
TrcFrameWrapper& TrcFrameWrapper::operator = ( const TrcFrameWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against a TrcFrameWrapper object
bool TrcFrameWrapper::operator == ( const TrcFrameWrapper& lhs ) const
{
	bool rc = false;
	int i = 0;

	rc = (mCount == lhs.mCount);
	rc = rc && mFrame == lhs.mFrame;

	while (i < mCount && rc)
	{
		rc = ( mMarkers[i][0] == lhs.mMarkers[i][0] &&
			   mMarkers[i][1] == lhs.mMarkers[i][1] &&
			   mMarkers[i][2] == lhs.mMarkers[i][2] );
		i++;
	}

	return rc;
}

// Inequality check against a TrcFrameWrapper object 
bool TrcFrameWrapper::operator != ( const TrcFrameWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a sTrcFrame*
void TrcFrameWrapper::Copy( const sTrcFrame* src, int count )
{
	// clear any previous data
	FreeMemory();

	// if the frame and count are valid, copy count number of marker slots
	if (src && count > 0)
	{
		mMarkers = new Point3[count];

		if (mMarkers)
		{
			mCount = count;
			mFrame = src->iFrame;

			for (int i = 0; i < mCount; i++)
			{
				mMarkers[i][0] = src->Markers[i][0];
				mMarkers[i][1] = src->Markers[i][1];
				mMarkers[i][2] = src->Markers[i][2];
			}
		}
	}
}

// Fill object with values from a TrcFrameWrapper object
void TrcFrameWrapper::Copy( const TrcFrameWrapper& src )
{
	// clear any previous data
	FreeMemory();
	
	int count = src.Size();

	// copy contents of source object
	if (count > 0)
	{
		mMarkers = new Point3[count];

		if (mMarkers)
		{
			mCount = count;
			mFrame = src.Frame();

			for (int i = 0; i < mCount; i++)
			{
				src.GetMarkerLocation( i, mMarkers[i] );
			}
		}
	}
}

// Deallocates any previously allocated memory
void TrcFrameWrapper::FreeMemory()
{
	if (mMarkers)
	{
		delete[] mMarkers;
	}

	mMarkers = NULL;
	mCount = 0;
	mFrame = -1;
}


//
// Wrapper for SegmentFrame structure
//

// Default constructor
SegmentFrameWrapper::SegmentFrameWrapper( const SegmentFrame* src, int count )
{
	mSegments = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src, count );
}

// Copy constructor
SegmentFrameWrapper::SegmentFrameWrapper( const SegmentFrameWrapper& src )
{
	mSegments = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src );
}

// Destructor
SegmentFrameWrapper::~SegmentFrameWrapper()
{
	FreeMemory();
}

// Set/Reset after creation
void SegmentFrameWrapper::Set( const SegmentFrame* src, int count )
{
	Copy( src, count );
}

// Get number of segments in this frame
int SegmentFrameWrapper::Size() const
{
	return mCount;
}

// Get frame number for this frame
int SegmentFrameWrapper::Frame() const
{
	return mFrame;
}

// Get the segment info of the segment at the given index
void SegmentFrameWrapper::GetSegmentInfo( int i, SegmentInfo info ) const
{
	info[0]=info[1]=info[2]=info[3]=info[4]=info[5]=info[6] = XEMPTY;

	if (i >= 0 && i < mCount && mSegments)
	{
		info[0] = mSegments[i][0];
		info[1] = mSegments[i][1];
		info[2] = mSegments[i][2];
		info[3] = mSegments[i][3];
		info[4] = mSegments[i][4];
		info[5] = mSegments[i][5];
		info[6] = mSegments[i][6];
	}
}


// Assignment operator from a SegmentFrameWrapper object
SegmentFrameWrapper& SegmentFrameWrapper::operator = ( const SegmentFrameWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against a SegmentFrameWrapper object
bool SegmentFrameWrapper::operator == ( const SegmentFrameWrapper& lhs ) const
{
	bool rc = false;
	int i = 0;

	rc = (mCount == lhs.mCount);
	rc = rc && mFrame == lhs.mFrame;

	while (i < mCount && rc)
	{
		rc = ( mSegments[i][0] == lhs.mSegments[i][0] &&
			   mSegments[i][1] == lhs.mSegments[i][1] &&
			   mSegments[i][2] == lhs.mSegments[i][2] &&
			   mSegments[i][3] == lhs.mSegments[i][3] &&
			   mSegments[i][4] == lhs.mSegments[i][4] &&
			   mSegments[i][5] == lhs.mSegments[i][5] &&
			   mSegments[i][6] == lhs.mSegments[i][6] );
		i++;
	}

	return rc;
}

// Inequality check against a SegmentFrameWrapper object 
bool SegmentFrameWrapper::operator != ( const SegmentFrameWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a SegmentFrame*
void SegmentFrameWrapper::Copy( const SegmentFrame* src, int count )
{
	// clear any previous data
	FreeMemory();

	// if the frame and count are valid, copy count number of segment slots
	if (src && count > 0)
	{
		mSegments = new SegmentInfo[count];

		if (mSegments)
		{
			mCount = count;
			mFrame = src->iFrame;

			for (int i = 0; i < mCount; i++)
			{
				mSegments[i][0] = src->Segments[i][0];
				mSegments[i][1] = src->Segments[i][1];
				mSegments[i][2] = src->Segments[i][2];
				mSegments[i][3] = src->Segments[i][3];
				mSegments[i][4] = src->Segments[i][4];
				mSegments[i][5] = src->Segments[i][5];
				mSegments[i][6] = src->Segments[i][6];
			}
		}
	}
}

// Fill object with values from a SegmentFrameWrapper object
void SegmentFrameWrapper::Copy( const SegmentFrameWrapper& src )
{
	// clear any previous data
	FreeMemory();
	
	int count = src.Size();

	// copy contents of source object
	if (count > 0)
	{
		mSegments = new SegmentInfo[count];

		if (mSegments)
		{
			mCount = count;
			mFrame = src.Frame();

			for (int i = 0; i < mCount; i++)
			{
				src.GetSegmentInfo( i, mSegments[i] );
			}
		}
	}
}

// Deallocates any previously allocated memory
void SegmentFrameWrapper::FreeMemory()
{
	if (mSegments)
	{
		delete[] mSegments;
	}

	mSegments = NULL;
	mCount = 0;
	mFrame = -1;
}



//
// Wrapper for sDofFrame structure
//

// Default constructor
DofFrameWrapper::DofFrameWrapper( const sDofFrame* src )
{
	mDofs = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src );
}

// Copy constructor
DofFrameWrapper::DofFrameWrapper( const DofFrameWrapper& src )
{
	mDofs = NULL;
	mCount = 0;
	mFrame = -1;

	Copy( src );
}

// Destructor
DofFrameWrapper::~DofFrameWrapper()
{
	FreeMemory();
}

// Set/Reset after creation
void DofFrameWrapper::Set( const sDofFrame* src )
{
	Copy( src );
}

// Get number of degrees of freedom in this frame
int DofFrameWrapper::Size() const
{
	return mCount;
}

// Get frame number for this frame
int DofFrameWrapper::Frame() const
{
	return mFrame;
}

// Get the degree of freedom value at index i
void DofFrameWrapper::GetDofValue( int i, double& value ) const
{
	value = XEMPTY;

	if (i >= 0 && i < mCount && mDofs)
	{
		value = mDofs[i];
	}
}


// Assignment operator from a DofFrameWrapper object
DofFrameWrapper& DofFrameWrapper::operator = ( const DofFrameWrapper& lhs )
{
	Copy( lhs );
	return *this;
}

// Equality check against a DofFrameWrapper object
bool DofFrameWrapper::operator == ( const DofFrameWrapper& lhs ) const
{
	bool rc = false;
	int i = 0;

	rc = (mCount == lhs.mCount);
	rc = rc && mFrame == lhs.mFrame;

	while (i < mCount && rc)
	{
		rc = mDofs[i] == lhs.mDofs[i];
		i++;
	}

	return rc;
}

// Inequality check against a DofFrameWrapper object 
bool DofFrameWrapper::operator != ( const DofFrameWrapper& lhs ) const
{
	return !(*this == lhs);
}


// Fill object with values from a sDofFrame*
void DofFrameWrapper::Copy( const sDofFrame* src )
{
	// clear any previous data
	FreeMemory();

	int count;

	// if the frame and count are valid, copy count number of DOF values
	if (src)
	{
		count = src->nDOFs;
		
		if (count > 0)
		{
			mDofs = new double[count];
			
			if (mDofs)
			{
				mCount = count;
				mFrame = src->iFrame;
				
				for (int i = 0; i < mCount; i++)
				{
					mDofs[i] = src->DOFs[i];
				}
			}
		}
	}
}

// Fill object with values from a DofFrameWrapper object
void DofFrameWrapper::Copy( const DofFrameWrapper& src )
{
	// clear any previous data
	FreeMemory();
	
	int count = src.Size();

	// copy contents of source object
	if (count > 0)
	{
		mDofs = new double[count];

		if (mDofs)
		{
			mCount = count;
			mFrame = src.Frame();

			for (int i = 0; i < mCount; i++)
			{
				mDofs[i] = src.mDofs[i];
			}
		}
	}
}

// Deallocates any previously allocated memory
void DofFrameWrapper::FreeMemory()
{
	if (mDofs)
	{
		delete[] mDofs;
	}

	mDofs = NULL;
	mCount = 0;
	mFrame = -1;
}
