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
%%% File: utils.h
%%%
%%% Description:
%%%
%%% Some helpful utility functions, and classes.
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#ifndef __UTILS_H__
#define __UTILS_H__

#include <cstdio>
#include <string>
#include <ctime>
#include <cctype>


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%
%%% Class: TimeoutTimer
%%%
%%% Description:
%%%
%%% This is a simple class to implement a busy wait. 
%%%
%%% Usage Notes:
%%%
%%%		TimeoutTimer t(5.0);  // five second wait
%%%  
%%%		t.Begin(); 
%%%		while (!t.IsExpired())
%%%		{
%%%			// do stuff
%%%		}
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
class TimeoutTimer
{

public:
	
	TimeoutTimer			( double timeout = 1.0 ) : mTimeout(timeout), mStart(0), mDidExpire(false) {}
	~TimeoutTimer			(){}

	void SetTimeout			( double timeout )	{ mTimeout = timeout;	}
	void Begin				()					{ mDidExpire = false; mStart = clock();	}

	bool DidExpire			() const	{ return mDidExpire; }
	bool IsExpired			()	
	{ 
		mDidExpire = ((double)(clock()-mStart))/CLOCKS_PER_SEC > mTimeout;
		return mDidExpire;
	}

private:
	clock_t		mStart;
	double		mTimeout;	// specified in seconds
	bool		mDidExpire;
};


void	trimWhiteSpace	( char* s );
bool	isInteger		( const char* s );
void	promptInput		( const char* prompt, const char* def, char *s, int size );
bool	promptYesNo		( const char* prompt, const char* def );
int		promptInteger	( const char* prompt, const char* def );

#endif