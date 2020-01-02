
#include "stdafx.h"

#ifndef __CSTRING_H__
#include "CString.h"
#endif


CString::CString() :
	m_pTheString( NULL ),
	m_iLength( 0 )
{
    m_iLength = 0;
}

CString::~CString()
{
	DeallocateString();
}

CString::CString( const CString& rvalue ) :
	m_pTheString( NULL ),
	m_iLength( 0 )
{
	*this = rvalue;
}

CString::CString( const char* rvalue ) :
	m_pTheString( NULL ),
	m_iLength( 0 )
{
	*this = rvalue;
}

const CString& CString::operator= ( const CString& rvalue )
{
	if (this != &rvalue)
	{		
		DeallocateString();
		AllocateString( rvalue.m_iLength + 1 );

		lstrcpy( m_pTheString, rvalue.m_pTheString );
	}

	return *this;
}

const CString& CString::operator= ( const char* rvalue )
{
	DeallocateString();
	AllocateString( lstrlen( rvalue ) + 1 );

	lstrcpy( m_pTheString, rvalue );

	return *this;
}

BOOL CString::operator==( const CString& rvalue ) const
{
	if (lstrcmp( m_pTheString, rvalue.m_pTheString ) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CString::operator==( const char* rvalue ) const
{
	if (lstrcmp( m_pTheString, rvalue ) == 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CString::operator!=( const CString& rvalue ) const
{
	if (lstrcmp( m_pTheString, rvalue.m_pTheString ) != 0)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CString::operator!=( const char* rvalue ) const
{
	if (lstrcmp( m_pTheString, rvalue ) != 0)
	{
		return TRUE;
	}

	return FALSE;
}

const CString CString::operator+ ( const CString& rvalue )
{
	CString NewString;
	NewString.SetSize( lstrlen( m_pTheString ) + lstrlen( rvalue.m_pTheString ) + 1 );
	lstrcpy( NewString.m_pTheString, m_pTheString );
	lstrcat( NewString.m_pTheString, rvalue.m_pTheString );
	return NewString;
}

BOOL CString::Output( FILE* pFile ) const
{
	if (fwrite( &m_iLength, sizeof( int ), 1, pFile ) < 1) return FALSE;
	if (fwrite( m_pTheString, sizeof( char ), m_iLength, pFile ) < m_iLength) return FALSE;

	return TRUE;
}

BOOL CString::Input( FILE* pFile )
{
	if (fread( &m_iLength, sizeof( int ), 1, pFile ) < 1) return FALSE;
	if (fread( m_pTheString, sizeof( char ), m_iLength, pFile ) < m_iLength) return FALSE;

	return TRUE;
}

void CString::Dump( char* szString ) const
{
}

BOOL CString::SetSize( int iSize )
{
	DeallocateString();
	return AllocateString( iSize );
}


////////////////////////////////////////////////////////////////////////
// protected methods


void CString::DeallocateString()
{
	if (m_pTheString)
	{
		delete[] m_pTheString;
	}
	m_pTheString = NULL;
}

BOOL CString::AllocateString( int iSize )
{
	if (m_pTheString = new char[ iSize ])
	{
		m_iLength = iSize - 1;
		return TRUE;
	}

	return FALSE;
}
