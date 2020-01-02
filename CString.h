
#ifndef __CSTRING_H__
#define __CSTRING_H__


#ifndef __CBASEOBJECT_H__
#include "CBaseObject.h"
#endif

class CString : public CBaseObject
{

public :

	// ctors
	CString();							// CString foo;
	~CString();
	CString( const CString& rvalue );	// CString foo = bar;
	CString( const char* rvalue );		// CString foo = "bar";

	// assignment operators
	const CString&	operator= ( const CString& rvalue );
	const CString&	operator= ( const char* rvalue );

	// comparison operators
	BOOL			operator==( const CString& rvalue )	const;
	BOOL			operator==( const char* rvalue )	const;
	BOOL			operator!=( const CString& rvalue )	const;
	BOOL			operator!=( const char* rvalue )	const;

	// concatenation operators
	const CString	operator+ ( const CString& rvalue );

	// typecasting operators
					operator char*()		{ return m_pTheString; }
					operator const char*()	{ return m_pTheString; }

	// array index operators
	char			operator[]( int i )		{ return m_pTheString[ i ]; }

	// input and output methods
	BOOL	Output( FILE* pFile ) const;
	BOOL	Input( FILE* pFile );

	void	Dump( char* szString ) const;

	BOOL			SetSize( int iSize );

protected :
	
	void	DeallocateString();
	BOOL	AllocateString( int iSize );
	
	char*	m_pTheString;
	int		m_iLength;		// does not include terminating null character
};



#endif