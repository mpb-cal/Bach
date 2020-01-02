// measure.cpp

#include "stdafx.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CMEASURE_H__
#include "CMeasure.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

#ifndef __CTICK_H__
#include "CTick.h"
#endif

#define MEASURE_WIDTH		(NOTE_WIDTH * (m_iTicksPerMeasure + 2))

CMeasure::CMeasure() : 
	CMusicalThing(), 
	CLinkedListItem(), 
	m_pTicks( NULL ),
	m_ppNotes( NULL ),
	m_iNotes( 0 ),
	m_iXpos( 0 ),
	m_iYpos( 0 )
{
	lstrcpy( m_szNumber, "" );
}

CMeasure::CMeasure(	int iKey, 
					int iXpos, 
					int iYpos, 
					int iNumber, 
					int iClef, 
					int iTicksPerMeasure	) :
	CMusicalThing(), 
	CLinkedListItem(), 
	m_pTicks( NULL ),
	m_ppNotes( NULL ),
	m_iNotes( 0 ),
	m_iXpos( iXpos ),
	m_iYpos( iYpos )
{
	m_iTicksPerMeasure = iTicksPerMeasure;
	m_iNumber = iNumber;
	m_iKey = iKey;
	m_iClef = iClef;

	sprintf( m_szNumber, "%d", m_iNumber + 1 );

	m_pTicks = new CTick[ m_iTicksPerMeasure ];
	m_pTicks[ 0 ].SetAttack( TRUE );

	m_ppNotes = new CNote*[ m_iTicksPerMeasure ];
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_ppNotes[ i ] = NULL;
	}

	MakeNotes();
}

CMeasure::~CMeasure()
{
	ClearNotes();
	if (m_pTicks) delete[] m_pTicks;
}

void CMeasure::MakeCopy( CLinkedListItem*& rpNewCopy )
{
	CMeasure* pMeasure = new CMeasure;
	*pMeasure = *this;
	rpNewCopy = pMeasure;
}

void CMeasure::Dump( char* szString ) const
{
	char sz[ 1024 ];

	sprintf( sz, "MEASURE:\r\n" );
	lstrcat( szString, sz );

	CMusicalThing::Dump( szString );

	sprintf( sz, "m_pTicks: " ); lstrcat( szString, sz );

	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_pTicks[ i ].Dump( szString );
	}
	lstrcat( szString, "\r\n" );

	for (i = 0; i < m_iNotes; i++)
	{
		m_ppNotes[ i ]->Dump( szString );
	}

	sprintf( sz, "m_iNotes = %d\r\n", m_iNotes ); lstrcat( szString, sz );
	sprintf( sz, "m_szNumber = %s\r\n", m_szNumber ); lstrcat( szString, sz );
	sprintf( sz, "m_iXpos = %d\r\n", m_iXpos ); lstrcat( szString, sz );
	sprintf( sz, "m_iYpos = %d\r\n", m_iYpos ); lstrcat( szString, sz );
}

const CMeasure&	CMeasure::operator=( const CMeasure& rvalue )
{
	CMusicalThing::operator=( rvalue );

	if (m_pTicks) delete[] m_pTicks;
	m_pTicks = new CTick[ m_iTicksPerMeasure ];
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_pTicks[ i ] = rvalue.m_pTicks[ i ];
	}

	lstrcpy( m_szNumber, rvalue.m_szNumber );
	m_iXpos = rvalue.m_iXpos;
	m_iYpos = rvalue.m_iYpos;;

	m_ppNotes = new CNote*[ m_iTicksPerMeasure ];
	for (i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_ppNotes[ i ] = NULL;
	}
	MakeNotes();

	return *this;
}

BOOL CMeasure::Output( FILE* pFile ) const
{
	if (!CMusicalThing::Output( pFile )) return FALSE;

	if (fwrite( &m_szNumber, sizeof( m_szNumber ), 1, pFile ) < 1 ) return FALSE;

	if (fwrite( &m_iXpos, sizeof( m_iXpos ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iYpos, sizeof( m_iYpos ), 1, pFile ) < 1 ) return FALSE;

	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		if (!m_pTicks[ i ].Output( pFile )) return FALSE;
	}

	return TRUE;
}

BOOL CMeasure::Input( FILE* pFile )
{
	if (!CMusicalThing::Input( pFile )) return FALSE;

	if (fread( &m_szNumber, sizeof( m_szNumber ), 1, pFile ) < 1) return FALSE;

	fpos_t pos;
	fgetpos( pFile, &pos );

	if (fread( &m_iXpos, sizeof( m_iXpos ), 1, pFile ) < 1) return FALSE;
	if (fread( &m_iYpos, sizeof( m_iYpos ), 1, pFile ) < 1) return FALSE;

	if (m_pTicks) delete[] m_pTicks;
	m_pTicks = new CTick[ m_iTicksPerMeasure ];

	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		if (!m_pTicks[ i ].Input( pFile )) return FALSE;
	}

	MakeNotes();

	return TRUE;
}

void CMeasure::ClearNotes()
{
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		if (m_ppNotes[ i ]) delete m_ppNotes[ i ];
		m_ppNotes[ i ] = NULL;
	}

	m_iNotes = 0;
}

void CMeasure::Draw( HDC hdc ) const
{
	int iXOrigin = CApplication::GetXOrigin();
	int iYOrigin = CApplication::GetYOrigin();

	int iYpos;
	if (CApplication::IsGrandStaff())
	{
		switch (m_iClef)
		{
			case CLEF_TREBLE :
				iYpos = STAFF_TOP;
				break;

			case CLEF_BASS :
				iYpos = STAFF_TOP + VOICE_HEIGHT;
				break;
		}
	}
	else
	{
		iYpos = m_iYpos;
	}

	// draw only if visible in scroll area
	if (iXOrigin + m_iXpos >= -(MEASURE_WIDTH) &&
		iXOrigin + (m_iXpos / CApplication::GetZoomDivisor()) <= CApplication::GetClientWindowWidth())
	{
		// draw notes
		for (int i = 0; i < m_iNotes; i++)
		{
			m_ppNotes[ i ]->Draw( hdc );
		}

		// draw bar line
		::MoveToEx(	hdc, 
					(iXOrigin + m_iXpos + MEASURE_WIDTH) / CApplication::GetZoomDivisor(), 
					(iYOrigin + iYpos) / CApplication::GetZoomDivisor(), 
					NULL	);
		::LineTo(	hdc, 
					(iXOrigin + m_iXpos + MEASURE_WIDTH) / CApplication::GetZoomDivisor(), 
					(iYOrigin + iYpos + STAFF_HEIGHT) / CApplication::GetZoomDivisor()	);

		if (CApplication::GetZoomDivisor() == 1)
		{
			// draw measure number (of next measure)
			char szNumberToDisplay[ 6 ];
			sprintf( szNumberToDisplay, "%d", m_iNumber + 2 );
			::TextOut(	hdc, 
						iXOrigin + (m_iXpos + MEASURE_WIDTH) - 4, 
						iYOrigin + iYpos - 10, 
						szNumberToDisplay,
						strlen( szNumberToDisplay )	);
		}
	}
}

void CMeasure::AddNote(	CNote* pNote, 
						int iTick, 
						BOOL blRest, 
						BOOL blSharp, 
						BOOL blRememberAttack	)
{

	// set appropriate ticks to pitch of pNote
	for (int i = 0; i < pNote->GetValue(); i++)
	{
		// stay within measure
		if (iTick >= m_iTicksPerMeasure)
		{
			break;
		}

		m_pTicks[ iTick ].
			SetPitch( (BYTE)((pNote->GetAbsolutePitch()) + (blSharp ? 1 : 0)) );

		if (i == 0 && blRememberAttack)
		{
			m_pTicks[ iTick ].SetAttack( TRUE );
		}
		else
		{
			m_pTicks[ iTick ].SetAttack( FALSE );
		}

		if (blRest)
		{
			m_pTicks[ iTick ].SetPitch( NOTE_REST );
		}

		iTick++;
	}

	if (blRememberAttack)
	{
		if (iTick < m_iTicksPerMeasure) m_pTicks[ iTick ].SetAttack( TRUE );
	}

	MakeNotes();
}

void CMeasure::Make( int iTop )
{
	m_iYpos = iTop;

	MakeNotes();
}

void CMeasure::Normalize( int iTop )
{
	m_iYpos = iTop;

	MakeNotes();
}

void CMeasure::MakeNotes()
{
	int iTicksUsed;
	int iTicksLeftOver = 0;

	ClearNotes();

	int iPitch = m_pTicks[ 0 ].GetPitch();
	int iValue = 1;
	int iXpos = m_iXpos + NOTE_WIDTH;

	for (int t = 1; t < m_iTicksPerMeasure; t++)
	{
		// add a note if there is an attack or pitch change on this tick
		if (m_pTicks[ t ].GetAttack() ||
			m_pTicks[ t ].GetPitch() != iPitch)
		{
			do
			{
				m_ppNotes[ m_iNotes++ ] = new CNote(	iPitch,
														m_iKey, 
														iValue, 
														m_iClef,
														iXpos, 
														m_iYpos, 
														iTicksUsed, 
														iTicksLeftOver	);

				iXpos += NOTE_WIDTH * iTicksUsed;
				iValue = iTicksLeftOver;

			} while (iTicksLeftOver);

			iValue = 0;
		}

		iPitch = m_pTicks[ t ].GetPitch();
		iValue++;
	}

	do
	{
		m_ppNotes[ m_iNotes++ ] = new CNote(	iPitch, 
												m_iKey, 
												iValue, 
												m_iClef, 
												iXpos, 
												m_iYpos,
												iTicksUsed, 
												iTicksLeftOver	);

		iXpos += NOTE_WIDTH * iTicksUsed;
		iValue = iTicksLeftOver;

	} while (iTicksLeftOver);
}

int CMeasure::GetNoteCount() const
{
	return m_iNotes;
}

void CMeasure::GetPitches( int*& rpiPitches ) const
{
	for (int i = 0; i < m_iNotes; i++)
	{
		*rpiPitches = m_ppNotes[ i ]->GetAbsolutePitch();
		rpiPitches++;
	}
}

void CMeasure::GetValues( int*& rpiValues ) const
{
	for (int i = 0; i < m_iNotes; i++)
	{
		*rpiValues = m_ppNotes[ i ]->GetValue();
		rpiValues++;
	}
}

BOOL CMeasure::SetTicksPerMeasure( int iTicksPerMeasure )
{
	ClearNotes();

	if (m_ppNotes) delete[] m_ppNotes;

	m_iTicksPerMeasure = iTicksPerMeasure;

	m_ppNotes = new CNote*[ m_iTicksPerMeasure ];
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_ppNotes[ i ] = NULL;
	}

	if (m_pTicks) delete[] m_pTicks;
	m_pTicks = new CTick[ m_iTicksPerMeasure ];

	return TRUE;
}

RECT CMeasure::GetRect() const
{
	RECT rc = 
	{
		m_iXpos, 
		m_iYpos, 
		m_iXpos + MEASURE_WIDTH, 
		m_iYpos + STAFF_HEIGHT
	};

	return rc;
}

int CMeasure::GetTickAtPoint( POINT p ) const
{
	int iTick = (p.x - m_iXpos) / NOTE_WIDTH - 1;

	if (iTick < 0)
	{
		iTick = 0;
	}

	if (iTick >= m_iTicksPerMeasure)
	{
		iTick = m_iTicksPerMeasure - 1;
	}

	return iTick;
}

void CMeasure::GetTicks( CTick* pTicks, int& rnIndex, int iFrom, int iTo ) const
{
	int i;
	for (i = iFrom; i <= iTo; i++, rnIndex++)
	{
		pTicks[ rnIndex ] = m_pTicks[ i ];
	}
}

void CMeasure::PutTicks( CTick* pTicks, int& rnIndex, int iFrom, int iTo )
{
	int i;
	for (i = iFrom; i <= iTo; i++, rnIndex++)
	{
		m_pTicks[ i ] = pTicks[ rnIndex ];
	}

	#ifdef _DEBUG

		CTick OneTick;
		for (int j = 0; j < m_iTicksPerMeasure; j++)
		{
			OneTick = m_pTicks[ j ];
		}

	#endif _DEBUG

	// put an attack on the next tick
	if (i < m_iTicksPerMeasure)
	{
		m_pTicks[ i ].SetAttack( TRUE );
	}

	MakeNotes();
}

int CMeasure::GetMaxLinesAbove() const
{
	int iMaxLinesAbove = 0;

	for (int i = 0; i < m_iNotes; i++)
	{
		iMaxLinesAbove = max( iMaxLinesAbove, m_ppNotes[ i ]->GetLedgerLines() );
	}

	return iMaxLinesAbove;
}

int CMeasure::GetMaxLinesBelow() const
{
	int iMaxLinesBelow = 0;

	for (int i = 0; i < m_iNotes; i++)
	{
		iMaxLinesBelow = min( iMaxLinesBelow, m_ppNotes[ i ]->GetLedgerLines() );
	}

	return iMaxLinesBelow;
}

void CMeasure::TransposeNotes( int iHalfSteps, ENoteDirection Direction, int iFrom, int iTo )
{
	switch (Direction)
	{
		case NOTE_UP : break;
		case NOTE_DOWN : iHalfSteps = -iHalfSteps; break;
	}

	for (int i = iFrom; i <= iTo; i++)
	{
		if (m_pTicks[ i ].GetPitch() != NOTE_REST)
		{
			m_pTicks[ i ].
				SetPitch( m_pTicks[ i ].GetPitch() + (BYTE)iHalfSteps );

			if (m_pTicks[ i ].GetPitch() < NOTE_BOTTOMPITCH) 
				m_pTicks[ i ].SetPitch( NOTE_BOTTOMPITCH );
			if (m_pTicks[ i ].GetPitch() > NOTE_TOPPITCH - 1) 
				m_pTicks[ i ].SetPitch( NOTE_TOPPITCH - 1 );
		}
	}

	MakeNotes();
}

void CMeasure::TransposeDiatonic( int iPitches, ENoteDirection Direction, int iFrom, int iTo )
{
	for (int i = iFrom; i <= iTo; i++)
	{
		if (m_pTicks[ i ].GetPitch() != NOTE_REST)
		{
			BYTE bPitch = m_pTicks[ i ].GetPitch();
			
			CNote::TransposeTickDiatonic( bPitch, iPitches, Direction, m_iKey );

			m_pTicks[ i ].SetPitch( bPitch );

			if (m_pTicks[ i ].GetPitch() < NOTE_BOTTOMPITCH) 
				m_pTicks[ i ].SetPitch( NOTE_BOTTOMPITCH );
			if (m_pTicks[ i ].GetPitch() > NOTE_TOPPITCH - 1) 
				m_pTicks[ i ].SetPitch( NOTE_TOPPITCH - 1 );
		}
	}

	MakeNotes();
}

BOOL CMeasure::HasNotes() const
{
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		if (m_pTicks[ i ].GetPitch() != NOTE_REST)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CMeasure::RemoveAttacks()
{
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		m_pTicks[ i ].SetAttack( FALSE );
	}
}



