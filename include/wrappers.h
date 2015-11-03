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
%%% File: wrappers.h
%%%
%%% Description:
%%%
%%% This file provides wrappers around some of the structures provided by the 
%%% EVaRT SDK. The wrappers allow the structures to be treated more like C++
%%% objects. You'll also note that most of the EVaRT structures are fixed in
%%% size, so if you plan on buffering the data streamed from EVaRT, it is more
%%% memory efficient to buffer these wrapper objects instead of the structures
%%% defined in EVaRT.h.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef __WRAPPERS_H__
#define __WRAPPERS_H__

// Disable linker warning about truncation to 255 characters in debug info with std::string
#pragma warning (disable: 4786)

//
// Standard include files
//
#include <vector>
#include <string>

//
// EVaRT SDK include files
//
#include "EVaRT.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: HierarchyWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sHierarchy structure defined in EVaRT.h as:
%%%
%%% typedef struct sHierarchy
%%% {
%%%    int    nSegments;
%%%    char **szSegmentNames;
%%%    int   *iParents;
%%%
%%% } sHierarchy;
%%%
%%% The "nSegments" field is the number of skeletal segments defined in EVaRT
%%% The "szSegmentNames" field is the name of the segments defined in EVaRT
%%% The "iParents" field establishes the skeletal hierarchy
%%%
%%% For example, let's say that I have the following hierarchy established in EVaRT
%%%
%%%                    SegmentA
%%%                   /        \
%%%                 /            \
%%%             SegmentB       SegmentC
%%%            /        \          |
%%%          /            \        |
%%%       SegmentD     SegmentE   SegmentF
%%%
%%% Then, the sHierarchy structure might look like this
%%%
%%% nSegments      = 6
%%% szSegmentNames = "SegmentA", "SegmentB", "SegmentC", "SegmentD", "SegmentE", "SegmentF"
%%% iParents       = -1, 0, 0, 1, 1, 2
%%%
%%% A value of "-1" in the iParents array means that segment is a child of the Global space.
%%% A value of zero or greater is the index into the szSegmentNames of the segments parent.                 
%%% 
%%% Usage Notes:
%%%
%%% A HierarchyWrapper object can not modify the contents of a sHierarchy, it is a 
%%% read-only wrapper.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class HierarchyWrapper
{
public:
	
	//
	// Constructors
	//
	HierarchyWrapper		( const sHierarchy* src = NULL );		// default constructor
	HierarchyWrapper		( const HierarchyWrapper& src );		// copy constructor

	//
	// Destructor
	//
	~HierarchyWrapper		();

	//
	// Set methods
	//
	void Set				( const sHierarchy* src = NULL );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()			const;			// number of segments
	std::string		Name				( int i )	const;			// segment name at index i
	int				Parent				( int i )	const;			// parent value at index i
	std::string		NameOfParent		( int i )	const;			// name of parent of segment at index i

	//
	// Operators
	//
	HierarchyWrapper&	operator	=	( const sHierarchy* lhs );			// assignment from sHierarchy structure
	HierarchyWrapper&	operator	=	( const HierarchyWrapper& lhs );	// assignment from HierarchyWrapper object

	bool				operator	==	( const sHierarchy* lhs ) const;		// equality to sHierarchy structure				
	bool				operator	==	( const HierarchyWrapper& lhs ) const;	// equality to HierarchyWrapper object
	bool				operator	!=	( const sHierarchy* lhs ) const;		// inequality to sHierarchy structure
	bool				operator	!=	( const HierarchyWrapper& lhs ) const;	// inequality to HierarchyWrapper object

//private:
public:

	std::vector<std::string>	mSegmentNames;
	std::vector<int>			mParents;

	void Copy( const sHierarchy* src );
	void Copy( const HierarchyWrapper& src );
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: MarkerListWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sMarkerList structure defined in EVaRT.h as:
%%%
%%% typedef struct sMarkerList
%%% {
%%%    int    nMarkers;
%%%    char **szMarkerNames;
%%%
%%% } sMarkerList;
%%%
%%% The "nMarkers" field is the number of named markers defined in EVaRT
%%% The "szMarkerNames" field is the name of the markers defined in EVaRT
%%% 
%%% Usage Notes:
%%%
%%% A MarkerListWrapper object can not modify the contents of a sMarkerList, it is a 
%%% read-only wrapper.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class MarkerListWrapper
{
public:
	
	//
	// Constructors
	//
	MarkerListWrapper		( const sMarkerList* src = NULL );		// default constructor
	MarkerListWrapper		( const MarkerListWrapper& src );		// copy constructor

	//
	// Destructor
	//
	~MarkerListWrapper		();

	//
	// Set methods
	//
	void Set				( const sMarkerList* src = NULL );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()			const;			// number of markers
	std::string		Name				( int i )	const;			// marker name at index i

	//
	// Operators
	//
	MarkerListWrapper&	operator	=	( const sMarkerList* lhs );			// assignment from sMarkerList structure
	MarkerListWrapper&	operator	=	( const MarkerListWrapper& lhs );	// assignment from MarkerListWrapper object

	bool				operator	==	( const sMarkerList* lhs ) const;		// equality to sMarkerList structure				
	bool				operator	==	( const MarkerListWrapper& lhs ) const;	// equality to MarkerListWrapper object
	bool				operator	!=	( const sMarkerList* lhs ) const;		// inequality to sMarkerList structure
	bool				operator	!=	( const MarkerListWrapper& lhs ) const;	// inequality to MarkerListWrapper object

private:

	std::vector<std::string>	mMarkerNames;

	void Copy( const sMarkerList* src );
	void Copy( const MarkerListWrapper& src );
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: DofNamesWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sDofNames structure defined in EVaRT.h as:
%%%
%%% typedef struct sDofNames
%%% {
%%%    int    nDOFs;
%%%    char **szNames;
%%%
%%% } sDofNames;
%%%
%%% The "nDOFs" field is the number of degrees of freedom defined in EVaRT
%%% The "szNames" field is the name of the degrees of freedom defined in EVaRT
%%% 
%%% Usage Notes:
%%%
%%% A DofNamesWrapper object can not modify the contents of a sDofNames, it is a 
%%% read-only wrapper.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class DofNamesWrapper
{
public:
	
	//
	// Constructors
	//
	DofNamesWrapper		( const sDofNames* src = NULL );	// default constructor
	DofNamesWrapper		( const DofNamesWrapper& src );		// copy constructor

	//
	// Destructor
	//
	~DofNamesWrapper		();

	//
	// Set methods
	//
	void Set				( const sDofNames* src = NULL );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()			const;			// number of degrees of freedom
	std::string		Name				( int i )	const;			// DOF name at index i

	//
	// Operators
	//
	DofNamesWrapper&	operator	=	( const sDofNames* lhs );			// assignment from sDofNames structure
	DofNamesWrapper&	operator	=	( const DofNamesWrapper& lhs );		// assignment from DofNamesWrapper object

	bool				operator	==	( const sDofNames* lhs ) const;			// equality to sDofNames structure				
	bool				operator	==	( const DofNamesWrapper& lhs ) const;	// equality to DofNamesWrapper object
	bool				operator	!=	( const sDofNames* lhs ) const;			// inequality to sDofNames structure
	bool				operator	!=	( const DofNamesWrapper& lhs ) const;	// inequality to DofNamesWrapper object

private:

	std::vector<std::string>	mDofNames;

	void Copy( const sDofNames* src );
	void Copy( const DofNamesWrapper& src );
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: TrcFrameWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sTrcFrame structure defined in EVaRT.h as:
%%%
%%% typedef struct sTrcFrame
%%% {
%%%    int    iFrame;
%%%    float  Markers[MAX_MARKERS][3];      // X,Y,Z 
%%%
%%% } sTrcFrame;
%%%
%%% The "iFrame" field is the frame number from EVaRT 
%%% The "Markers" array gives an X,Y,Z location for each named marker in EVaRT
%%% A value of "XEMPTY" (defined in EVaRT.h), for a marker means that marker
%%% was not identified in the given frame. The values are in terms of the 
%%% calibration units used in EVaRT.
%%% 
%%% Usage Notes:
%%%
%%% A TrcFrameWrapper object can not modify the contents of a sTrcFrame, it is a 
%%% read-only wrapper, however, it only stores the number of markers which are valid,
%%% whereas a sTrcFrame has a fixed size of MAX_MARKERS.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

typedef float Point3[3];

class TrcFrameWrapper
{
public:
	
	//
	// Constructors
	//
	TrcFrameWrapper		( const sTrcFrame* src = NULL, int count = 0 );	// default constructor
	TrcFrameWrapper		( const TrcFrameWrapper& src );					// copy constructor

	//
	// Destructor
	//
	~TrcFrameWrapper		();

	//
	// Set methods
	//
	void Set				( const sTrcFrame* src = NULL, int count = 0 );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()					const;	// number of markers in this frame
	int				Frame				()					const;	// frame number of this frame
	void			GetMarkerLocation	(int i, Point3 loc) const;	// 3-D position of the marker at the specified index 
	

	//
	// Operators
	//
	TrcFrameWrapper&	operator	=	( const TrcFrameWrapper& lhs );		// assignment from TrcFrameWrapper object

	bool				operator	==	( const TrcFrameWrapper& lhs ) const;	// equality to TrcFrameWrapper object
	bool				operator	!=	( const TrcFrameWrapper& lhs ) const;	// inequality to TrcFrameWrapper object

private:

	Point3* mMarkers;
	int		mFrame;
	int		mCount;

	void Copy( const sTrcFrame* src, int count );
	void Copy( const TrcFrameWrapper& src );
	void FreeMemory();
};


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: SegmentFrameWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sHtr2Frame and the sGtrFrame structures defined 
%%% in EVaRT.h as:
%%%
%%% typedef struct sHtr2Frame 
%%% {
%%%    int    iFrame;
%%%    float  Segments[MAX_SEGMENTS][7];    //X,Y,Z,aX,aY,aZ,Length 
%%%
%%% } sHtr2Frame;
%%%
%%% typedef struct sGtrFrame 
%%% {
%%%    int    iFrame;
%%%    float  Segments[MAX_SEGMENTS][7];    //X,Y,Z,aX,aY,aZ,Length 
%%%
%%% } sGtrFrame;
%%%
%%% The "iFrame" field is the frame number from EVaRT 
%%% The "Segments" array gives an X,Y,Z translation, an X,Y,Z rotation, and a length
%%% for each skeletal segment in EVaRT. The translation and rotation values are
%%% relative to the segment's parent. For GTR data, every segment is a child of the
%%% GLOBAL space. The translation and length values are in terms
%%% of the calibration units in EVaRT; the rotation values are Euler angles in degrees. 
%%% 
%%% Usage Notes:
%%%
%%% A SegmentFrameWrapper object can not modify the contents of a sHtr2Frame or sGtrFrame, 
%%% it is a read-only wrapper, however, it only stores the number of segments which are valid.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

typedef float SegmentInfo[7];
typedef sHtr2Frame SegmentFrame;

class SegmentFrameWrapper
{
public:
	
	//
	// Constructors
	//
	SegmentFrameWrapper		( const SegmentFrame* src = NULL, int count = 0 );	// default constructor
	SegmentFrameWrapper		( const SegmentFrameWrapper& src );					// copy constructor

	//
	// Destructor
	//
	~SegmentFrameWrapper		();

	//
	// Set methods
	//
	void Set				( const SegmentFrame* src = NULL, int count = 0 );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()							const;	// number of segments in this frame
	int				Frame				()							const;	// frame number of this frame
	void			GetSegmentInfo		(int i, SegmentInfo info)	const;	// segment info for segment at the specified index 
	

	//
	// Operators
	//
	SegmentFrameWrapper&	operator	=	( const SegmentFrameWrapper& lhs );		// assignment from SegmentFrameWrapper object

	bool				operator	==	( const SegmentFrameWrapper& lhs ) const;	// equality to SegmentFrameWrapper object
	bool				operator	!=	( const SegmentFrameWrapper& lhs ) const;	// inequality to SegmentFrameWrapper object

private:

	SegmentInfo*	mSegments;
	int				mFrame;
	int				mCount;

	void Copy( const SegmentFrame* src, int count );
	void Copy( const SegmentFrameWrapper& src );
	void FreeMemory();
};



/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: DofFrameWrapper
%%%
%%% Description:
%%%
%%% This class encapsulates the sDofFrame structures defined in EVaRT.h as:
%%%
%%% typedef struct sDofFrame  // Mostly Angles
%%% {
%%%    int    iFrame;
%%%    int    nDOFs;
%%%    double DOFs[MAX_DOFS];
%%%
%%% } sDofFrame;
%%%
%%% The "iFrame" field is the frame number from EVaRT
%%% The "nDOFs" field is the number of degrees of freedom 
%%% The "DOFs" array gives a value for each degree of freedom 
%%% 
%%% Usage Notes:
%%%
%%% A DofFrameWrapper object can not modify the contents of a sDofFrame, 
%%% it is a read-only wrapper, however, it only stores the number of DOFs which are valid.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

class DofFrameWrapper
{
public:
	
	//
	// Constructors
	//
	DofFrameWrapper		( const sDofFrame* src = NULL);	// default constructor
	DofFrameWrapper		( const DofFrameWrapper& src );	// copy constructor

	//
	// Destructor
	//
	~DofFrameWrapper		();

	//
	// Set methods
	//
	void Set				( const sDofFrame* src = NULL );		// set/reset after creation
	
	//
	// Get methods
	//
	int				Size				()							const;	// number of DOFs in this frame
	int				Frame				()							const;	// frame number of this frame
	void			GetDofValue			(int i, double& value)		const;	// DOF value at index i
	

	//
	// Operators
	//
	DofFrameWrapper&	operator	=	( const DofFrameWrapper& lhs );		// assignment from DofFrameWrapper object

	bool				operator	==	( const DofFrameWrapper& lhs ) const;	// equality to DofFrameWrapper object
	bool				operator	!=	( const DofFrameWrapper& lhs ) const;	// inequality to DofFrameWrapper object

private:

	double*			mDofs;
	int				mFrame;
	int				mCount;

	void Copy( const sDofFrame* src );
	void Copy( const DofFrameWrapper& src );
	void FreeMemory();
};


#endif