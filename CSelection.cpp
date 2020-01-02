
// Filename : CSelection.cpp
// Description : implementation of the CSelection class
// Author : Marc P. Bernasconi

#include "stdafx.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CSELECTION_H__
#include "CSelection.h"
#endif

#ifndef __CTICK_H__
#include "CTick.h"
#endif

#ifndef __CSONG_H__
#include "CSong.h"
#endif

#include "GlobalDefs.h"

 
CSelection::CSelection() :
	m_iTicks( 0 ),
	m_iVoices( 0 ),
	m_ppTicks( NULL ),
	m_blEnabled( FALSE ),
	m_iCurrentX( 0 ),
	m_iPreviousX( 0 ),
	m_iFirstX( 0 ),
	m_iCurrentY( 0 ),
	m_iPreviousY( 0 ),
	m_iFirstY( 0 )
{
}

CSelection::~CSelection()
{
	FreeTicks();
}

void CSelection::FreeTicks()
{
	if (m_ppTicks)
	{
		for (int i = 0; i < m_iVoices; i++)
		{
			delete[] m_ppTicks[ i ];
		}
		delete[] m_ppTicks;
		m_ppTicks = NULL;
	}

	m_iVoices = 0;
	m_iTicks = 0;
}

void CSelection::GetTicks( char* pcBuffer )
{
// returns number of ticks (int), number of voices (int), and ticks in pcBuffer

	if (HasData())
	{
		memcpy( pcBuffer, (LPBYTE)&m_iTicks, sizeof m_iTicks );
		pcBuffer += sizeof m_iTicks;

		memcpy( pcBuffer, (LPBYTE)&m_iVoices, sizeof m_iVoices );
		pcBuffer += sizeof m_iVoices;

		for (int i = 0; i < m_iVoices; i++)
		{
			for (int j = 0; j < m_iTicks; j++)
			{
				*(CTick*)pcBuffer = m_ppTicks[ i ][ j ];

				pcBuffer += sizeof CTick;
			}
		}
	}
}

void CSelection::SetTicks( char* pcBuffer )
{
// sets number of ticks (int), number of voices (int), and ticks from pcBuffer

	// delete old data
	FreeTicks();

	// get number of ticks and voices from pcBuffer
	m_iTicks = *(int*)pcBuffer;
	pcBuffer += sizeof m_iTicks;

	m_iVoices = *(int*)pcBuffer;
	pcBuffer += sizeof m_iVoices;

	// allocate array of arrays of ticks
	m_ppTicks = new CTick*[ m_iVoices ];

	// allocate arrays of ticks and set them to values from pcBuffer
	for (int i = 0; i < m_iVoices; i++)
	{
		m_ppTicks[ i ] = new CTick[ m_iTicks ];

		for (int j = 0; j < m_iTicks; j++)
		{
			m_ppTicks[ i ][ j ] = *(CTick*)pcBuffer;

			pcBuffer += sizeof CTick;
		}
	}
}

void CSelection::LoadTicks( CSong* pSong )
{
// sets ticks from song data

	// delete old data
	FreeTicks();

	CPosition posStart = m_posStart;
	CPosition posStop = m_posStop;

	m_iVoices = abs( m_posStart.GetVoice() - m_posStop.GetVoice() ) + 1;

	int i;

	if (m_posStart < m_posStop)
	{
		posStop.SetAbsoluteTick( posStop.GetAbsoluteTick() - 1 );

		m_iTicks = m_posStop.GetAbsoluteTick() - m_posStart.GetAbsoluteTick();

		// allocate array of arrays of ticks
		m_ppTicks = new CTick*[ m_iVoices ];

		for (i = 0; i < m_iVoices; i++)
		{
			m_ppTicks[ i ] = new CTick[ m_iTicks ];

			pSong->GetTicks( m_ppTicks[ i ], posStart, posStop );

			#ifdef _DEBUG
/*
				CTick OneTick;
				for (j = 0; j < m_poaTicks[ i ].GetSize(); j++)
				{
					OneTick = *(CTick*)m_poaTicks[ i ][ j ];
				}
*/
			#endif _DEBUG

			posStart.SetVoice( posStart.GetVoice() + 1 );
			posStop.SetVoice( posStop.GetVoice() + 1 );
		}
	}
	else if (m_posStart > m_posStop)
	{
		posStop.SetAbsoluteTick( posStop.GetAbsoluteTick() + 1 );

		m_iTicks = m_posStart.GetAbsoluteTick() - m_posStop.GetAbsoluteTick();

		// allocate array of arrays of ticks
		m_ppTicks = new CTick*[ m_iVoices ];

		for (i = 0; i < m_iVoices; i++)
		{
			m_ppTicks[ i ] = new CTick[ m_iTicks ];

			pSong->GetTicks( m_ppTicks[ i ], posStop, posStart );

			posStart.SetVoice( posStart.GetVoice() + 1 );
			posStop.SetVoice( posStop.GetVoice() + 1 );
		}
	}
}

void CSelection::PutTicks(	CSong* pSong,
							CPosition posFrom, 
							int iTicksPerMeasure,
							int iKey	)
{
// pastes ticks into song

	CPosition posTo;
	posTo.Initialize(	posFrom.GetVoice(), 
						posFrom.GetAbsoluteTick() + m_iTicks - 1,
						iTicksPerMeasure	);

	for (int i = 0; i < m_iVoices; i++)
	{
		pSong->PutTicks( m_ppTicks[ i ], posFrom, posTo, iKey );

		posFrom.SetVoice( posFrom.GetVoice() + 1 );
		posTo.SetVoice( posTo.GetVoice() + 1 );
	}
}

void CSelection::TransposeNotes( CSong* pSong, int iHalfSteps, ENoteDirection Direction )
{
	if (m_posStart < m_posStop)
	{
		pSong->TransposeNotes( iHalfSteps, Direction, m_posStart, m_posStop );
	}
	else if (m_posStart > m_posStop)
	{
		pSong->TransposeNotes( iHalfSteps, Direction, m_posStop, m_posStart );
	}
}

void CSelection::TransposeDiatonic( CSong* pSong, int iPitches, ENoteDirection Direction )
{
	if (m_posStart < m_posStop)
	{
		pSong->TransposeDiatonic( iPitches, Direction, m_posStart, m_posStop );
	}
	else if (m_posStart > m_posStop)
	{
		pSong->TransposeDiatonic( iPitches, Direction, m_posStop, m_posStart );
	}
}

void CSelection::Enable(	int iVoice, 
							int iMeasure, 
							int iTick, 
							int iX, 
							int iFirstY, 
							int iCurrentY,
							int iTicksPerMeasure	)
{
	m_blEnabled = TRUE;

	m_posStart.Initialize( iVoice, iMeasure, iTick, iTicksPerMeasure );
	m_posStop.Initialize( iVoice, iMeasure, iTick, iTicksPerMeasure );

	m_iCurrentX = iX;
	m_iPreviousX = iX;
	m_iFirstX = iX;

	m_iCurrentY = iCurrentY;
	m_iPreviousY = iFirstY;
	m_iFirstY = iFirstY;
}

void CSelection::Disable()
{
	if (m_blEnabled)
	{
		m_blEnabled = FALSE;

		m_posStart.Initialize( 0, 0, 0, 0 );
		m_posStop.Initialize( 0, 0, 0, 0 );

		m_iCurrentX = 0;
		m_iPreviousX = 0;
		m_iFirstX = 0;

		m_iCurrentY = 0;
		m_iPreviousY = 0;
		m_iFirstY = 0;
	}
}

void CSelection::SetX( int iVoice, int iMeasure, int iTick, int iXpos, int iTicksPerMeasure )
{
	m_posStop.Initialize( iVoice, iMeasure, iTick, iTicksPerMeasure );

	m_iPreviousX = m_iCurrentX;
	m_iCurrentX = iXpos;
}

void CSelection::SetY( int iVoice, int iMeasure, int iTick, int iYpos, int iTicksPerMeasure )
{
	m_posStop.Initialize( iVoice, iMeasure, iTick, iTicksPerMeasure );

	m_iPreviousY = m_iCurrentY;
	m_iCurrentY = iYpos;
}

void CSelection::Fill( int iPitch, int iTicks )
{
// sets all ticks to same pitch; sets all attacks to FALSE

	// delete old data
	FreeTicks();

	m_iTicks = iTicks;

	m_iVoices = abs( m_posStart.GetVoice() - m_posStop.GetVoice() ) + 1;

	m_ppTicks = new CTick*[ m_iVoices ];

	for (int i = 0; i < m_iVoices; i++)
	{
		m_ppTicks[ i ] = new CTick[ m_iTicks ];

		for (int j = 0; j < m_iTicks; j++)
		{
			m_ppTicks[ i ][ j ].SetPitch( iPitch );
			m_ppTicks[ i ][ j ].SetAttack( FALSE );
		}
	}
}

BOOL CSelection::DrawNowX( HDC hdc ) const
{
	if (m_blEnabled)
	{
		HPEN hOldPen = (HPEN)::SelectObject( hdc, ::GetStockObject( NULL_PEN ) );
		HBRUSH hOldBrush = (HBRUSH)::SelectObject( hdc, ::GetStockObject( BLACK_BRUSH ) );

		int iOldMode = ::SetROP2( hdc, R2_NOT );

		int iXOrigin = CApplication::GetXOrigin();
		int iYOrigin = CApplication::GetYOrigin();

		RECT rc = 
		{ 
			iXOrigin + m_iPreviousX, 
			iYOrigin + m_iFirstY, 
			iXOrigin + m_iCurrentX, 
			iYOrigin + m_iCurrentY 
		};

		rc.left = max( rc.left, 0 );
		rc.right = min( rc.right, CApplication::GetClientWindowWidth() );

		NormalizeRectangle( &rc );

		::Rectangle( hdc, rc.left, rc.top, rc.right + 1, rc.bottom + 1 );

		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		::SetROP2( hdc, iOldMode );
	}

	return TRUE;
}

BOOL CSelection::DrawNowY( HDC hdc ) const
{
	if (m_blEnabled)
	{
		HPEN hOldPen = (HPEN)::SelectObject( hdc, ::GetStockObject( NULL_PEN ) );
		HBRUSH hOldBrush = (HBRUSH)::SelectObject( hdc, ::GetStockObject( BLACK_BRUSH ) );

		int iOldMode = ::SetROP2( hdc, R2_NOT );

		int iXOrigin = CApplication::GetXOrigin();
		int iYOrigin = CApplication::GetYOrigin();

		RECT rc = 
		{ 
			iXOrigin + m_iFirstX, 
			iYOrigin + m_iPreviousY, 
			iXOrigin + m_iCurrentX, 
			iYOrigin + m_iCurrentY 
		};

		rc.left = max( rc.left, 0 );
		rc.right = min( rc.right, CApplication::GetClientWindowWidth() );

		NormalizeRectangle( &rc );

		::Rectangle( hdc, rc.left, rc.top, rc.right + 1, rc.bottom + 1 );

		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		::SetROP2( hdc, iOldMode );
	}

	return TRUE;
}

BOOL CSelection::Draw( HDC hdc ) const
{
	if (m_blEnabled)
	{
		HPEN hOldPen = (HPEN)::SelectObject( hdc, ::GetStockObject( NULL_PEN ) );
		HBRUSH hOldBrush = (HBRUSH)::SelectObject( hdc, ::GetStockObject( BLACK_BRUSH ) );

		int iOldMode = ::SetROP2( hdc, R2_NOT );

		int iXOrigin = CApplication::GetXOrigin();
		int iYOrigin = CApplication::GetYOrigin();

		RECT rc = 
		{
			iXOrigin + m_iFirstX, 
			iYOrigin + m_iFirstY, 
			iXOrigin + m_iCurrentX, 
			iYOrigin + m_iCurrentY 
		};

		rc.left = max( rc.left, 0 );
		rc.right = min( rc.right, CApplication::GetClientWindowWidth() );

		NormalizeRectangle( &rc );

		::Rectangle( hdc, rc.left, rc.top, rc.right + 1, rc.bottom + 1 );

		::SelectObject( hdc, hOldPen );
		::SelectObject( hdc, hOldBrush );
		::SetROP2( hdc, iOldMode );
	}

	return TRUE;
}



