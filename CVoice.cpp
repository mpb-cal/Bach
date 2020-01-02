
// CVoice.cpp

#include "stdafx.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#ifndef __CVOICE_H__
#include "CVoice.h"
#endif

#ifndef __CTICK_H__
#include "CTick.h"
#endif

#ifndef __CMEASURE_H__
#include "CMeasure.h"
#endif

#ifndef __CMIDIFILE_H__
#include "CMidiFile.h"
#endif

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

#ifndef __CCURSOR_H__
#include "CCursor.h"
#endif

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif

#ifndef __CPOSITION_H__
#include "CPosition.h"
#endif

#include "CDialogVoiceSettings.h"

#include "GlobalDefs.h"


#define MEASURE_WIDTH		(NOTE_WIDTH * (m_iTicksPerMeasure + 2))


CVoice::CVoice( int iNumber ) :
	CMusicalThing(),
	CLinkedListItem(),
	m_iTop( 0 ),
	m_iRoomAbove( 0 ),
	m_iRoomBelow( 0 ),
	m_iClefBitmap( 0 ),
	m_blMute( FALSE ),
	m_crColor( RGB( 0, 0, 0 ) ),
	m_hPen( NULL )
//	m_pWave( NULL )
{
	m_iNumber = iNumber;
	m_hPen = ::CreatePen( PS_SOLID, 1, m_crColor );
	lstrcpy( m_szWaveFile, "" );
}

CVoice::~CVoice()
{
	::DeleteObject( m_hPen );
}

void CVoice::MakeCopy( CLinkedListItem*& rpNewCopy )
{
	CVoice* pVoice = new CVoice( m_iNumber );
	*pVoice = *this;
	rpNewCopy = pVoice;
}

void CVoice::Dump( char* szString ) const
{
	char sz[ 1024 ];

	sprintf( sz, "START VOICE\r\n" ); lstrcat( szString, sz );

	CMusicalThing::Dump( szString );
	m_MeasureList.Dump( szString );
	sprintf( sz, "m_iTop = %d\r\n", m_iTop ); lstrcat( szString, sz );
	sprintf( sz, "m_iRoomAbove = %d\r\n", m_iRoomAbove ); lstrcat( szString, sz );
	sprintf( sz, "m_iRoomBelow = %d\r\n", m_iRoomBelow ); lstrcat( szString, sz );
	sprintf( sz, "m_iClefBitmap = %d\r\n", m_iClefBitmap ); lstrcat( szString, sz );
	sprintf( sz, "m_blMute = %d\r\n", m_blMute ); lstrcat( szString, sz );
	sprintf( sz, "m_crColor = %d\r\n", m_crColor ); lstrcat( szString, sz );
	sprintf( sz, "m_hPen = %d\r\n", m_hPen ); lstrcat( szString, sz );

	sprintf( sz, "END VOICE\r\n\r\n" ); lstrcat( szString, sz );
}

void CVoice::Init(	int iChannel, 
					int iVelocity,
					int iTop, 
					int iClef, 
					int iTicksPerMeasure	)
{
	ASSERT (iChannel >= CHANNEL_MIN && iChannel <= CHANNEL_MAX);

	m_iTop = iTop;
	m_iRoomAbove = STAFF_TOP;
	m_iRoomBelow = STAFF_TOP;
	m_iTicksPerMeasure = iTicksPerMeasure;
	m_blMute = FALSE;

	SetPatch( 0 );
	SetChannel( iChannel );
	SetClef( iClef );
	SetVelocity( iVelocity );

	// start with one measure
	m_MeasureList.AppendItem(	new CMeasure(	CMAJOR, 
												MEASURE_FIRSTX, 
												m_iTop + m_iRoomAbove, 
												0, 
												m_iClef, 
												m_iTicksPerMeasure	)	);
}

void CVoice::Reset()
{
/*
	if (m_pWave == NULL)
	{
		delete m_pWave;
		m_pWave = NULL;
	}
	lstrcpy( m_szWaveFile, "" );
*/
}

BOOL CVoice::Output( FILE* pFile ) const
{
	if (!CMusicalThing::Output( pFile )) return FALSE;

	if (fwrite( &m_iTop, sizeof( m_iTop ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iRoomAbove, sizeof( m_iRoomAbove ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iRoomBelow, sizeof( m_iRoomBelow ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iClefBitmap, sizeof( m_iClefBitmap ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_blMute, sizeof( m_blMute ), 1, pFile ) < 1 ) return FALSE;

	int iMeasures = m_MeasureList.GetSize();
	if (fwrite( &iMeasures, sizeof( iMeasures ), 1, pFile ) < 1 ) return FALSE;

	if (fwrite( &m_crColor, sizeof( m_crColor ), 1, pFile ) < 1 ) return FALSE;

	m_MeasureList.Output( pFile );

	return TRUE;
}

BOOL CVoice::Input( FILE* pFile )
{
	if (!CMusicalThing::Input( pFile )) return FALSE;

	if (fread( &m_iTop, sizeof( m_iTop ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iRoomAbove, sizeof( m_iRoomAbove ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iRoomBelow, sizeof( m_iRoomBelow ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_iClefBitmap, sizeof( m_iClefBitmap ), 1, pFile ) < 1 ) return FALSE;
	if (fread( &m_blMute, sizeof( m_blMute ), 1, pFile ) < 1 ) return FALSE;

	int iMeasures;
	if (fread( &iMeasures, sizeof( iMeasures ), 1, pFile ) < 1 ) return FALSE;

	COLORREF crColor;
	if (fread( &crColor, sizeof( crColor ), 1, pFile ) < 1 ) return FALSE;
	SetColor( crColor );

	m_MeasureList.RemoveAll();
	m_MeasureList.AppendItem(	new CMeasure(	CMAJOR, 
												MEASURE_FIRSTX, 
												m_iTop + m_iRoomAbove, 
												0, 
												m_iClef, 
												m_iTicksPerMeasure	)	);
	for (int i = 1; i < iMeasures; i++)
	{
		AddMeasure( CMAJOR );
	}

	m_MeasureList.Input( pFile );

	return TRUE;
}

CMeasure* CVoice::GetMeasure( int iMeasure ) const
{
	return ((CMeasure*)(m_MeasureList[ iMeasure ]));
}

CMeasure* CVoice::GetLastMeasure() const
{
	return ((CMeasure*)(m_MeasureList[ m_MeasureList.GetSize() - 1 ]));
}

void CVoice::Draw( HDC hdc ) const
{
	int iXOrigin = CApplication::GetXOrigin();
	int iYOrigin = CApplication::GetYOrigin();

	int iTop;
	int iRoomAbove;

	if (CApplication::IsGrandStaff())
	{
		switch (m_iClef)
		{
			case CLEF_TREBLE :
				iTop = 0;
				iRoomAbove = STAFF_TOP;
				break;

			case CLEF_BASS :
				iTop = VOICE_HEIGHT;
				iRoomAbove = STAFF_TOP;
				break;
		}
	}
	else
	{
		iTop = m_iTop;
		iRoomAbove = m_iRoomAbove;
	}

	// draw info only if visible in scroll area
	if (iXOrigin >= -(CApplication::GetClientWindowWidth()))
	{
		if (CApplication::GetZoomDivisor() == 1)
		{
			// cover beginning of staff with white rectangle
			HPEN	hOldPen		= (HPEN)::SelectObject( hdc, ::GetStockObject( NULL_PEN ) );
			HBRUSH	hOldBrush	= (HBRUSH)::SelectObject( hdc, ::GetStockObject( WHITE_BRUSH ) );

			::Rectangle(	hdc, 
							iXOrigin, 
							(iYOrigin + iTop) / CApplication::GetZoomDivisor(), 
							(iXOrigin + MEASURE_FIRSTX - 45) / CApplication::GetZoomDivisor(), 
							(iYOrigin + iTop + iRoomAbove + 36) / CApplication::GetZoomDivisor()	);

			// show voice info
			char szChannel[ 20 ];
			sprintf( szChannel, "channel: %d  ", m_iChannel );

			char szPatch[ 128 ];
			if (m_iChannel == CHANNEL_DRUM)
			{
				lstrcpy( szPatch, "Percussion" );
			}
			else
			{
				lstrcpy( szPatch, g_pszPatches[ m_iPatch ] );
			}

			char szVoice[ 64 ];
			sprintf( szVoice, "Voice: %d  ", m_iNumber + 1 );

			::TextOut(	hdc, 
						iXOrigin / CApplication::GetZoomDivisor(), 
						(iYOrigin + iTop + iRoomAbove - 8) / CApplication::GetZoomDivisor(),
						szVoice, 
						lstrlen( szVoice )	);

			::TextOut(	hdc, 
						iXOrigin / CApplication::GetZoomDivisor(), 
						(iYOrigin + iTop + iRoomAbove + 8) / CApplication::GetZoomDivisor(),
						szPatch,
						lstrlen( szPatch )	);

			::TextOut(	hdc, 
						iXOrigin / CApplication::GetZoomDivisor(), 
						(iYOrigin + iTop + iRoomAbove + 24) / CApplication::GetZoomDivisor(),
						szChannel, 
						lstrlen( szChannel )	);

			// draw vertical line at beginning of staff
			::SelectObject( hdc, ::GetStockObject( BLACK_PEN ) );
			
			::MoveToEx(	hdc, 
						(iXOrigin + MEASURE_FIRSTX - 46) / CApplication::GetZoomDivisor(), 
						(iYOrigin + iTop + iRoomAbove) / CApplication::GetZoomDivisor(), 
						NULL	);
			
			::LineTo(	hdc,
						(iXOrigin + MEASURE_FIRSTX - 46) / CApplication::GetZoomDivisor(), 
						(iYOrigin + iTop + iRoomAbove + STAFF_HEIGHT) / CApplication::GetZoomDivisor()	);

			::SelectObject( hdc, hOldPen );
			::SelectObject( hdc, hOldBrush );
		}

		// draw clef
		CBMStorage::DrawBitmap(	hdc, 
								(iXOrigin + MEASURE_FIRSTX - 40) / CApplication::GetZoomDivisor(), 
								(iYOrigin + iTop + iRoomAbove - 10) / CApplication::GetZoomDivisor(), 
								CApplication::GetZoomDivisor(),
								m_iClefBitmap	);
	}

	// draw measures
	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		((CMeasure*)(m_MeasureList[ i ]))->Draw( hdc );
	}
}

int CVoice::AddNote(	CNote* pNote, 
						int iTick, 
						int iMeasure, 
						BOOL blRest, 
						BOOL blSharp,
						int& rnCursorYpos, 
						BOOL blRememberAttack	)
{

	// returns:
	//	UPDATE_MEASURE if space between voices doesn't change
	//  UPDATE_SONG if space between voices changes

	((CMeasure*)(m_MeasureList[ iMeasure ]))->AddNote(	pNote, 
														iTick, 
														blRest,	
														blSharp, 
														blRememberAttack	);

	int iDummy1, iDummy2;
	CNote* pNewNote = new CNote(	pNote->GetAbsolutePitch(), 
									pNote->GetKey(),
									pNote->GetValue(), 
									m_iClef, 
									0, 
									0, 
									iDummy1, 
									iDummy2	);

	int iLedgerLines = pNewNote->GetLedgerLines();

	if (iLedgerLines > 0)
	{
		if (m_iRoomAbove <= (iLedgerLines + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT)
		{
			int iOldRoomAbove = m_iRoomAbove;
			m_iRoomAbove = (iLedgerLines + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT;

			rnCursorYpos += m_iRoomAbove - iOldRoomAbove;
			
			return UPDATE_SONG;
		}
	}
	else if (iLedgerLines < 0)
	{
		if (m_iRoomBelow <= (-iLedgerLines + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT)
		{
			m_iRoomBelow = (-iLedgerLines + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT;
			return UPDATE_SONG;
		}
	}

	return UPDATE_MEASURE;
}

void CVoice::ClearNotes()
// puts a whole-note rest in each measure
{
	int iDummy1, iDummy2;

	CNote WholeNoteRest( 0, 0, m_iTicksPerMeasure, m_iClef, 0, 0, iDummy1, iDummy2 );

	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		((CMeasure*)(m_MeasureList[ i ]))->AddNote( &WholeNoteRest, 0, TRUE, FALSE, FALSE );
	}

	m_iRoomAbove = STAFF_SPACEWIDTH * 3;
	m_iRoomBelow = STAFF_SPACEWIDTH * 3;
}

void CVoice::AddMeasure( int iKey )
{
	m_MeasureList.AppendItem(	new CMeasure(	iKey, 
												((CMeasure*)
													(m_MeasureList[ m_MeasureList.GetSize() - 1 ]))->
													GetXpos() + MEASURE_WIDTH, 
												m_iTop + m_iRoomAbove, 
												m_MeasureList.GetSize(), 
												m_iClef,
												m_iTicksPerMeasure	)	);
}

BOOL CVoice::ChopMeasure()
{
	if (m_MeasureList.GetSize() > 1)
	{
		m_MeasureList.ChopItem();
		return TRUE;
	}

	return FALSE;
}

BOOL CVoice::HasNotesInLastMeasure() const
{
	return GetLastMeasure()->HasNotes();
}

int CVoice::GetMeasureCount() const
{
	return m_MeasureList.GetSize();
}

void CVoice::ExportMidiFile(	CMidiFile& mfile, 
								int iNumber, 
								BOOL blSwingIt,
								CPosition& rposFrom, 
								CPosition& rposTo	) const
{

	int iNoteCount		=	0;
	int* piPitchesTemp	=	NULL;
	int* piPitches		=	NULL;
	int* piValuesTemp	=	NULL;
	int* piValues		=	NULL;

	if (!m_blMute)
	{
		for (int i = 0; i < m_MeasureList.GetSize(); i++)
		{
			if (i >= rposFrom.GetMeasure() && i <= rposTo.GetMeasure())
			{
				iNoteCount += ((CMeasure*)(m_MeasureList[ i ]))->GetNoteCount();
			}
		}

		piPitches = new int[ iNoteCount ];
		piPitchesTemp = piPitches;

		piValues = new int[ iNoteCount ];
		piValuesTemp = piValues;

		for (i = 0; i < m_MeasureList.GetSize(); i++)
		{
			if (i >= rposFrom.GetMeasure() && i <= rposTo.GetMeasure())
			{
				((CMeasure*)(m_MeasureList[ i ]))->GetPitches( piPitchesTemp );
				((CMeasure*)(m_MeasureList[ i ]))->GetValues( piValuesTemp );
			}
		}
	}

	if (blSwingIt)
	{
		int iTick = 0;

		for (int i = 0; i < iNoteCount; i++)
		{
			int iValue = piValues[ i ];
			if ((iTick - NOTE_QUARTER / 2) % NOTE_QUARTER == 0)
			{
				--(piValues[ i ]);
				++(piValues[ i - 1 ]);
			}

			iTick += iValue;
			iTick = iTick % m_iTicksPerMeasure;
		}
	}

	mfile.edit_track(	iNumber, 
						iNoteCount, 
						piPitches,
						piValues, 
						m_iPatch, 
						TheCursor.GetPan( m_iChannel ),
						m_iChannel,
						m_iVelocity	);

	if (piPitches)		delete[] piPitches;
	if (piValues)		delete[] piValues;
}

void CVoice::Make( int& riTop )
{
	// rebuild measures

	m_iTop = riTop;

	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		((CMeasure*)(m_MeasureList[ i ]))->Make( riTop + m_iRoomAbove );
	}

	// return the top of the next voice in riTop
	riTop += m_iRoomAbove + STAFF_HEIGHT + m_iRoomBelow;
}

void CVoice::Normalize( int& riTop )
{

	m_iTop = riTop;

	// reset space above and below to defaults
	m_iRoomAbove = STAFF_TOP;
	m_iRoomBelow = STAFF_TOP;

	int iMaxLinesAbove = 0;
	int iMaxLinesBelow = 0;

	// find extra space needed above and below
	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		iMaxLinesAbove = max( iMaxLinesAbove, ((CMeasure*)(m_MeasureList[ i ]))->GetMaxLinesAbove() );
		iMaxLinesBelow = min( iMaxLinesBelow, ((CMeasure*)(m_MeasureList[ i ]))->GetMaxLinesBelow() );
	}

	if (iMaxLinesAbove > 0)
	{
		if (m_iRoomAbove <= (iMaxLinesAbove + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT)
		{
			m_iRoomAbove = (iMaxLinesAbove + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT;
		}
	}
	
	if (iMaxLinesBelow < 0)
	{
		if (m_iRoomBelow <= (-iMaxLinesBelow + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT)
		{
			m_iRoomBelow = (-iMaxLinesBelow + 1) * STAFF_SPACEWIDTH + NOTE_ACCHEIGHT;
		}
	}

	// rebuild measures
	for (i = 0; i < m_MeasureList.GetSize(); i++)
	{
		((CMeasure*)(m_MeasureList[ i ]))->Make( riTop + m_iRoomAbove );
	}

	// return the top of the next voice in riTop
	riTop += m_iRoomAbove + STAFF_HEIGHT + m_iRoomBelow;
}

int CVoice::OnSettings( HINSTANCE hInstance, HWND hwndParent )
{
	int iUpdate = UPDATE_NONE;
	int iOldClef = m_iClef;

	CDialogVoiceSettings dvs;
	
	dvs.m_iChannel = m_iChannel;
	dvs.m_iVelocity = m_iVelocity;
	dvs.m_iPatch = m_iPatch;
	dvs.m_iVoice = m_iNumber + 1;
	dvs.m_iClef = m_iClef;
	dvs.m_blMute = m_blMute;
	dvs.m_crColor = m_crColor;

	if (dvs.DoModal( hInstance, hwndParent ) == IDOK)
	{
		m_iChannel = dvs.m_iChannel;
		m_iVelocity = dvs.m_iVelocity;
		m_iPatch = dvs.m_iPatch;
		m_blMute = dvs.m_blMute;
		SetColor( dvs.m_crColor );
		SetClef( dvs.m_iClef );

		iUpdate = UPDATE_VIEW | UPDATE_DOC;
	}

	if (iOldClef != m_iClef) 
	{
		iUpdate |= UPDATE_VIEW | UPDATE_DOC;

		for (int i = 0; i < m_MeasureList.GetSize(); i++)
		{
			((CMeasure*)(m_MeasureList[ i ]))->SetClef( m_iClef );
		}
	}

	return iUpdate;
}

RECT CVoice::GetRect( int iMeasure ) const
{
	// iMeasure = -1 : all measures

	RECT rc = { 0, 0, 0, 0 };

	if (iMeasure == -1)
	{
		// get rect from first measure
		rc = ((CMeasure*)(m_MeasureList[ 0 ]))->GetRect();

		// stretch right side of rect to include the rest of the measures
		for (int i = 0; i < m_MeasureList.GetSize(); i++)
		{
			rc.right = ((CMeasure*)(m_MeasureList[ i ]))->GetRect().right;
		}
	}
	else
	{		
		rc = ((CMeasure*)(m_MeasureList[ iMeasure ]))->GetRect();
	}

	rc.left -= MEASURE_FIRSTX;
	rc.top -= m_iRoomAbove;
	rc.bottom += m_iRoomBelow;

	return rc;
}

int CVoice::GetMeasureAtPoint( POINT p ) const
{
	RECT rc;

	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		rc = ((CMeasure*)(m_MeasureList[ i ]))->GetRect();

		if (::PtInRect( &rc, p ))
		{
			return i;
		}
	}

	return -1;
}

int CVoice::GetTickAtPoint( POINT p ) const
{
	int iMeasure = GetMeasureAtPoint( p );
	if (iMeasure != -1)
	{
		return	iMeasure * 
				m_iTicksPerMeasure + 
				((CMeasure*)(m_MeasureList[ iMeasure ]))->GetTickAtPoint( p );
	}

	return -1;
}

BOOL CVoice::SetClef( int iClef )
{
	if (CMusicalThing::SetClef( iClef ))
	{
		switch (m_iClef)
		{
			case CLEF_TREBLE : m_iClefBitmap = BM_TREBLECLEF; break;
			case CLEF_BASS : m_iClefBitmap = BM_BASSCLEF; break;
		}

		for (int i = 0; i < m_MeasureList.GetSize(); i++)
		{
			((CMeasure*)(m_MeasureList[ i ]))->SetClef( m_iClef );
		}

		return TRUE;
	}

	return FALSE;
}

void CVoice::GetTicks(	CTick* pTicks, 
						const CPosition& rposFrom, 
						const CPosition& rposTo	) const
{
	// get ticks iiTo consecutive measures

	int iIndex = 0; // will be incremented by CMeasure::PutTicks()
	for (int i = rposFrom.GetMeasure(); i <= rposTo.GetMeasure(); i++)
	{
		int iFrom = 0;
		int iTo = m_iTicksPerMeasure - 1;

		if (i == rposFrom.GetMeasure()) // first measure
		{
			iFrom = rposFrom.GetTick();
		}
		
		if (i == rposTo.GetMeasure()) // last measure
		{
			iTo = rposTo.GetTick();
		}

		((CMeasure*)(m_MeasureList[ i ]))->GetTicks( pTicks, iIndex, iFrom, iTo );
	}
}

void CVoice::PutTicks(	CTick* pTicks, 
						const CPosition& rposFrom, 
						const CPosition& rposTo	)
{
	// put ticks iiTo consecutive measures

	int iIndex = 0; // will be incremented by CMeasure::PutTicks()
	for (int i = rposFrom.GetMeasure(); i <= rposTo.GetMeasure(); i++)
	{
		int iFrom = 0;
		int iTo = m_iTicksPerMeasure - 1;

		if (i == rposFrom.GetMeasure()) // first measure
		{
			iFrom = rposFrom.GetTick();
		}
		
		if (i == rposTo.GetMeasure()) // last measure
		{
			iTo = rposTo.GetTick();
		}

		((CMeasure*)(m_MeasureList[ i ]))->PutTicks( pTicks, iIndex, iFrom, iTo );
	}
}

void CVoice::TransposeNotes(	int iHalfSteps, 
								ENoteDirection Direction, 
								const CPosition& rposFrom, 
								const CPosition& rposTo	)
{
	for (int i = rposFrom.GetMeasure(); i <= rposTo.GetMeasure(); i++)
	{
		int iFrom = 0;
		int iTo = m_iTicksPerMeasure - 1;

		if (i == rposFrom.GetMeasure()) // first measure
		{
			iFrom = rposFrom.GetTick();
		}
		
		if (i == rposTo.GetMeasure()) // last measure
		{
			iTo = rposTo.GetTick() - 1;
		}

		((CMeasure*)(m_MeasureList[ i ]))->TransposeNotes( iHalfSteps, Direction, iFrom, iTo );
	}
}

void CVoice::TransposeDiatonic(	int iPitches, 
								ENoteDirection Direction, 
								const CPosition& rposFrom, 
								const CPosition& rposTo	)
{
	for (int i = rposFrom.GetMeasure(); i <= rposTo.GetMeasure(); i++)
	{
		int iFrom = 0;
		int iTo = m_iTicksPerMeasure - 1;

		if (i == rposFrom.GetMeasure()) // first measure
		{
			iFrom = rposFrom.GetTick();
		}
		
		if (i == rposTo.GetMeasure()) // last measure
		{
			iTo = rposTo.GetTick() - 1;
		}

		((CMeasure*)(m_MeasureList[ i ]))->TransposeDiatonic( iPitches, Direction, iFrom, iTo );
	}
}

BOOL CVoice::SetTicksPerMeasure( int iTicksPerMeasure )
{
	ClearNotes();

	for (int i = 0; i < m_MeasureList.GetSize(); i++)
	{
		((CMeasure*)(m_MeasureList[ i ]))->SetTicksPerMeasure( iTicksPerMeasure );
	}

	m_iTicksPerMeasure = iTicksPerMeasure;

	return TRUE;
}

void CVoice::RemoveAttacks( int iMeasure )
{
	// iMeasure = -1 : all measures

	if (iMeasure == -1)
	{
		for (int i = 0; i < m_MeasureList.GetSize(); i++)
		{
			((CMeasure*)(m_MeasureList[ i ]))->RemoveAttacks();
		}
	}
	else
	{		
		((CMeasure*)(m_MeasureList[ iMeasure ]))->RemoveAttacks();
	}
}

void CVoice::SetColor( COLORREF crColor )
{
	m_crColor = crColor;
	m_hPen = ::CreatePen( PS_SOLID, 1, m_crColor );
}

void CVoice::SetWaveFileName( LPSTR lpstrFile )
{
	lstrcpy( m_szWaveFile, lpstrFile );
}

void CVoice::GetWaveFileName( LPSTR lpstrFile )
{
	lstrcpy( lpstrFile, m_szWaveFile );
}

BOOL CVoice::RecordPrep( char szFilePath[], WAVEFORMATEX& wf, int iMaxTime )
{
/*
	SetWaveFileName( szFilePath );

	m_pWave = new CWave(	szFilePath,
							wf.wFormatTag,
							wf.nChannels,
							wf.nSamplesPerSec,
							wf.wBitsPerSample	);

	return m_pWave->RecordPrep( iMaxTime );
*/
	return TRUE;
}

BOOL CVoice::RecordStart()
{
/*
	if (m_pWave == NULL) return FALSE;
	return m_pWave->RecordStart();
*/
	return TRUE;
}

BOOL CVoice::RecordStop()
{
//	if (m_pWave == NULL) return FALSE;
//	m_pWave->RecordStop();

//	delete m_pWave;
//	m_pWave = NULL;

	return TRUE;
}

