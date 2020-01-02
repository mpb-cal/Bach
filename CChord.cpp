
// Filename : chord.cpp
// Description : implementation of the CChord class
// Author : Marc P. Bernasconi

#include "stdafx.h"

#ifndef __CCHORD_H__
#include "CChord.h"
#endif

#include "constant.h"

CChord::CChord() :
	m_pNotes( NULL ),
	m_iNotes( 0 ),
	m_iTicks( 8 ),
	m_iMelodyNote( 0 )
{
	CreateMacro();
}

CChord::CChord( int p1 )
{
	m_iTicks = 8;
	m_iNotes = 1;
	m_pNotes = new CNote[ m_iNotes ];
	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_iMelodyNote = 0;
	CreateMacro();
}

CChord::CChord( int p1, int p2 )
{
	m_iTicks = 8;

	m_iNotes = 2;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3 )
{
	m_iTicks = 8;

	m_iNotes = 3;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4 )
{
	m_iTicks = 8;

	m_iNotes = 4;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5 )
{
	m_iTicks = 8;

	m_iNotes = 5;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6 )
{
	m_iTicks = 8;

	m_iNotes = 6;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7 )
{
	m_iTicks = 8;

	m_iNotes = 7;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8 )
{
	m_iTicks = 8;

	m_iNotes = 8;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_pNotes[ 7 ].SetAbsolutePitch( p8 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9 )
{
	m_iTicks = 8;

	m_iNotes = 9;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_pNotes[ 7 ].SetAbsolutePitch( p8 );
	m_pNotes[ 8 ].SetAbsolutePitch( p9 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10 )
{
	m_iTicks = 8;

	m_iNotes = 10;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_pNotes[ 7 ].SetAbsolutePitch( p8 );
	m_pNotes[ 8 ].SetAbsolutePitch( p9 );
	m_pNotes[ 9 ].SetAbsolutePitch( p10 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11 )
{
	m_iTicks = 8;

	m_iNotes = 11;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_pNotes[ 7 ].SetAbsolutePitch( p8 );
	m_pNotes[ 8 ].SetAbsolutePitch( p9 );
	m_pNotes[ 9 ].SetAbsolutePitch( p10 );
	m_pNotes[ 10 ].SetAbsolutePitch( p11 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::CChord( int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10, int p11, int p12 )
{
	m_iTicks = 8;

	m_iNotes = 12;
	m_pNotes = new CNote[ m_iNotes ];

	m_pNotes[ 0 ].SetAbsolutePitch( p1 );
	m_pNotes[ 1 ].SetAbsolutePitch( p2 );
	m_pNotes[ 2 ].SetAbsolutePitch( p3 );
	m_pNotes[ 3 ].SetAbsolutePitch( p4 );
	m_pNotes[ 4 ].SetAbsolutePitch( p5 );
	m_pNotes[ 5 ].SetAbsolutePitch( p6 );
	m_pNotes[ 6 ].SetAbsolutePitch( p7 );
	m_pNotes[ 7 ].SetAbsolutePitch( p8 );
	m_pNotes[ 8 ].SetAbsolutePitch( p9 );
	m_pNotes[ 9 ].SetAbsolutePitch( p10 );
	m_pNotes[ 10 ].SetAbsolutePitch( p11 );
	m_pNotes[ 11 ].SetAbsolutePitch( p12 );
	m_iMelodyNote = 0;

	CreateMacro();
}

CChord::~CChord()
{
	FreeMemory();
}

void CChord::SetPitches( int iPitches, int* pPitches )
{
	FreeMemory();

	m_pNotes = new CNote[ iPitches ];
	m_iNotes = iPitches;

	for (int i = 0; i < iPitches; i++)
	{
		m_pNotes[ i ].SetAbsolutePitch( pPitches[ i ] );
	}
}

void CChord::CreateMacro()
{
	lstrcpy( m_szMacro, "" );

	switch (GetPitch( 0 ))
	{
		case NOTE_REST : sprintf( m_szMacro, "%st ", m_szMacro ); break;
		
		default : 
			if (m_pNotes[ 0 ].IsSharp())
			{
				sprintf( m_szMacro, "%s3 ", m_szMacro );
			}
			else
			{
				sprintf( m_szMacro, "%sn ", m_szMacro );
			}
	}

	for (int i = 1; i < m_iNotes; i++)
	{
		int iCurrentAdjustedScaleDegree = 
			m_pNotes[ i ].GetOctave() * 7 + m_pNotes[ i ].GetScaleDegree();
		int iPreviousAdjustedScaleDegree = 
			m_pNotes[ i - 1 ].GetOctave() * 7 + m_pNotes[ i - 1 ].GetScaleDegree();

		sprintf(	m_szMacro, 
					"%sU1 u%d ", 
					m_szMacro, 
					iCurrentAdjustedScaleDegree - iPreviousAdjustedScaleDegree	);

		switch (GetPitch( i ))
		{
			case NOTE_REST : sprintf( m_szMacro, "%st ", m_szMacro ); break;
			
			default : 
				if (m_pNotes[ i ].IsSharp())
				{
					sprintf( m_szMacro, "%s3 ", m_szMacro );
				}
				else
				{
					sprintf( m_szMacro, "%sn ", m_szMacro );
				}
		}
	}

	sprintf( m_szMacro, "%sD%d", m_szMacro, m_iNotes - 1 );
}

const char* CChord::GetMacro()
{
	return m_szMacro;
}

int CChord::GetPitch( int iNumber )
{
	if (iNumber < m_iNotes)
	{
		return m_pNotes[ iNumber ].GetAbsolutePitch();
	}
	else
	{
		return -1;
	}
}

void CChord::StartMelody()
{
	m_iMelodyNote = 0;
}

int CChord::GetMelodyPitch()
{
	int iPitch = GetPitch( m_iMelodyNote );
	if (++m_iMelodyNote >= m_iNotes) m_iMelodyNote = 0;
	return iPitch;
}

void CChord::FreeMemory()
{
	if (m_pNotes)
	{
		delete[] m_pNotes;
		m_pNotes = NULL;
	}

	m_iNotes = 0;
}




