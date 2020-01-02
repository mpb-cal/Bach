// CStaff.h

#include "stdafx.h"

#ifndef __CSTAFF_H__
#include "CStaff.h"
#endif

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CSONG_H__
#include "CSong.h"
#endif

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#include "resource.h"

CStaff::CStaff() :
	m_iLength( 0 ),
	m_iNumVoices( 0 )
{
	m_hMutePen = ::CreatePen( PS_SOLID, 1, RGB(200,200,200) );
}

CStaff::~CStaff()
{
	::DeleteObject( m_hMutePen );
}

const CStaff& CStaff::operator=( const CStaff& rvalue )
{
	m_iLength = rvalue.m_iLength;
	SetNumVoices( rvalue.m_iNumVoices );

	return *this;
}

void CStaff::Dump( char* szString ) const
{
}

BOOL CStaff::Output( FILE* pFile ) const
{
	if (fwrite( &m_iLength, sizeof( m_iLength ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iNumVoices, sizeof( m_iNumVoices ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iYpos, sizeof( m_iYpos ), 1, pFile ) < 1 ) return FALSE;
	
	return TRUE;
}

BOOL CStaff::Input( FILE* pFile )
{
	if (fread( &m_iLength, sizeof( m_iLength ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iNumVoices, sizeof( m_iNumVoices ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iYpos, sizeof( m_iYpos ), 1, pFile ) < 1 ) return FALSE;
	
	return TRUE;
}

void CStaff::SetNumVoices( int iNumVoices )
{
	ASSERT( iNumVoices > 0 && iNumVoices <= 256 );

	m_iNumVoices = iNumVoices;
	
	for (int i = 0; i < m_iNumVoices; i++)
	{
		m_iYpos[ i ] = STAFF_TOP + (VOICE_HEIGHT * i);
	}
}

BOOL CStaff::Draw( HDC hdc ) const
{
	int iStaffLength = CApplication::GetClientWindowWidth();
	if (m_iLength / CApplication::GetZoomDivisor() < iStaffLength)
	{
		iStaffLength = m_iLength / CApplication::GetZoomDivisor();
	}

	HPEN hBlackPen = (HPEN)::GetStockObject( BLACK_PEN );
	HPEN hOldPen = (HPEN)::SelectObject( hdc, hBlackPen );	

	for (int v = 0; v < m_iNumVoices; v++)
	{
		if (TheSong.IsMute( v ))
		{
			::SelectObject( hdc, m_hMutePen );
		}
		else
		{
			::SelectObject( hdc, TheSong.GetVoicePen( v ) );
		}

		// draw staff lines
		int y, i;
		for (y = m_iYpos[ v ] / CApplication::GetZoomDivisor(), i = 0; i < STAFF_LINES; y += STAFF_SPACEWIDTH / CApplication::GetZoomDivisor(), i++)
		{
			::MoveToEx( hdc, 0, CApplication::GetYOrigin() + y, NULL );
			::LineTo( hdc, iStaffLength, CApplication::GetYOrigin() + y );
		}
	}

	// clean up
	::SelectObject( hdc, hOldPen );
	
	return TRUE;
}

