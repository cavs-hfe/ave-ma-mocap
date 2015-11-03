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
%%% File: utils.cpp
%%%
%%% Description:
%%%
%%% Some helpful utility functions
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "utils.h"


// Remove leading and trailing whitespace
void trimWhiteSpace( char *s )
{
	char *p;
	int shift = 0;

	if (!s) return;

	p = s;
	
	// Trim leading white space
	while ( isspace((int)*p) )	p++;

	shift = p - s;

	while (*p)
	{
		*(p-shift) = *p;
		p++;
	}
	
	*(p-shift) = '\0';
	p = p - shift;

	// Trim trailing white space
	while ( (p != s) && isspace((int)(*(--p))) )	*p = '\0';
}

// Determine if the string represents an integer (all digits)
bool isInteger( const char* s )
{
	bool rc = false;
	
	if (s && *s)
	{
		rc = true;
		while (*s && rc)
		{
			if ( !isdigit((int) *(s++)) )
				rc = false;
		}
	}

	return rc;
}

// Get an input string from the user
void promptInput( const char* prompt, const char* def, char *s, int size )
{
	int i = 0;
	int iochar;

	if (prompt && def && s && size > 0)
	{
		printf("%s [default:%s] > ", prompt, def);
		
		size--;
		while ( (iochar = getchar()) != '\n' )
		{
			if ( i < size )
			{
				s[i++] = (char) iochar;
			}
		}
		
		s[i] = '\0';
		
		trimWhiteSpace(s);
		
		if (strlen(s) == 0)
		{
			strncpy( s, def, size );
			s[size] = '\0';
		}
	}
}

// Get a Yes/No response from the user
bool promptYesNo( const char* prompt, const char* def )
{
	bool rc = false;
	char buf[2];

	promptInput( prompt, def, buf, 2 );
	rc = (*buf == 'Y' || *buf == 'y');

	return rc;
}


int	promptInteger( const char* prompt, const char* def )
{
	int value;
	char buf[16];

	promptInput( prompt, def, buf, 16 );

	if (isInteger(buf))
	{
		value = atoi(buf);
	}
	else
	{
		value = atoi(def);
	}

	return value;
}
