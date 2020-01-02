
// Filename : cursor.cpp
// Description : implementation of the CCursor class
// Author : Marc P. Bernasconi

#include "stdafx.h"
#include "math.h"
#include "strstrea.h"

#ifndef __CCURSOR_H__
#include "CCursor.h"	
#endif
#ifndef __CONSTANT_H__	
#include "Constant.h"	
#endif
#ifndef __CTICK_H__
#include "CTick.h"
#endif
#ifndef __CSTAFF_H__		
#include "CStaff.h"		
#endif
#ifndef __CNOTE_H__		
#include "CNote.h"		
#endif
#ifndef __CSONG_H__		
#include "CSong.h"		
#endif
#ifndef __CSELECTION_H__	
#include "CSelection.h"	
#endif
#ifndef __CPOSITION_H__	
#include "CPosition.h"	
#endif
#ifndef __CAPPLICATION_H__	
#include "CApplication.h"	
#endif
#ifndef __CCHORD_H__
#include "CChord.h"	
#endif

#include "CDialogCursorSettings.h"
#include "CDialogChannelSettings.h"
#include "CDialogMacro.h"
#include "CDialogDrumPart.h"
#include "CDialogClickTrack.h"
#include "CDialogGuitarChord.h"
#include "CDialogChordProgression.h"
#include "CDialogMelody.h"
#include "CDialogHarmonize.h"
#include "CDialogTransposeNotes.h"
#include "CDialogTransposeDiatonic.h"
#include "CDialogPercussion.h"


CChord GuitarChord_CM(		C0,		G0,		C1,		E1,		G1	);
CChord GuitarChord_CM7(		C0,		G0,		B0,		E1,		G1	);
CChord GuitarChord_Cm(		C0,		G0,		C1,		Eb1,	G1	);
CChord GuitarChord_Cm7(		C0,		G0,		Bb0,	Eb1,	G1	);
CChord GuitarChord_Cm9(		C0,		Eb0,	Bb0,	D1,		G1	);
CChord GuitarChord_Cm11(	C0,		F0,		Bb0,	Eb1,	G1	);
CChord GuitarChord_C7(		C0,		G0,		Bb0,	E1,		G1	);
CChord GuitarChord_Cb9(		C0,		E0,		Bb0,	Db1,	G1	);
CChord GuitarChord_C9(		C0,		E0,		Bb0,	D1,		G1	);
CChord GuitarChord_C13(		C0,		Bb0,	E1,		A1	);
CChord GuitarChord_C13b9(	C0,		Bb0,	Db1,	A1	);
CChord GuitarChord_Cm7b5(	C0,		Gb0,	Bb0,	Eb1			);
CChord GuitarChord_Cdim7(	Eb0,	A0,		C1,		Gb1			);
CChord GuitarChord_C6(		C0,		G0,		C1,		E1,		A1	);
CChord GuitarChord_Cm6(		C0,		G0,		Eb1,	A1	);
CChord GuitarChord_Caug(	C0,		E0,		Gs0,	C1			);
CChord GuitarChord_Caug7(	Bb0,	E0,		Gs0,	C1			);
CChord GuitarChord_Csus(	C0,		G0,		C1,		F1,		G1	);
CChord GuitarChord_Csus7(	C0,		G0,		Bb0,	F1,		G1	);

CChord GuitarChord_FM(		F0,		C1,		F1,		A1,		C2,		F2	);
CChord GuitarChord_FM7(		F0,		E1,		A1,		C2					);
CChord GuitarChord_Fm(		F0,		C1,		F1,		Ab1,	C2,		F2	);
CChord GuitarChord_Fm7(		F0,		C1,		Eb1,	Ab1,	C2,		F2	);
CChord GuitarChord_Fm9(		F0,		C1,		Eb1,	Ab1,	C2,		F2	);
CChord GuitarChord_Fm11(	F0,		Bb0,	Eb1,	Ab1,	C2,		F2	);
CChord GuitarChord_F7(		F0,		C1,		Eb1,	A1,		C2,		F2	);
CChord GuitarChord_Fb9(		F0,		C1,		Eb1,	A1,		C2,		Gb2	);
CChord GuitarChord_F9(		F0,		C1,		Eb1,	A1,		C2,		G2	);
CChord GuitarChord_F13(		F0,		C1,		Eb1,	A1,		D2,		G2	);
CChord GuitarChord_F13b9(	F0,		Eb1,	A1,		D2,		Gb2	);
CChord GuitarChord_Fm7b5(	F0,		B0,		Eb1,	Ab1					);
CChord GuitarChord_Fdim7(	F0,		B0,		D1,		Ab1					);
CChord GuitarChord_F6(		F0,		C1,		A1,		D2,		F2			);
CChord GuitarChord_Fm6(		F0,		C1,		Ab1,	D2,		F2			);
CChord GuitarChord_Faug(	F0,		A0,		Cs1,	F1					);
CChord GuitarChord_Faug7(	Eb0,	A0,		Cs1,	F1					);
CChord GuitarChord_Fsus(	F0,		Bb0,	F1,		Bb1,	C2,		F2	);
CChord GuitarChord_Fsus7(	F0,		Bb0,	Eb1,	Bb1,	C2,		F2	);



#define MEASURE_WIDTH		(NOTE_WIDTH * (m_iTicksPerMeasure + 2))


CCursor::CCursor() :
	m_pNote( NULL ),
	m_iMeasure( 0 ),
	m_iVoice( 0 ),
	m_iTick( 0 ),
	m_iAbsoluteTick( 0 ),
	m_iVoicesInChord( 3 ),
	m_iMove( NOTE_QUARTER ),
	m_iMoveCtl( NOTE_EIGHTH ),
	m_iTicksPerMeasure( NOTE_QUARTER * SONG_TIMESIG_N ),
	m_blChord( FALSE ),
	m_blRememberAttack( TRUE ),
	m_blInsertMode( FALSE ),
	m_blMakeOnAddNote( TRUE ),
	m_blRetainPitch( FALSE ),
	m_blAutoAdvance( FALSE ),
	m_iXpos( CURSOR_STARTXPOS ),
	m_iYpos( CURSOR_STARTYPOS ),
	m_iXOrigin( 0 ),
	m_iYOrigin( 0 ),
	m_iPreviousGuitarChordTicks( 8 ),
	m_iPreviousGuitarChord( -1 ),
	m_blPreviousGuitarChordChord( TRUE ),
	m_blPreviousGuitarChordBassNote( TRUE ),
	m_blPreviousGuitarChordMelody( TRUE ),
	m_iConsecutiveGuitarChords( 0 ),
	m_hClipboardMem( NULL )
{
	lstrcpy( m_szPreviousMacro, "" );

	for (int i = 0; i < CHANNEL_MAX; i++)
	{
		m_iPan[ i ] = 0x40;
	}

	int iDummy1, iDummy2;
	m_pNote = new CNote( 72, CMAJOR, NOTE_QUARTER, CLEF_TREBLE, 0, 0, iDummy1, iDummy2 );
}

CCursor::~CCursor()
{
	delete m_pNote;

	if (m_hClipboardMem)
	{
		::GlobalFree( m_hClipboardMem );
	}
}

int CCursor::GetXpos() const 
{ 
	return m_iXpos / CApplication::GetZoomDivisor(); 
}

int CCursor::GetYpos() const 
{ 
	return m_iYpos / CApplication::GetZoomDivisor(); 
}

int CCursor::GetCaretBitmapId() const
{
	return m_pNote->GetCaretBitmapId();
}

int CCursor::GetKey() const
{
	return m_pNote->GetKey();
}

void CCursor::GetMode( char* szMode ) const
{
	if (m_blInsertMode)
	{
		lstrcpy( szMode, "Insert" );
	}
	else
	{
		lstrcpy( szMode, "Overwrite" );
	}
}

void CCursor::SetXOrigin( int iXOrigin )
{
	m_iXOrigin = iXOrigin;
}

void CCursor::SetYOrigin( int iYOrigin )
{
	m_iYOrigin = iYOrigin;
}

void CCursor::SetTicksPerMeasure( int iTicksPerMeasure )
{
	m_iTicksPerMeasure = iTicksPerMeasure;
}

void CCursor::SetKey( int iKey )
{
	m_pNote->SetKey( iKey );
}

void CCursor::Reset()
{
	MoveToVoice( 0 );
	Move( MOVE_HOMELONG, FALSE, NULL );
	ResetPitch();
}

void CCursor::ResetPitch()
{
	int iVoiceTop = TheSong.GetVoiceTop( m_iVoice );

	switch (TheSong.GetClef( m_iVoice ))
	{
		case CLEF_TREBLE :
			m_pNote->SetAbsolutePitch( 72 );
			m_iYpos = ((iVoiceTop + STAFF_SPACEWIDTH + CURSOR_YOFFSET) - CURSOR_SPACEABOVE);
			break;
		
		case CLEF_BASS :
			m_pNote->SetAbsolutePitch( 48 );
			m_iYpos = ((iVoiceTop + (STAFF_SPACEWIDTH * 2) + CURSOR_YOFFSET) - CURSOR_SPACEABOVE);
			break;
	}

	char szMacro[ 20 ];

	switch (m_pNote->GetKey())
	{
		case CMAJOR :
			lstrcpy( szMacro, "" );
			break;

		case GMAJOR :
			lstrcpy( szMacro, "d3 " );
			break;

		case DMAJOR :
			lstrcpy( szMacro, "u1 " );
			break;

		case AMAJOR :
			lstrcpy( szMacro, "d2 " );
			break;

		case EMAJOR :
			lstrcpy( szMacro, "u2 " );
			break;

		case BMAJOR :
			lstrcpy( szMacro, "d1 " );
			break;

	}

	Macro( szMacro );
}

int CCursor::Move( int iDirection, BOOL blShift, HDC hdc )
{
	// returns UPDATE_VIEW if view needs update

	int iReturn = UPDATE_NONE;

	int iOldXpos = m_iXpos;
	int iOldYpos = m_iYpos;
	int iOldTick = m_iTick;
	int iOldMeasure = m_iMeasure;
	int iOldVoice = m_iVoice;

	int iTicksPerMeasure = TheSong.GetTicksPerMeasure();

	BOOL blFailed = FALSE;

	int i;

	// enable selection if shift key is down
	if (blShift)
	{
		if (!m_Selection.IsEnabled())
		{
			int iVoiceTop = (TheSong.GetRect( m_iVoice, 0 )).top + m_iYOrigin;
			int iVoiceBottom = (TheSong.GetRect( m_iVoice, 0 )).bottom + m_iYOrigin;

			m_Selection.Enable(	m_iVoice, 
								m_iMeasure, 
								m_iTick, 
								m_iXpos, 
								iVoiceTop, 
								iVoiceBottom,
								m_iTicksPerMeasure	);
		}
	}
	else // disable selection
	{
		if (m_Selection.IsEnabled())
		{
			if (hdc != NULL) m_Selection.Draw( hdc );
	
			m_Selection.Disable();
		}
	}

	BOOL blHorzMove = FALSE;
	BOOL blVertMove = FALSE;

	// change position
	switch (iDirection)
	{
		case MOVE_NONE :
			break;

		case MOVE_RIGHT :
			m_iXpos += NOTE_WIDTH * m_iMove;
			m_iTick += m_iMove;
			blHorzMove = TRUE;
			break;

		case MOVE_RIGHTCTL :
			m_iXpos += NOTE_WIDTH * m_iMoveCtl;
			m_iTick += m_iMoveCtl;
			blHorzMove = TRUE;
			break;

		case MOVE_LEFT :
			m_iXpos -= NOTE_WIDTH * m_iMove;
			m_iTick -= m_iMove;
			blHorzMove = TRUE;
			break;

		case MOVE_LEFTCTL :
			m_iXpos -= NOTE_WIDTH * m_iMoveCtl;
			m_iTick -= m_iMoveCtl;
			blHorzMove = TRUE;
			break;

		case MOVE_UPSHORT :
			if (m_pNote->ChangePitch( NOTE_UP )) m_iYpos -= STAFF_SPACEWIDTH / 2;
			break;

		case MOVE_UPLONG :
			if (m_pNote->ChangePitch( NOTE_UPOCTAVE )) m_iYpos -= STAFF_SPACEWIDTH / 2 * 7;
			break;

		case MOVE_DOWNSHORT :
			if (m_pNote->ChangePitch( NOTE_DOWN )) m_iYpos += STAFF_SPACEWIDTH / 2;
			break;

		case MOVE_DOWNLONG :
			if (m_pNote->ChangePitch( NOTE_DOWNOCTAVE )) m_iYpos += STAFF_SPACEWIDTH / 2 * 7;
			break;

		case MOVE_HOMESHORT :
			m_iXpos -= NOTE_WIDTH * m_iTick;
			m_iTick -= m_iTick;
			blHorzMove = TRUE;
			break;
			
		case MOVE_HOMELONG :
			m_iXpos = CURSOR_STARTXPOS;
			m_iTick = 0;
			m_iMeasure = 0;
			blHorzMove = TRUE;
			break;
			
		case MOVE_ENDSHORT :
			m_iXpos += NOTE_WIDTH * ((iTicksPerMeasure - 1) - m_iTick);
			m_iTick += (iTicksPerMeasure - 1) - m_iTick;
			blHorzMove = TRUE;
			break;
			
		case MOVE_ENDLONG :
			m_iTick = iTicksPerMeasure - 1;
			m_iMeasure = TheSong.GetMeasureCount() - 1;
			m_iXpos = (MEASURE_FIRSTX + MEASURE_WIDTH * (TheSong.GetMeasureCount() - 1) +
				NOTE_WIDTH + (NOTE_WIDTH * m_iTick) + CURSOR_XOFFSET);
			blHorzMove = TRUE;
			break;

		case MOVE_VOICEUPSHORT :
			m_iVoice--;
			if (m_iVoice < 0)
			{
				blFailed = TRUE;
				break;
			}
			MoveToDefaultPitch( TheSong.GetClef( m_iVoice ) );
			//HiliteVoice( hdc, m_iVoice, iOldVoice );
			blVertMove = TRUE;
			break;

		case MOVE_VOICEUPLONG :
			for (i = 0; i < TheSong.GetNumVoices(); i++) 
				Move( MOVE_VOICEUPSHORT, blShift, NULL );
			blVertMove = TRUE;
			break;

		case MOVE_VOICEDOWNSHORT :
			m_iVoice++;
			if (m_iVoice >= TheSong.GetNumVoices())
			{
				blFailed = TRUE;
				break;
			}
			MoveToDefaultPitch( TheSong.GetClef( m_iVoice ) );
			//HiliteVoice( hdc, m_iVoice, iOldVoice );
			blVertMove = TRUE;
			break;

		case MOVE_VOICEDOWNLONG :
			for (i = 0; i < TheSong.GetNumVoices(); i++) 
				Move( MOVE_VOICEDOWNSHORT, blShift, NULL );
			blVertMove = TRUE;
			break;
	}

	// move to next measure if necessary
	if (m_iTick >= iTicksPerMeasure)
	{
		m_iTick -= iTicksPerMeasure;
		m_iMeasure++;
		m_iXpos += (NOTE_WIDTH * 2);

		// append measure if necessary
		if (m_iMeasure >= TheSong.GetMeasureCount())
		{
			AddMeasure();
			iReturn |= UPDATE_VIEW;
		}
	}

	// move to previous measure if necessary
	if (m_iTick < 0)
	{
		m_iTick += iTicksPerMeasure;
		m_iMeasure--;
		m_iXpos -= (NOTE_WIDTH * 2);

		// can't go beyond first measure
		if (m_iMeasure < 0)
		{
			blFailed = TRUE;
		}
	}

	if (blFailed)
	{
		// restore previous values
		m_iXpos = iOldXpos;
		m_iYpos = iOldYpos;
		m_iTick = iOldTick;
		m_iMeasure = iOldMeasure;
		m_iVoice = iOldVoice;
	}
	else
	{
		// set absolute tick
		m_iAbsoluteTick = m_iMeasure * m_iTicksPerMeasure + m_iTick;
	}

	// set extents of selection and draw it
	if (m_Selection.IsEnabled())
	{
		int iVoiceBottom = (TheSong.GetRect( m_iVoice, 0 )).bottom;

		m_Selection.SetX( m_iVoice, m_iMeasure, m_iTick, m_iXpos, m_iTicksPerMeasure );
		m_Selection.SetY( m_iVoice, m_iMeasure, m_iTick, iVoiceBottom, m_iTicksPerMeasure );

		if (hdc != NULL)
		{
			if (blHorzMove) m_Selection.DrawNowX( hdc );
			else if (blVertMove) m_Selection.DrawNowY( hdc );
		}
	}

	return iReturn;
}

void CCursor::MoveToDefaultPitch( int iClef )
{
// moves cursor to correct position when voice has changed

	int iVoiceTop = TheSong.GetVoiceTop( m_iVoice );
	int iFactor;
	int iAdd;
	int iOctave;
	char szRelativePitch[ 5 ];

	if (m_blRetainPitch)
	{
		m_pNote->GetRelativePitch( szRelativePitch );
		iOctave = m_pNote->GetOctave();
	}
	else
	{
		CNote::KeyToString( m_pNote->GetKey(), szRelativePitch );
		switch (iClef)
		{
			case CLEF_TREBLE : 
				switch (szRelativePitch[ 0 ])
				{
					case 'C' : iOctave = 6; MoveToPitch( 72 ); break;
					case 'G' : iOctave = 5; MoveToPitch( 67 ); break;
					case 'D' : iOctave = 6; MoveToPitch( 74 ); break;
					case 'A' : iOctave = 5; MoveToPitch( 69 ); break;
					case 'E' : iOctave = 6; MoveToPitch( 76 ); break;
					case 'B' : iOctave = 5; MoveToPitch( 71 ); break;
				}
				break;

			case CLEF_BASS :
				switch (szRelativePitch[ 0 ])
				{
					case 'C' : iOctave = 4; MoveToPitch( 48 ); break;
					case 'G' : iOctave = 4; MoveToPitch( 55 ); break;
					case 'D' : iOctave = 4; MoveToPitch( 50 ); break;
					case 'A' : iOctave = 4; MoveToPitch( 57 ); break;
					case 'E' : iOctave = 4; MoveToPitch( 52 ); break;
					case 'B' : iOctave = 3; MoveToPitch( 47 ); break;
				}
				break;
		}
	}

	switch (iClef)
	{
		case CLEF_TREBLE :

			switch (szRelativePitch[ 0 ])
			{
				case 'C' : iAdd = 26; break;
				case 'D' : iAdd = 27; break;
				case 'E' : iAdd = 28; break;
				case 'F' : iAdd = 29; break;
				case 'G' : iAdd = 30; break;
				case 'A' : iAdd = 31; break;
				case 'B' : iAdd = 32; break;
			}

			break;
		
		case CLEF_BASS :

			switch (szRelativePitch[ 0 ])
			{
				case 'C' : iAdd = 38; break;
				case 'D' : iAdd = 39; break;
				case 'E' : iAdd = 40; break;
				case 'F' : iAdd = 41; break;
				case 'G' : iAdd = 42; break;
				case 'A' : iAdd = 43; break;
				case 'B' : iAdd = 44; break;
			}

			break;
	}

	// 10 = number of octaves
	// 7 = number of notes in octave
	iFactor = (10 - iOctave) * 7 - iAdd;
	m_iYpos = ((iVoiceTop + (STAFF_SPACEWIDTH / 2 * iFactor) + CURSOR_YOFFSET) - CURSOR_SPACEABOVE);
}

void CCursor::MoveToMeasure( int iMeasure )
{
	while (iMeasure >= TheSong.GetMeasureCount())
	{
		AddMeasure();
	}

	int iOldAbsoluteTick = m_iAbsoluteTick;
	int iOldMeasure = m_iMeasure;

	m_iMeasure = iMeasure;

	m_iTick = 0;
	m_iAbsoluteTick = m_iMeasure * m_iTicksPerMeasure;

	int iTicksMoved = m_iAbsoluteTick - iOldAbsoluteTick;
	int iMeasuresMoved = m_iMeasure - iOldMeasure;

	m_iXpos += NOTE_WIDTH * iTicksMoved;
	m_iXpos += NOTE_WIDTH * 2 * iMeasuresMoved;
}

void CCursor::MoveToAbsoluteTick( int iAbsoluteTick )
{
	int iOldAbsoluteTick = m_iAbsoluteTick;
	int iOldMeasure = m_iMeasure;

	m_iAbsoluteTick = iAbsoluteTick;

	m_iTick = m_iAbsoluteTick % m_iTicksPerMeasure;
	m_iMeasure = m_iAbsoluteTick / m_iTicksPerMeasure;

	int iTicksMoved = m_iAbsoluteTick - iOldAbsoluteTick;
	int iMeasuresMoved = m_iMeasure - iOldMeasure;

	m_iXpos += NOTE_WIDTH * iTicksMoved;
	m_iXpos += NOTE_WIDTH * 2 * iMeasuresMoved;
}

void CCursor::MoveToVoice( int iVoice )
{
	ASSERT( iVoice >= 0 && iVoice < TheSong.GetNumVoices());

	while (m_iVoice > iVoice) Move( MOVE_VOICEUPSHORT, FALSE, NULL );
	while (m_iVoice < iVoice) Move( MOVE_VOICEDOWNSHORT, FALSE, NULL );
}

void CCursor::MoveToPitch( int iAbsolutePitch )
{
	// this works because m_pNote is always diatonic :
	//  better make sure iAbsolutePitch is too

	while (m_pNote->GetAbsolutePitch() < iAbsolutePitch)
	{
		m_pNote->ChangePitch( NOTE_UP );
		m_iYpos -= STAFF_SPACEWIDTH / 2;
	}

	while (m_pNote->GetAbsolutePitch() > iAbsolutePitch)
	{
		m_pNote->ChangePitch( NOTE_DOWN );
		m_iYpos += STAFF_SPACEWIDTH / 2;
	}
}

void CCursor::AddMeasure()
{

	TheSong.AddMeasure( m_pNote->GetKey() );
}

int CCursor::AddNote( BOOL blRest, BOOL blSharp )
{
	// returns UPDATE_VIEW if view needs update
	int iReturn = FALSE;

	int iUpdate = TheSong.AddNote(	m_pNote, 
									m_iTick, 
									m_iVoice, 
									m_iMeasure, 
									blRest, 
									blSharp, 
									m_iYpos,
									m_blRememberAttack, 
									m_blMakeOnAddNote	);

	if (m_blAutoAdvance)
	{
		Move( MOVE_RIGHT, FALSE, NULL );
	}
	
	if (iUpdate & UPDATE_SONG)
	{
		iReturn |= UPDATE_VIEW;
	}
	else if (iUpdate & UPDATE_MEASURE)
	{
		iReturn |= UPDATE_VIEW;
	}

	return iReturn;
}

int CCursor::AddNote2( int iAbsolutePitch, int iTicks )
{
	// returns UPDATE_VIEW if view needs update
	int iReturn = FALSE;

	BOOL blSharp = FALSE;
	CNote* pNote = new CNote;
	*pNote = *m_pNote;
	pNote->SetAbsolutePitch( iAbsolutePitch );
	pNote->SetValue( iTicks );

	int iUpdate = TheSong.AddNote(	pNote, 
									m_iTick, 
									m_iVoice, 
									m_iMeasure, 
									FALSE, 
									blSharp, 
									m_iYpos,
									m_blRememberAttack, 
									m_blMakeOnAddNote	);

	delete pNote;

	if (m_blAutoAdvance)
	{
		Move( MOVE_RIGHT, FALSE, NULL );
	}
	
	if (iUpdate & UPDATE_SONG)
	{
		iReturn |= UPDATE_VIEW;
	}
	else if (iUpdate & UPDATE_MEASURE)
	{
		iReturn |= UPDATE_VIEW;
	}

	return iReturn;
}

int CCursor::AddChord( BOOL blRest, BOOL blSharp, int iMode )
{
// TODO: doesn't support sharps

	// returns UPDATE_VIEW if view needs update (ie. always)
	// iMode must be MODE_DIATONIC

	ASSERT( iMode == MODE_DIATONIC );

	char szMacro[ 256 ];
	sprintf( szMacro, "v%d ", m_pNote->GetValue() );

	int i;
	if (blRest)
	{
		lstrcat( szMacro, "t " );

		for (i = 1; i < m_iVoicesInChord; i++)
		{
			lstrcat( szMacro, "U1 u2 t " );
		}
		for (i = 1; i < m_iVoicesInChord; i++)
		{
			lstrcat( szMacro, "d2 D1 " );
		}
	}
	else
	{
		lstrcat( szMacro, "n " );

		for (i = 1; i < m_iVoicesInChord; i++)
		{
			lstrcat( szMacro, "U1 u2 n " );
		}
		for (i = 1; i < m_iVoicesInChord; i++)
		{
			lstrcat( szMacro, "d2 D1 " );
		}
	}

	BOOL blOldRetainPitch = m_blRetainPitch;
	m_blRetainPitch = TRUE;
	BOOL blOldAutoAdvance = m_blAutoAdvance;
	m_blAutoAdvance = FALSE;
	Macro( szMacro );
	m_blRetainPitch = blOldRetainPitch;
	m_blAutoAdvance = blOldAutoAdvance;

	if (m_blAutoAdvance)
	{
		Move( MOVE_RIGHT, FALSE, NULL );
	}

	return UPDATE_VIEW;
}

int CCursor::OnKeyDown( unsigned int uiChar, HDC hdc ) 
{
	// returns TRUE if view needs to update

	int iReturn = UPDATE_SCROLL;

	BOOL blShift = FALSE;
	if (::GetKeyState( VK_SHIFT ) & 0x80) blShift = TRUE;

	switch (uiChar)
	{
		int iDirection;

		case VK_RIGHT :
			iReturn |= Move( MOVE_RIGHT, blShift, hdc );
			break;

		case 222 :	// '''
			iReturn |= Move( MOVE_RIGHTCTL, blShift, hdc );
			break;

		case VK_LEFT :
			iReturn |= Move( MOVE_LEFT, blShift, hdc );
			break;

		case 76 :	// 'l'
			iReturn |= Move( MOVE_LEFTCTL, blShift, hdc );
			break;

		case 80 :	// 'p'
		case VK_UP :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_UPLONG;
			else iDirection = MOVE_UPSHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;

		case 186 :	// ';'
		case VK_DOWN :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_DOWNLONG;
			else iDirection = MOVE_DOWNSHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;

		case VK_HOME :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_HOMELONG;
			else iDirection = MOVE_HOMESHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;

		case VK_END :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_ENDLONG;
			else iDirection = MOVE_ENDSHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;

		case VK_PRIOR :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_VOICEUPLONG;
			else iDirection = MOVE_VOICEUPSHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;

		case VK_NEXT :
			if (::GetKeyState( VK_CONTROL ) & 0x80) iDirection = MOVE_VOICEDOWNLONG;
			else iDirection = MOVE_VOICEDOWNSHORT;
			iReturn |= Move( iDirection, blShift, hdc );
			break;


		case 32 :	//space
			
			// disable selection
			Move( MOVE_NONE, FALSE, hdc );
			
			if (m_blInsertMode)
			{
				InsertionModeCopy();
			
				CPosition pos;
				pos.Initialize(	m_iVoice, 
								m_iAbsoluteTick + m_pNote->GetValue(), 
								m_iTicksPerMeasure		);

				InsertionModePaste( pos );
			}

			if (m_blChord)
			{
				iReturn |= AddChord( FALSE, FALSE, MODE_DIATONIC );
			}
			else iReturn |= AddNote( FALSE, FALSE );
			break;

		case '3' :

			// disable selection
			Move( MOVE_NONE, FALSE, hdc );
			
			if (m_blInsertMode)
			{
				InsertionModeCopy();
			
				CPosition pos;
				pos.Initialize(	m_iVoice, 
								m_iAbsoluteTick + m_pNote->GetValue(), 
								m_iTicksPerMeasure		);

				InsertionModePaste( pos );
			}

			if (m_blChord)
			{
				iReturn |= AddChord( FALSE, TRUE, MODE_DIATONIC );
			}
			else iReturn |= AddNote( FALSE, TRUE );
			break;

/*		case 'J' :
			if (m_blChord)
			{
				if (m_iVoice == 2) iReturn |= AddChord( FALSE, MODE_MAJOR );
			}
			break;

		case 'N' :
			if (m_blChord)
			{
				if (m_iVoice == 2) iReturn |= AddChord( FALSE, MODE_MINOR );
			}
			break;
*/
		case 'R' :

			// disable selection
			Move( MOVE_NONE, FALSE, hdc );
			
			if (m_blInsertMode)
			{
				InsertionModeCopy();
			
				CPosition pos;
				pos.Initialize(	m_iVoice, 
								m_iAbsoluteTick + m_pNote->GetValue(), 
								m_iTicksPerMeasure		);

				InsertionModePaste( pos );
			}

			if (m_blChord)
			{
				iReturn |= AddChord( TRUE, FALSE, MODE_DIATONIC );
			}
			else iReturn |= AddNote( TRUE, FALSE );
			break;

		case VK_BACK :
		{			
			// disable selection
			Move( MOVE_NONE, FALSE, hdc );
			
			int iTickToMoveTo = m_iAbsoluteTick - m_iMove;
			
			if (iTickToMoveTo < 0)
			{
				::MessageBeep( MB_OK );
			}
			else
			{
				InsertionModeCut();
		
				CPosition pos;
				pos.Initialize( m_iVoice, 
								iTickToMoveTo, 
								m_iTicksPerMeasure		);

				InsertionModePaste( pos );

				Move( MOVE_LEFT, FALSE, hdc );
			}

			iReturn |= UPDATE_VIEW;

			break;
		}

		case VK_DELETE :
		{
			// disable selection
			Move( MOVE_NONE, FALSE, hdc );
			
			Move( MOVE_RIGHT, FALSE, hdc );
			InsertionModeCut();
			Move( MOVE_LEFT, FALSE, hdc );
	
			CPosition pos;
			pos.Initialize( m_iVoice, 
							m_iAbsoluteTick, 
							m_iTicksPerMeasure		);

			InsertionModePaste( pos );

			iReturn |= UPDATE_VIEW;

			break;
		}

		default :

			iReturn = UPDATE_NONE;

			break;

	}

	return iReturn;
}

BOOL CCursor::ComposeMacro( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogMacro dm;

	lstrcpy( dm.m_szMacro, m_szPreviousMacro );
	dm.m_iRepetitions = 1;

	if (dm.DoModal( hInstance, hwndParent ) == IDOK)
	{
		Macro( dm.m_szMacro, dm.m_iRepetitions );

		lstrcpy( m_szPreviousMacro, dm.m_szMacro );

		return TRUE;
	}

	return FALSE;
}

struct SPitchFreq
{
	BYTE	bPitch;
	int		iFreq;

	SPitchFreq() { bPitch = 0; iFreq = 0; }
};

BOOL CCursor::ComposeHarmonize( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogHarmonize dh;

	dh.m_iGranularity = 1;
	dh.m_iVoice = 0;
	dh.m_iMeasures = 1;

	if (dh.DoModal( hInstance, hwndParent ) == IDOK)
	{
		Harmonize( dh.m_szGranularity, dh.m_iVoice, dh.m_iMeasures );

		return TRUE;
	}

	return FALSE;
}

void CCursor::Harmonize( const char* szGranularity, int iVoice, int iMeasures )
{
	BOOL blOldRetainPitch = m_blRetainPitch;
	m_blRetainPitch = TRUE;

	CSelection selVoice;
	selVoice.Enable( iVoice, m_iMeasure, 0, 0, 0, 0, m_iTicksPerMeasure );
	selVoice.SetX( iVoice, m_iMeasure + iMeasures, 0, 0, m_iTicksPerMeasure );
	selVoice.LoadTicks( &TheSong );

	int iGranularity;

	if (ARE_EQUAL( szGranularity, "Eighth" )) iGranularity = NOTE_EIGHTH;
	else if (ARE_EQUAL( szGranularity, "Quarter")) iGranularity = NOTE_QUARTER;
	else if (ARE_EQUAL( szGranularity, "Half")) iGranularity = NOTE_HALF;
	else if (ARE_EQUAL( szGranularity, "Whole")) iGranularity = NOTE_WHOLE;

	SPitchFreq* pPitchFreq = NULL;
	int iTick = 0;
	
	// loop through all ticks in selection
	while (iTick < selVoice.GetNumberTicks())
	{
		// loop through one grain's worth of ticks
		if (pPitchFreq) delete[] pPitchFreq;
		pPitchFreq = new SPitchFreq[ iGranularity ];
		for (int j = 0; j < iGranularity; j++, iTick++)
		{
			if (!(iTick < selVoice.GetNumberTicks())) goto BREAK;
			BOOL blFound = FALSE;
			
			// update count for this tick
			for (int k = 0; k < iGranularity; k++)
			{
				if (selVoice.m_ppTicks[ 0 ][ iTick ].GetPitch() == pPitchFreq[ k ].bPitch)
				{
					pPitchFreq[ k ].iFreq++;
					blFound = TRUE;
					break;
				}
			}
			
			if (!blFound) // start counting this tick's pitch
			{
				int l = 0;
				while (pPitchFreq[ l ].bPitch != 0)
				{							
					l++;
				}
				pPitchFreq[ l ].bPitch = selVoice.m_ppTicks[ 0 ][ iTick ].GetPitch();
				pPitchFreq[ l ].iFreq = 1;
			}
		}
	
		// find pitch with the highest count
		BYTE bDominant = 0;
		int iMaxCount = 0;
		for (j = 0; j < iGranularity; j++)
		{
			if (pPitchFreq[ j ].iFreq > iMaxCount)
			{
				iMaxCount = pPitchFreq[ j ].iFreq;
				bDominant = pPitchFreq[ j ].bPitch;
			}
		}

		bDominant = m_pNote->ChromaticToKey( bDominant % 12);

		int iChord;
		switch (bDominant)
		{
			case 0 : 
				switch (rand() % 2)
				{
					case 0 : iChord = 1; break;
					case 1 : iChord = 4; break;
//						case 2 : iChord = 6; break;
				}
				break;
			case 1 : iChord = 1; break;
			case 2 :
				switch (rand() % 5)
				{
					case 0 : iChord = 2; break;
					default : iChord = 5; break;
				}
				break;
			case 3 : iChord = 1; break;
			case 4 :
				switch (rand() % 12)
				{
					case 0 : 
					case 1 :
					case 2 :
					case 3 :
					case 4 :
					case 5 :
					case 6 : iChord = 1; break;
					case 7 :
					case 8 :
					case 9 : iChord = 3; break;
					case 10 :
					case 11 : iChord = 6; break;
				}
				break;
			case 5 :
				switch (rand() % 5)
				{
					case 0 : iChord = 2; break;
					default : iChord = 4; break;
				}
				break;
			case 6 : iChord = 1; break;
			case 7 :
				switch (rand() % 2)
				{
					case 0 : iChord = 1; break;
					case 1 : iChord = 5; break;
				}
				break;
			case 8 : iChord = 1; break;
			case 9 :
				switch (rand() % 3)
				{
					case 0 : iChord = 2; break;
					case 1 : iChord = 4; break;
					case 2 : iChord = 6; break;
				}
				break;
			case 10 : iChord = 1; break;
			case 11 : iChord = 5; break;
		}
		
		ResetPitch();

		char szMacro[ 256 ];
		sprintf( szMacro, "v%d ", iGranularity );

		switch (iChord)
		{
			case 1 : lstrcat( szMacro, "u0 n U1 u4 n U1 u5 n U1 u5 n d5 D1 d5 D1 d4 D1 r" ); break;
			case 2 : lstrcat( szMacro, "u1 n U1 u2 n U1 u9 n U1 u3 n d3 D1 d9 D1 d2 D1 r" ); break;
			case 3 : lstrcat( szMacro, "u2 n U1 u4 n U1 u5 n U1 u2 n d2 D1 d5 D1 d4 D1 r" ); break;
			case 4 : lstrcat( szMacro, "u3 n U1 u2 n U1 u5 n U1 u4 n d4 D1 d5 D1 d2 D1 r" ); break;
			case 5 : lstrcat( szMacro, "d3 n U1 u7 n U1 u4 n U1 u5 n d5 D1 d4 D1 d7 D1 r" ); break;
			case 6 : lstrcat( szMacro, "d2 n U1 u4 n U1 u10 n U1 u2 n d2 D1 d10 D1 d4 D1 r" ); break;
		}

		char szMove[ 256 ];
		sprintf( szMove, "%d ", iGranularity );
		lstrcat( szMacro, szMove );

		Macro( szMacro );
	}

BREAK:

	if (pPitchFreq) delete[] pPitchFreq;

	m_blRetainPitch = blOldRetainPitch;
}

BOOL CCursor::ComposeDrumPart( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogDrumPart ddp;

	int iNumVoices = TheSong.GetNumVoices();

	ddp.m_iKick		= iNumVoices;
	ddp.m_iSnare	= iNumVoices - 1;
	ddp.m_iHiHat	= iNumVoices - 2;
	ddp.m_iTom1		= iNumVoices - 3;
	ddp.m_iTom2		= iNumVoices - 4;

	ddp.m_iStartMeasure = 1;
	ddp.m_iMeasures = 1;
	ddp.m_iStyle = 0;

	if (ddp.DoModal( hInstance, hwndParent ) == IDOK)
	{
		DrumPart(	ddp.m_iKick - 1, 
					ddp.m_iSnare - 1, 
					ddp.m_iHiHat - 1, 
					ddp.m_iTom1 - 1, 
					ddp.m_iTom2 - 1, 
					ddp.m_iMeasures, 
					ddp.m_iStyle, 
					ddp.m_iStartMeasure	- 1	);

		return TRUE;
	}

	return FALSE;
}


#define GENERATE_TOM_PART											\
	MoveToMeasure( iStartMeasure );									\
	MoveToPitch( rand() % 60 + 27 );								\
																	\
	iTicks = 0;														\
	lstrcpy( szRandomPart, "h " );											\
	iRight = (int)pow( 2, rand() % 5 + 1 );								\
	while (iTicks < m_iTicksPerMeasure)								\
	{																\
		char szNote[ 20 ] = "n ";										\
		iRight = (int)pow( 2, rand() % 5 + 1 );							\
		iTicks += iRight;											\
																	\
		if (rand() % 4 == 0) lstrcpy( szNote, "t ");						\
																	\
		sprintf( szNote, "%s r%d v%d ", szNote, iRight, 8 );		\
		lstrcat( szRandomPart, szNote );									\
	}																\
	Macro( szRandomPart );


void CCursor::DrumPart(	int iKickVoice, 
						int iSnareVoice, 
						int iHiHatVoice, 
						int iTom1Voice, 
						int iTom2Voice, 
						int iMeasures, 
						int iStyle, 
						int iStartMeasure	)
{
	// set key temporarily to C
	int iOldKey = m_pNote->GetKey();
	m_pNote->SetKey( CMAJOR );

	// save time by not having song make itself on avery added note
	BOOL blOldMakeOnAddNote = m_blMakeOnAddNote;
	m_blMakeOnAddNote = FALSE;

	BOOL blOldAutoAdvance = m_blAutoAdvance;
	m_blAutoAdvance = FALSE;

	// set voices to bass clef
	TheSong.SetClef( iKickVoice, CLEF_BASS );
	TheSong.SetClef( iSnareVoice, CLEF_BASS );
	TheSong.SetClef( iHiHatVoice, CLEF_BASS );
	TheSong.SetClef( iTom1Voice, CLEF_BASS );
	TheSong.SetClef( iTom2Voice, CLEF_BASS );

	// set channels to 10
	TheSong.SetChannel( iKickVoice, 10 );
	TheSong.SetChannel( iSnareVoice, 10 );
	TheSong.SetChannel( iHiHatVoice, 10 );
	TheSong.SetChannel( iTom1Voice, 10 );
	TheSong.SetChannel( iTom2Voice, 10 );

	int i;
	int iTicks;
	char szRandomPart[ 1000 ];
	int iRight;

	switch (iStyle)
	{
		case DRUMSTYLE_ROCK1 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v4 n r16 n r16" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SNARE1 );
			Macro( "v4 r8 n r16 n r8" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "v2 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 u2 v4 3 r4 d2" );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

	
			break;
			
		case DRUMSTYLE_ROCK2 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v4 n r12 v2 n r4 v4 n r16" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SNARE1 );
			Macro( "v4 r8 n r16 n r8" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "v2 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 u2 v4 3 r4 d2" );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

	
			break;

		case DRUMSTYLE_TECHNO1 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v8 n r8 n r8 n r8 n r8" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SIDESTICK );
			Macro( "v1 r4 3 r2 3 r6 3 r2 3 r6 3 r2 3 r6 3 r2 3 r2" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "v1 3 r2 3 r2 v3 u2 3 r4 d2 v1 3 r2 3 r2 v3 u2 3 r4 d2 v1 3 r2 3 r2 v3 u2 3 r4 d2 v1 3 r2 3 r2 v3 u2 3 r4 d2 " );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

	
			break;

		case DRUMSTYLE_SKA1 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v8 n r8 n r8 n r8 n r8" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SIDESTICK );
			Macro( "v2 r8 3 r6 3 r4 3 r6 3 r8" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "v2 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4" );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

	
			break;

		case DRUMSTYLE_HIPHOP1 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v2 n r8 r8 n r4 n r4 r4 n r4" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SNARE1 );
			Macro( "v2 r8 n r6 n r4 n r6 n r6 n r2" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "v2 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4 3 r4" );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

			break;

		case DRUMSTYLE_SWING1 :

			// kick drum
			MoveToVoice( iKickVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_KICK1 );
			Macro( "v4 n r16 n r16" );
			
			// snare drum
			MoveToVoice( iSnareVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_SNARE1 );
			Macro( "v4 r8 n r16 n r8" );
			
			// hihat
			MoveToVoice( iHiHatVoice );
			MoveToMeasure( iStartMeasure );
			MoveToPitch( DRUM_CLOSEDHIHAT );
			Macro( "u2 v8 3 r8 d2 v4 3 r4 v4 3 r4 u2 v8 3 r8 d2 v4 3 r4 v4 3 r4 " );

			// tom1 (random)
			MoveToVoice( iTom1Voice );
			GENERATE_TOM_PART;

			// tom2 (random)
			MoveToVoice( iTom2Voice );
			GENERATE_TOM_PART;

	
			break;
			

		default :
			break;
	}

	// copy and paste
	MoveToMeasure( iStartMeasure );

	Macro( "bs r32 D4 " );
	OnEditCopy();
	Macro( "U4 " ); 
	
	for (i = 1; i < iMeasures; i++)
	{
		OnEditPaste();
		Macro( "r32 " );
	}
				
	Move( MOVE_HOMELONG, FALSE, NULL );
	MoveToVoice( 0 );
	ResetPitch();

	m_blMakeOnAddNote = blOldMakeOnAddNote;
	m_blAutoAdvance = blOldAutoAdvance;

	// set key back
	m_pNote->SetKey( iOldKey );
}

BOOL CCursor::ComposeClickTrack( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogClickTrack dct;

	int iNumVoices = TheSong.GetNumVoices();

	dct.m_iMeasures = 1;
	dct.m_iVoice = TheSong.GetNumVoices() - 3;

	if (dct.DoModal( hInstance, hwndParent ) == IDOK)
	{
		// set key temporarily to C
		int iOldKey = m_pNote->GetKey();
		m_pNote->SetKey( CMAJOR );

		// save time by not having song make itself on avery added note
		BOOL blOldMakeOnAddNote = m_blMakeOnAddNote;
		m_blMakeOnAddNote = FALSE;

		BOOL blOldAutoAdvance = m_blAutoAdvance;
		m_blAutoAdvance = FALSE;

		// set voice to bass clef
		TheSong.SetClef( dct.m_iVoice, CLEF_BASS );

		// set channel to 10
		TheSong.SetChannel( dct.m_iVoice, 10 );

		int i;
		int iTicks;
		char szRandomPart[ 1000 ];
		int iRight;

		MoveToVoice( dct.m_iVoice );
		MoveToMeasure( 0 );
		MoveToPitch( DRUM_METRONOMECLICK );
		Macro( "v4 n r8 " );
		MoveToPitch( DRUM_SQUARECLICK );
		Macro( "v4 n r8 n r8 n r8 " );
		
		// copy and paste
		MoveToMeasure( 0 );

		Macro( "bs r32 " );
		OnEditCopy();
		
		for (i = 1; i < dct.m_iMeasures; i++)
		{
			OnEditPaste();
			Macro( "r32 " );
		}
					
		Move( MOVE_HOMELONG, FALSE, NULL );
		MoveToVoice( 0 );
		ResetPitch();

		m_blMakeOnAddNote = blOldMakeOnAddNote;
		m_blAutoAdvance = blOldAutoAdvance;

		// set key back
		m_pNote->SetKey( iOldKey );

		return TRUE;
	}

	return FALSE;
}

BOOL CCursor::ComposeGuitarChord( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogGuitarChord dgc;

	dgc.m_iTicks = m_iPreviousGuitarChordTicks;
	if (m_iPreviousGuitarChord == -1)
	{
		m_iPreviousGuitarChord = 0;
	}
	dgc.m_iChord = m_iPreviousGuitarChord;
	dgc.m_blChord = m_blPreviousGuitarChordChord;
	dgc.m_blBassNote = m_blPreviousGuitarChordBassNote;
	dgc.m_blMelody = m_blPreviousGuitarChordMelody;

	if (dgc.DoModal( hInstance, hwndParent ) == IDOK)
	{
		GuitarChord( dgc.m_iTicks, dgc.m_iChord, dgc.m_blChord, dgc.m_blBassNote, dgc.m_blMelody );

		m_iPreviousGuitarChordTicks = dgc.m_iTicks;
		m_iPreviousGuitarChord = dgc.m_iChord;
		m_blPreviousGuitarChordChord = dgc.m_blChord;
		m_blPreviousGuitarChordBassNote = dgc.m_blBassNote;
		m_blPreviousGuitarChordMelody = dgc.m_blMelody;

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCursor::ComposeChordProgression( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogChordProgression dcp;
	dcp.m_blChord = m_blPreviousGuitarChordChord;
	dcp.m_blBassNote = m_blPreviousGuitarChordBassNote;
	dcp.m_blMelody = m_blPreviousGuitarChordMelody;

	if (dcp.DoModal( hInstance, hwndParent ) == IDOK)
	{
		m_iConsecutiveGuitarChords = 0;

		BOOL blOldAutoAdvance = m_blAutoAdvance;
		m_blAutoAdvance = TRUE;
		int iOldMove = m_iMove;

		MoveToMeasure( dcp.m_iStartMeasure - 1 );

		for (int i = 0; i < dcp.m_PlacedChordList.GetSize(); i++)
		{
			m_iMove = ((CPlacedChord*)dcp.m_PlacedChordList[ i ])->m_iTicks;

			GuitarChord(	m_iMove,
							((CPlacedChord*)dcp.m_PlacedChordList[ i ])->m_iChord,
							dcp.m_blChord,
							dcp.m_blBassNote,
							dcp.m_blMelody	);

		}

		m_iPreviousGuitarChord = -1;

		m_blAutoAdvance = blOldAutoAdvance;
		m_iMove = iOldMove;

		m_blPreviousGuitarChordChord= dcp.m_blChord;
		m_blPreviousGuitarChordBassNote = dcp.m_blBassNote;
		m_blPreviousGuitarChordMelody = dcp.m_blMelody;

		// reset cursor
		MoveToMeasure( 0 );
		ResetPitch();

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CCursor::GuitarChord( int iTicks, int iChord, BOOL blChord, BOOL blBassNote, BOOL blMelody )
{
	// save time by not having song make itself on every added note
	BOOL blOldMakeOnAddNote = m_blMakeOnAddNote;
	m_blMakeOnAddNote = FALSE;

	char szMacro[ 256 ] = "";
	char szRestMacro[ 256 ] = "";
	char szChordMacro[ 256 ] = "";

	sprintf( szMacro, "v%d ", iTicks );

	int iRoot = iChord / CHORD_TYPES;
	iChord %= CHORD_TYPES;
	int iTranspose;
	int iOldVoice = GetVoice();

	CChord* pChord;

	switch (iRoot)
	{
		case 0 :		// roots on A string
		case 1 :
		case 2 :
		case 3 :
		case 4 :
		case 10 :
		case 11 :
		{
			MoveToVoice( iOldVoice - 1 );
			MoveToPitch( 48 );

			switch (iChord)
			{
				case 0 :	pChord = &GuitarChord_CM; break;
				case 1 :	pChord = &GuitarChord_CM7; break;
				case 2 :	pChord = &GuitarChord_Cm; break;
				case 3 :	pChord = &GuitarChord_Cm7; break;
				case 4 :	pChord = &GuitarChord_Cm9; break;
				case 5 :	pChord = &GuitarChord_Cm11; break;
				case 6 :	pChord = &GuitarChord_C7; break;
				case 7 :	pChord = &GuitarChord_Cb9; break;
				case 8 :	pChord = &GuitarChord_C9; break;
				case 9 :	pChord = &GuitarChord_C13; break;
				case 10 :	pChord = &GuitarChord_C13b9; break;
				case 11 :	pChord = &GuitarChord_Cm7b5; break;
				case 12 :	pChord = &GuitarChord_Cdim7; MoveToPitch( 50 ); break;
				case 13 :	pChord = &GuitarChord_C6; break;
				case 14 :	pChord = &GuitarChord_Cm6; break;
				case 15 :	pChord = &GuitarChord_Caug; break;
				case 16 :	pChord = &GuitarChord_Caug7; MoveToPitch( 58 ); break;
				case 17 :	pChord = &GuitarChord_Csus; break;
				case 18 :	pChord = &GuitarChord_Csus7; break;
			}

			break;
		}

		case 5 :		// roots on E string
		case 6 :
		case 7 :
		case 8 :
		case 9 :
		{
			MoveToPitch( 41 );

			switch (iChord)
			{
				case 0 :	pChord = &GuitarChord_FM; break;
				case 1 :	pChord = &GuitarChord_FM7; break;
				case 2 :	pChord = &GuitarChord_Fm; break;
				case 3 :	pChord = &GuitarChord_Fm7; break;
				case 4 :	pChord = &GuitarChord_Fm9; break;
				case 5 :	pChord = &GuitarChord_Fm11; break;
				case 6 :	pChord = &GuitarChord_F7; break;
				case 7 :	pChord = &GuitarChord_Fb9; break;
				case 8 :	pChord = &GuitarChord_F9; break;
				case 9 :	pChord = &GuitarChord_F13; break;
				case 10 :	pChord = &GuitarChord_F13b9; break;
				case 11 :	pChord = &GuitarChord_Fm7b5; MoveToPitch( 53 ); break;
				case 12 :	pChord = &GuitarChord_Fdim7; MoveToPitch( 53 ); break;
				case 13 :	pChord = &GuitarChord_F6; break;
				case 14 :	pChord = &GuitarChord_Fm6; break;
				case 15 :	pChord = &GuitarChord_Faug;  MoveToPitch( 53 ); break;
				case 16 :	pChord = &GuitarChord_Faug7; MoveToPitch( 51 ); break;
				case 17 :	pChord = &GuitarChord_Fsus; break;
				case 18 :	pChord = &GuitarChord_Fsus7; break;
			}

			break;
		}
	}

	lstrcat( szMacro, pChord->GetMacro() );

	// execute macro
	BOOL blOldRetainPitch = m_blRetainPitch;
	m_blRetainPitch = TRUE;
	BOOL blOldAutoAdvance = m_blAutoAdvance;
	m_blAutoAdvance = FALSE;

	// first delete what's there
	sprintf( szRestMacro, "v%d t U1 t U1 t U1 t U1 t U1 t D5 ", iTicks );
	Macro( szRestMacro );

	if (blChord) Macro( szMacro );

	m_blRetainPitch = blOldRetainPitch;
	m_blAutoAdvance = blOldAutoAdvance;
	
	MoveToVoice( iOldVoice );

	// find transposition
	switch (iRoot)
	{
		case 0 : iTranspose = 0; break;
		case 1 : iTranspose = 1; break;
		case 2 : iTranspose = 2; break;
		case 3 : iTranspose = 3; break;
		case 4 : iTranspose = 4; break;
		
		case 5 : iTranspose = 0; break;
		case 6 : iTranspose = 1; break;
		case 7 : iTranspose = 2; break;
		case 8 : iTranspose = 3; break;
		case 9 : iTranspose = 4; break;

		case 10 : iTranspose = -2; break;
		case 11 : iTranspose = -1; break;
	}

	// transpose chord
	char szTransposeMacro[ 256 ] = "";
	sprintf( szTransposeMacro, "U5 bs r%d D5", iTicks );
	if (blChord) Macro( szTransposeMacro );
	m_Selection.TransposeNotes( &TheSong, iTranspose, NOTE_UP );
	sprintf( szTransposeMacro, "es l%d", iTicks );
	if (blChord) Macro( szTransposeMacro );

	// enter bass note
	if (blBassNote)
	{
		if (iRoot == m_iPreviousGuitarChord && m_iConsecutiveGuitarChords < 2)
		{
			m_iConsecutiveGuitarChords++;
		}
		else
		{
			m_iConsecutiveGuitarChords = 0;
			m_iPreviousGuitarChord = iRoot;
		}

		int iBassPitch;

		if (GetTick() == 0)
		{
			m_iConsecutiveGuitarChords = 0;
			iBassPitch = pChord->GetPitch( m_iConsecutiveGuitarChords );
		}
		else
		{
			while ((iBassPitch = pChord->GetPitch( m_iConsecutiveGuitarChords )) == -1)
			{
				m_iConsecutiveGuitarChords = 0;
			}
		}

		blOldAutoAdvance = m_blAutoAdvance;
		m_blAutoAdvance = FALSE;

		MoveToVoice( GetVoice() + 1 );
		AddNote2( iBassPitch + 36 + iTranspose, iTicks );
//		AddNote2( iRoot + 36, iTicks );
		MoveToVoice( GetVoice() - 1 );

		m_blAutoAdvance = blOldAutoAdvance;
	}

	// improv melody
	if (blMelody)
	{
		blOldAutoAdvance = m_blAutoAdvance;
		m_blAutoAdvance = TRUE;

		int iOldMove = m_iMove;
		int iOldVoice = GetVoice();
		int iOldAbsoluteTick = GetAbsoluteTick();

		for (int i = 0; i < 1; i++)
		{
			pChord->StartMelody();

			MoveToAbsoluteTick( iOldAbsoluteTick );

			MoveToVoice( i );

			int iPrevPitch = -1;
			m_iMove = 4;
			int t = 0;
			while (t < iTicks)
			{
//				if (rand() % 4 == 0)
//				{
//					m_iMove = (int)pow( 2, rand() % 2 + 2 );
//				}
				t += m_iMove;

				int iPitch = iPrevPitch;
				while (iPitch == iPrevPitch)
				{
//					iPitch = pChord->GetPitch( rand() % pChord->GetNotes() );
					iPitch = pChord->GetMelodyPitch();
					iPitch += rand() % NOTE_TOPOCTAVE * 12;
					while (iPitch < 60) { iPitch += 12; }
					while (iPitch > 72) { iPitch -= 12; }
				}
				iPrevPitch = iPitch;
				iPitch += iTranspose;
//				if (rand() % 6 == 0) iPitch = NOTE_REST;

				AddNote2( iPitch, NOTE_QUARTER);
			}
		}

		MoveToVoice( iOldVoice );
		m_blAutoAdvance = blOldAutoAdvance;
		m_iMove = iOldMove;
		MoveToAbsoluteTick( iOldAbsoluteTick );
	}

	if (m_blAutoAdvance)
	{
		Move( MOVE_RIGHT, FALSE, NULL );
	}
	
	m_blMakeOnAddNote = blOldMakeOnAddNote;

	return TRUE;
}

BOOL CCursor::ComposeMelody( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogMelody dm;

	dm.m_iMeasures = 2;

	if (dm.DoModal( hInstance, hwndParent ) == IDOK)
	{
		Melody( dm.m_iMeasures );

		// reset cursor
		MoveToMeasure( 0 );
		MoveToVoice( 0 );
		ResetPitch();

		return TRUE;
	}

	return FALSE;
}

void CCursor::Melody( int iMeasures )
{
	char szMacro[ 1000 ] = "";
	for (int i = 0; i < iMeasures; i++)
	{
		int iTicks = 0;
		char szMeasure[ 200 ] = "h ";
//		int iRight = (int)pow( 2, rand() % 3 + 1 );
		int iRight = (int)pow( 2, rand() % 2 + 2 );
		while (iTicks < m_iTicksPerMeasure)
		{
			char szNote[ 20 ] = "";
			if (rand() % 4 == 0)
			{
//				iRight = (int)pow( 2, rand() % 3 + 1 );
				iRight = (int)pow( 2, rand() % 2 + 2 );
			}
			iTicks += iRight;
			char szDirection[ 5 ] = "";
			switch (rand() % 2)
			{
				case 0 : lstrcat( szDirection, "u" ); break;
				case 1 : lstrcat( szDirection, "d" ); break;
			}
			int iUpDown = rand() % 4;
			int iValue = 8;
			
			char szType[ 5 ] = "n ";
			if (rand() % 4 == 0) lstrcpy( szType, "t " );

			sprintf( szNote, "%s r%d %s%d v%d ", szType, iRight, szDirection, iUpDown, iValue );
			lstrcat( szMeasure, szNote );
		}
		lstrcat( szMacro, szMeasure );
	}

	Macro( szMacro );
}

BOOL CCursor::TransposeNotes( HINSTANCE hInstance, HWND hwndParent )
{
	if (!m_Selection.IsEnabled()) return FALSE;

	CDialogTransposeNotes dtn;
	dtn.m_iHalfSteps = 12;
	dtn.m_iDirection = 0;

	if (dtn.DoModal( hInstance, hwndParent ) == IDOK)
	{
		ENoteDirection Direction;
		switch (dtn.m_iDirection)
		{
			case 0 : Direction = NOTE_UP; break;
			case 1 : Direction = NOTE_DOWN; break;
		}

		m_Selection.TransposeNotes( &TheSong, dtn.m_iHalfSteps, Direction );

		return TRUE;
	}

	return FALSE;
}


#define GENERATE_CHORDRESTS									\
	lstrcpy( szRestsMacro, "" );										\
	iTicks = 0;												\
	iRight = (int)pow( 2, rand() % 5 + 1 );						\
	if (iRight == 2) iRight = 0;							\
	while (iTicks < m_iTicksPerMeasure)						\
	{														\
		if (rand() % 4 == 0)								\
		{													\
			iRight = (int)pow( 2, rand() % 5 + 1 );				\
			if (iRight == 2) iRight = 0;					\
		}													\
		iTicks += iRight;									\
		iValue = 4;											\
															\
		if (iTicks < m_iTicksPerMeasure)					\
		{													\
			sprintf( szRest, "r%d v%d t ", iRight, iValue );	\
			lstrcat( szRestsMacro, szRest );						\
		}													\
		else	\
		{	\
			sprintf( szRest, "r%d h ", iRight );	\
			lstrcat( szRestsMacro, szRest );						\
		}	\
	}

void CCursor::ComposeAddRestsToChords( int iPatternLength )
{
	char szRestsMacro[ 100 ] = "";
	char szRest[ 100 ] = "";
	int iTicks;
	int iRight;
	int iValue;

	int iStartMeasure = m_iMeasure;

	switch (rand() % 2)
	{
		case 0 :	// same for all voices

			GENERATE_CHORDRESTS;
			
			MoveToVoice( 4 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			MoveToVoice( 3 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			MoveToVoice( 2 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			MoveToVoice( 1 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			break;
						
		case 1 :	// each voice different

			GENERATE_CHORDRESTS;
						
			MoveToVoice( 4 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			GENERATE_CHORDRESTS;
			
			MoveToVoice( 3 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			GENERATE_CHORDRESTS;
			
			MoveToVoice( 2 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			GENERATE_CHORDRESTS;
			
			MoveToVoice( 1 );
			MoveToMeasure( iStartMeasure );
			Macro( szRestsMacro, iPatternLength );
			
			break;
						
	}

	MoveToVoice( 0 );
	MoveToMeasure( iStartMeasure );
}

void CCursor::ComposeSection( int iRepetitions, int* piStartMeasures, BOOL blCreateMelody )
{
	int iNumVoices = TheSong.GetNumVoices();

	int iKick	= iNumVoices - 1;
	int iSnare	= iNumVoices - 2;
	int iHiHat	= iNumVoices - 3;
	int iTom1	= iNumVoices - 4;
	int iTom2	= iNumVoices - 5;

	int iStyle;

	int iGranularity;
	char szGranularity[ 20 ] = "";
	int iStartingPitch;
	char szMacro[ 1000 ] = "";

	int iPatternLength = 4;
	if (blCreateMelody)
	{
		switch (rand() % 3)
		{
			case 0 : iPatternLength = 1; break;
			case 1 : iPatternLength = 2; break;
			case 2 : iPatternLength = 4; break;
		}
	}

	// drums
	iStyle = rand() % LastDrumStyle;
	DrumPart(	iKick, 
				iSnare, 
				iHiHat, 
				iTom1, 
				iTom2, 
				iPatternLength, 
				iStyle, 
				piStartMeasures[ 0 ]	);

	// melody

	// go to a random note in voice 0 (C5 - C7)
	MoveToVoice( 0 );
	MoveToMeasure( piStartMeasures[ 0 ] );
	ResetPitch();
	iStartingPitch = rand() % 14;
	sprintf( szMacro, "d%d ", iStartingPitch );
	Macro( szMacro );

	if (blCreateMelody)
	{
		Melody( iPatternLength );
	}

	// chords
	MoveToVoice( 4 );
	MoveToMeasure( piStartMeasures[ 0 ] );

	iGranularity = rand() % 3;
	switch (iGranularity)
	{
		case 0 : lstrcpy( szGranularity, "Quarter" ); break;
		case 1 : lstrcpy( szGranularity, "Half" ); break;
		case 2 : lstrcpy( szGranularity, "Whole" ); break;
	}
	Harmonize( szGranularity, 0, iPatternLength );
	MoveToVoice( 0 );
	MoveToMeasure( piStartMeasures[ 0 ] );
	
	// add rests to chords
	ComposeAddRestsToChords( iPatternLength );

	// copy and paste
	ResetPitch();
	sprintf( szMacro, "bs r%d D9 ", iPatternLength * m_iTicksPerMeasure );
	Macro( szMacro );
	OnEditCopy();
	Macro( "U9 " ); 

	for (int i = 0; i < iRepetitions; i++)
	{
		for (int j = 0; j < 4; j += iPatternLength)
		{
			MoveToMeasure( piStartMeasures[ i ] + j );
			OnEditPaste();
		}
	}
}

void CCursor::ComposeSectionAABA( int iRepetitions, int* piStartMeasures )
{
	int iNumVoices = TheSong.GetNumVoices();

	int iGranularity;
	char szGranularity[ 20 ] = "";
	int iStartingPitch;
	char szMacro[ 1000 ] = "";

	int iPatternLength = 4;

	// melody

	// go to a random note in voice 0 (C5 - C7)
	MoveToVoice( 0 );
	MoveToMeasure( piStartMeasures[ 0 ] );
	ResetPitch();
	iStartingPitch = rand() % 14;
	sprintf( szMacro, "d%d ", iStartingPitch );
	Macro( szMacro );

	Melody( iPatternLength );

	// chords
	MoveToVoice( 4 );
	MoveToMeasure( piStartMeasures[ 0 ] );

	iGranularity = rand() % 3;
	switch (iGranularity)
	{
		case 0 : lstrcpy( szGranularity, "Quarter" ); break;
		case 1 : lstrcpy( szGranularity, "Half" ); break;
		case 2 : lstrcpy( szGranularity, "Whole" ); break;
	}
	Harmonize( szGranularity, 0, iPatternLength );
	MoveToVoice( 0 );
	MoveToMeasure( piStartMeasures[ 0 ] );
	
	// add rests to chords
	ComposeAddRestsToChords( iPatternLength );

	// copy and paste
	ResetPitch();
	sprintf( szMacro, "bs r%d D4 ", iPatternLength * m_iTicksPerMeasure );
	Macro( szMacro );
	OnEditCopy();
	Macro( "U4 " ); 

	for (int i = 0; i < iRepetitions; i++)
	{
		for (int j = 0; j < 8; j += iPatternLength)
		{
			MoveToMeasure( piStartMeasures[ i ] + j );
			OnEditPaste();
		}
	}
}

void CCursor::ComposeSetEntryPoint( int iVoice )
{
	// entry point will be either 0, 4, 8, 12

	int iEntryMeasure;

	int iRandom = rand() % 120;

	if (iRandom > 60) iEntryMeasure = 0;
	else if (iRandom > 40) iEntryMeasure = 4;
	else if (iRandom > 20) iEntryMeasure = 8;
	else if (iRandom >  0) iEntryMeasure = 12;

	MoveToMeasure( 0 );
	MoveToVoice( iVoice );

	char szMacro[ 20 ];
	sprintf( szMacro, "v%d t r%d ", m_iTicksPerMeasure, m_iTicksPerMeasure  );
	
	Macro( szMacro, iEntryMeasure );
}

void CCursor::ComposeSong( BOOL blCreateMelody )
{
	// save time by not having song make itself on every added note
	BOOL blOldMakeOnAddNote = m_blMakeOnAddNote;
	m_blMakeOnAddNote = FALSE;

	BOOL blOldInsertMode = m_blInsertMode;
	m_blInsertMode = FALSE;

	// tempo
	int iTempo = rand() % 81 + 120;	// 120 - 200
	TheSong.SetTempo( iTempo );

	// section A

	int iRepetitions = 5;
	int iStartMeasuresA[] = { 0, 4, 12, 24, 28 };

	ComposeSection( iRepetitions, iStartMeasuresA, blCreateMelody );

	// section B

	iRepetitions = 2;
	int iStartMeasuresB[] = { 8, 16 };

	ComposeSection( iRepetitions, iStartMeasuresB, blCreateMelody );


	// section C

	iRepetitions = 1;
	int iStartMeasuresC[] = { 20 };

	ComposeSection( iRepetitions, iStartMeasuresC, blCreateMelody );


	// copy bars 0-3, voices 7-9 and paste into bars 8-11 and 16-23
	Move( MOVE_HOMELONG, FALSE, NULL );
	MoveToVoice( 7 );
	Macro( "bs r128 D2 " );
	OnEditCopy();
	Macro( "U2 " );
	Macro( "r128 " ); OnEditPaste();
	Macro( "r256 " ); OnEditPaste();
	Macro( "r128 " ); OnEditPaste();


	// set entry points for each voice
	ComposeSetEntryPoint( 0 );
	ComposeSetEntryPoint( 1 );
	ComposeSetEntryPoint( 2 );
	ComposeSetEntryPoint( 3 );
	ComposeSetEntryPoint( 4 );
	ComposeSetEntryPoint( 5 );
	ComposeSetEntryPoint( 6 );
	ComposeSetEntryPoint( 7 );
	ComposeSetEntryPoint( 8 );
	ComposeSetEntryPoint( 9 );


	// repeat the whole thing
	MoveToMeasure( 0 );
	MoveToVoice( 0 );
	Macro( "bs E D9 " );
	OnEditCopy();

	MoveToMeasure( 32 );
	MoveToVoice( 0 );
	OnEditPaste();


	// cadence
	MoveToMeasure( 63 );
	MoveToVoice( 0 );
	Macro( "bs r1 D9 " );
	OnEditCopy();

	MoveToMeasure( 63 );
	MoveToVoice( 0 );
	char szMacro[ 20 ];
	sprintf ( szMacro, "r%d ", m_iTicksPerMeasure );
	Macro( szMacro );
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		OnEditPaste();
		Macro( "r1 " );
	}

	// tranpose the repetition
	MoveToMeasure( 32 );
	MoveToVoice( 0 );
	Macro( "bs E D6 " );
	int iHalfSteps = rand() % 8;
	m_Selection.TransposeNotes( &TheSong, iHalfSteps, NOTE_UP );

	TheSong.RemoveAttacks( 64 );


	// reset cursor
	MoveToMeasure( 0 );
	MoveToVoice( 0 );
	ResetPitch();
	m_blMakeOnAddNote = blOldMakeOnAddNote;
	m_blInsertMode = blOldInsertMode;
}

void CCursor::ComposeSongAABA()
{
	// save time by not having song make itself on every added note
	BOOL blOldMakeOnAddNote = m_blMakeOnAddNote;
	m_blMakeOnAddNote = FALSE;

	BOOL blOldInsertMode = m_blInsertMode;
	m_blInsertMode = FALSE;

	// tempo
	int iTempo = rand() % 81 + 120;	// 120 - 200
	TheSong.SetTempo( iTempo );

	// section A

	int iRepetitions = 3;
	int iStartMeasuresA[] = { 0, 8, 24 };

	ComposeSectionAABA( iRepetitions, iStartMeasuresA );

	// section B

	iRepetitions = 1;
	int iStartMeasuresB[] = { 16 };

	ComposeSectionAABA( iRepetitions, iStartMeasuresB );

	// cadence
	MoveToMeasure( 31 );
	MoveToVoice( 0 );
	Macro( "bs r1 D4 " );
	OnEditCopy();

	MoveToMeasure( 31 );
	MoveToVoice( 0 );
	char szMacro[ 20 ];
	sprintf ( szMacro, "r%d ", m_iTicksPerMeasure );
	Macro( szMacro );
	for (int i = 0; i < m_iTicksPerMeasure; i++)
	{
		OnEditPaste();
		Macro( "r1 " );
	}

	TheSong.RemoveAttacks( 32 );


	// reset cursor
	MoveToMeasure( 0 );
	MoveToVoice( 0 );
	ResetPitch();
	m_blMakeOnAddNote = blOldMakeOnAddNote;
	m_blInsertMode = blOldInsertMode;
}

BOOL CCursor::TransposeDiatonic( HINSTANCE hInstance, HWND hwndParent )
{
	if (!m_Selection.IsEnabled()) return FALSE;

	CDialogTransposeDiatonic dtd;
	dtd.m_iPitches = 2;
	dtd.m_iDirection = 0;

	if (dtd.DoModal( hInstance, hwndParent ) == IDOK)
	{
		ENoteDirection Direction;
		switch (dtd.m_iDirection)
		{
			case 0 : Direction = NOTE_UP; break;
			case 1 : Direction = NOTE_DOWN; break;
		}

		m_Selection.TransposeDiatonic( &TheSong, dtd.m_iPitches, Direction );

		return TRUE;
	}

	return FALSE;
}

/* 
format string for macros: 
 "n"	drop a note (spacebar)
 "3"	drop a sharp note
 "t"	drop a rest ('r')
 "r4"	move right 4 ticks
 "l6"	move left 6 ticks
 "u2"	move up 2 notes
 "d4"	move down 4 notes
 "U3"	move up 3 voices
 "D2"	move down 2 voices
 "v4"	change value to 4 ticks
 "h"	go home short
 "H"	go home long
 "e"	go end short
 "E"	go end long
 "bs"	begin selection
 "es"	end selection

 e.g. "v2 n r4 n r4 n v1 n r2 n r2 v2 n r4"
  makes 1/8 note, 1/8 rest, 1/8 note, 1/8 rest, 1/16 note, 1/16 rest, 1/16 note,
  1/16 rest, 1/8 note, 1/8 rest
*/
void CCursor::Macro( const char* szMacro, int iRepetitions )
{
	char* szBuffer = new char[ lstrlen( szMacro ) + 1 ];
	lstrcpy( szBuffer, szMacro );

	// save settings
	BOOL blChord = m_blChord;
	int iMove = m_iMove;
	int iValue = m_pNote->GetValue();

	// change settings
	m_blChord = FALSE;
	m_iMove = 1;		// one-tick movement

	BOOL blShift = FALSE;

	for (int i = 0; i < iRepetitions; i++)
	{
		int iTicks;
		int iNotes;
		int iVoices;

		char szToken[ 16 ];
		istrstream sin( szBuffer );
		sin.eatwhite();
		sin.get( szToken, sizeof( szToken ), ' ' );
		
		while (szToken[ 0 ] != '\0')
		{
			if (szToken[ 0 ] == 'n')
			{
				OnKeyDown( 32, NULL );	// simulate a spacebar to enter a note
			}
			else if (szToken[ 0 ] == '3')
			{
				OnKeyDown( '3', NULL );	// simulate a '3' to enter a sharp note
			}
			else if (szToken[ 0 ] == 't')
			{
				OnKeyDown( 'R', NULL );	// simulate an 'r' to enter a rest
			}
			else if (szToken[ 0 ] == 'r')
			{
				iTicks = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iTicks; i++) Move( MOVE_RIGHT, blShift, NULL ); // move cursor right
			}
			else if (szToken[ 0 ] == 'l')
			{
				iTicks = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iTicks; i++) Move( MOVE_LEFT, blShift, NULL ); // move cursor left
			}
			else if (szToken[ 0 ] == 'u')
			{
				iNotes = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iNotes; i++) Move( MOVE_UPSHORT, blShift, NULL ); // move cursor up
			}
			else if (szToken[ 0 ] == 'd')
			{
				iNotes = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iNotes; i++) Move( MOVE_DOWNSHORT, blShift, NULL ); // move cursor down
			}
			else if (szToken[ 0 ] == 'U')
			{
				iVoices = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iVoices; i++) Move( MOVE_VOICEUPSHORT, blShift, NULL ); // move up a voice
			}
			else if (szToken[ 0 ] == 'D')
			{
				iVoices = atoi( &(szToken[ 1 ]) );
				for (int i = 0; i < iVoices; i++) Move( MOVE_VOICEDOWNSHORT, blShift, NULL ); // move down a voice
			}
			else if (szToken[ 0 ] == 'v')
			{
				iTicks = atoi( &(szToken[ 1 ]) );
				m_pNote->SetValue( iTicks ); // change note value
			}
			else if (szToken[ 0 ] == 'h')
			{
				Move( MOVE_HOMESHORT, blShift, NULL ); // move cursor right
			}
			else if (szToken[ 0 ] == 'H')
			{
				Move( MOVE_HOMELONG, blShift, NULL ); // move cursor right
			}
			else if (szToken[ 0 ] == 'e')
			{
				if (szToken[ 1 ] == 's')
				{
					blShift = TRUE;
				}
				else
				{
					Move( MOVE_ENDSHORT, blShift, NULL ); // move cursor right
				}
			}
			else if (szToken[ 0 ] == 'E')
			{
				Move( MOVE_ENDLONG, blShift, NULL ); // move cursor right
			}
			else if (szToken[ 0 ] == 'b')
			{
				if (szToken[ 1 ] == 's')
				{
					blShift = TRUE;
				}
				else
				{
					if (MessageBox( NULL, "There was an error in the macro string. Continue?",
						"Warning",
						MB_ICONWARNING | MB_TASKMODAL | MB_YESNO ) != IDYES)
					{
						break;
					}
				}
			}
			else
			{
				if (MessageBox( NULL, "There was an error in the macro string. Continue?",
					"Warning",
					MB_ICONWARNING | MB_TASKMODAL | MB_YESNO ) != IDYES)
				{
					break;
				}
			}

			sin.eatwhite();
			sin.get( szToken, sizeof( szToken ), ' ' );
		}
	}

	// restore settings
	m_blChord = blChord;
	m_iMove = iMove;
	m_pNote->SetValue( iValue );

//	if (lpstrBuffer) /[] lpstrBuffer;
}

void CCursor::InsertionModeCut()
{
	// start temporary selection
	m_TempSelection.Enable( m_iVoice, m_iMeasure, m_iTick, m_iXpos, 0, 0, m_iTicksPerMeasure );

	// save position
	int iAbsoluteTick = m_iAbsoluteTick;

	// move to last tick, then one right, then home (creating a new measure)
	Macro( "E r1 h " );

	// end temporary selection
	m_TempSelection.SetX( m_iVoice, m_iMeasure, m_iTick, m_iXpos, m_iTicksPerMeasure );

	// load ticks into Temporary Selection object
	m_TempSelection.LoadTicks( &TheSong );

	// replace ticks with rests
	CSelection Rests;
	Rests.Enable(	m_TempSelection.GetStartPos().GetVoice(), 
					m_TempSelection.GetStartPos().GetMeasure(), 
					m_TempSelection.GetStartPos().GetTick(), 
					0, 
					0, 
					0, 
					m_iTicksPerMeasure						);

	Rests.SetX( m_iVoice, m_iMeasure, m_iTick, m_iXpos, m_iTicksPerMeasure );
	Rests.SetY( m_iVoice, m_iMeasure, m_iTick, m_iYpos, m_iTicksPerMeasure );

	Rests.Fill( NOTE_REST, m_TempSelection.GetNumberTicks() );

	Rests.PutTicks(	&TheSong,
					m_TempSelection.GetStartPos(), 
					m_iTicksPerMeasure, 
					m_pNote->GetKey()	);

	// restore position
	MoveToAbsoluteTick( iAbsoluteTick );
}

void CCursor::InsertionModeCopy()
{
// used when adding a note in Insertion Mode

	// start temporary selection
	m_TempSelection.Enable( m_iVoice, m_iMeasure, m_iTick, m_iXpos, 0, 0, m_iTicksPerMeasure );

	// save position
	int iAbsoluteTick = m_iAbsoluteTick;

	// move to last tick, then one right, then home (creating a new measure)
	Macro( "E r1 h " );

	// end temporary selection
	m_TempSelection.SetX( m_iVoice, m_iMeasure, m_iTick, m_iXpos, m_iTicksPerMeasure );

	// load ticks into Temporary Selection object
	m_TempSelection.LoadTicks( &TheSong );

	// restore position
	MoveToAbsoluteTick( iAbsoluteTick );
}

void CCursor::InsertionModePaste( CPosition& rposFrom )
{
// used when adding a note in Insertion Mode

	m_TempSelection.PutTicks( &TheSong, rposFrom, m_iTicksPerMeasure, m_pNote->GetKey() );

	// chop off the new measure we made in InsertionModeCopy() if it's empty
	TheSong.ChopEmptyMeasures();
}

void CCursor::OnEditCopy()
{
	if (!m_Selection.IsEnabled()) return;

	if (::OpenClipboard( CApplication::GetMainWindowHandle() ))
	{
		::EmptyClipboard();

		// load ticks into Selection object
		m_Selection.LoadTicks( &TheSong );

		// allocate memory for clipboard data
		int iDataSize = m_Selection.GetNumberTicks() * 
						m_Selection.GetNumberVoices() * 
						sizeof CTick + 
						sizeof m_Selection.m_iTicks + 
						sizeof m_Selection.m_iVoices;

		m_hClipboardMem = ::GlobalAlloc( GMEM_MOVEABLE | GMEM_DDESHARE, iDataSize );

		m_Selection.GetTicks( (char*)::GlobalLock( m_hClipboardMem ) );

		::GlobalUnlock( m_hClipboardMem );

		// give ticks to clipboard
		::SetClipboardData( g_uiClipboardFormat, m_hClipboardMem );

		::CloseClipboard();
	}
}

void CCursor::OnEditCut()
{
	OnEditCopy();

	// replace ticks with rests
	CSelection Rests;
	Rests.Enable(	m_Selection.GetStartPos().GetVoice(), 
					m_Selection.GetStartPos().GetMeasure(), 
					m_Selection.GetStartPos().GetTick(), 
					0, 
					0, 
					0, 
					m_iTicksPerMeasure						);

	Rests.SetX( m_iVoice, m_iMeasure, m_iTick, m_iXpos, m_iTicksPerMeasure );
	Rests.SetY( m_iVoice, m_iMeasure, m_iTick, m_iYpos, m_iTicksPerMeasure );

	Rests.Fill( NOTE_REST, m_Selection.GetNumberTicks() );

	Rests.PutTicks(	&TheSong,
					m_Selection.GetStartPos(), 
					m_iTicksPerMeasure, 
					m_pNote->GetKey()	);
}

void CCursor::OnEditPaste()
{
	if (::IsClipboardFormatAvailable( g_uiClipboardFormat ))
	{
		if (::OpenClipboard( CApplication::GetMainWindowHandle() ))
		{
			HANDLE hClipboardMem = ::GetClipboardData( g_uiClipboardFormat );

			m_Selection.SetTicks( (char*)::GlobalLock( hClipboardMem ) );
			::GlobalUnlock( hClipboardMem );

			CPosition pos;
			pos.Initialize( m_iVoice, m_iMeasure, m_iTick, m_iTicksPerMeasure );

			m_Selection.PutTicks( &TheSong, pos, m_iTicksPerMeasure, m_pNote->GetKey() );

			::CloseClipboard();
		}
	}
}

void CCursor::OnDestroyClipboard()
{
	// free memory previously allocated for clipboard data
	if (m_hClipboardMem)
	{
		::GlobalFree( m_hClipboardMem );
		m_hClipboardMem = NULL;
	}
}

BOOL CCursor::OnSettings( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogCursorSettings dcs;

	BOOL blOldRememberAttack = m_blRememberAttack;

	dcs.m_iArrow = m_iMove;
	dcs.m_iCtlArrow = m_iMoveCtl;
	dcs.m_blRememberAttack = m_blRememberAttack;
	dcs.m_blInsertMode = m_blInsertMode;
	dcs.m_blRetainPitch = m_blRetainPitch;
	dcs.m_blAutoAdvance = m_blAutoAdvance;

	if (dcs.DoModal( hInstance, hwndParent ) == IDOK)
	{
		m_iMove = dcs.m_iArrow;
		m_iMoveCtl = dcs.m_iCtlArrow;
		m_blRememberAttack = dcs.m_blRememberAttack;
		m_blInsertMode = dcs.m_blInsertMode;
		m_blRetainPitch = dcs.m_blRetainPitch;
		m_blAutoAdvance = dcs.m_blAutoAdvance;

		if (m_blRememberAttack != blOldRememberAttack &&
			m_blRememberAttack == FALSE)
		{
			TheSong.RemoveAttacks( -1 );
			TheSong.Make();
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CCursor::OnSettingsChannel( HINSTANCE hInstance, HWND hwndParent )
{
	CDialogChannelSettings dcs;

	for (int i = 0; i < CHANNEL_MAX; i++)
	{
		dcs.m_iPan[ i ] = m_iPan[ i ];
	}

	if (dcs.DoModal( hInstance, hwndParent ) == IDOK)
	{
		for (int i = 0; i < CHANNEL_MAX; i++)
		{
			m_iPan[ i ] = dcs.m_iPan[ i ];
		}

		return TRUE;
	}

	return FALSE;
}

void CCursor::OnPercussion()
{
	CDialogPercussion dp;

	dp.m_iPitch = 0;

	if (dp.DoModal( CApplication::GetInstance(), CApplication::GetMainWindowHandle() ))
	{
		MoveToPitch( dp.m_iPitch );
	}
}


/*
void CCursor::HiliteVoice( HDC hdc, int iNewVoice, int iOldVoice )
{
// called by CCursor::Move()

	RECT rcNew = TheSong.GetRect( iNewVoice, -1 );
	RECT rcOld = TheSong.GetRect( iOldVoice, -1 );

	// transfer old voice from dc
	static BOOL FirstTime = TRUE;
	if (!FirstTime)
	{
		BitBlt( hdc, rcOld.left, rcOld.top, 2000, 200, &m_hdcVoice, 0, 0, SRCCOPY );
	}
	FirstTime = FALSE;

	// store new voice in dc
	BitBlt( m_hdcVoice, 0, 0, 2000, 200, hdc, rcNew.left, rcNew.top, SRCCOPY );

	// hilite new voice in blue
	HPEN hBluePen = CreatePen( PS_SOLID, 1, RGB(0,0,255) );
	HPEN hOldPen = SelectObject( hdc, &hBluePen );

	HBRUSH hBlueBrush = CreateSolidBrush( RGB(0,0,255) );
	HBRUSH hOldBrush = SelectObject( hdc, hBlueBrush );

	int iOldMode = SetROP2( hdc, R2_MERGEPEN );

	Rectangle( hdc, rcNew.left, rcNew.top, rcNew.right, rcNew.bottom );

	// clean up
	SelectObject( hdc, hOldPen );
	SelectObject( hdc, hOldBrush );
	SetROP2( hdc, iOldMode );
}

void CCursor::HiliteVoice( HDC hdc )
{
	static BOOL FirstTime = TRUE;

	if (!FirstTime)
	{
		RECT rc = TheSong.GetRect( m_iVoice, -1 );

		// hilite current voice in blue
		HPEN hBluePen = CreatePen( PS_SOLID, 1, RGB(0,0,255) );
		HPEN hOldPen = SelectObject( hdc, hBluePen );

		HBRUSH hBlueBrush = CreateSolidBrush( RGB(0,0,255) );
		HBRUSH hOldBrush = SelectObject( hdc, hBlueBrush );

		int iOldMode = SetROP2( hdc, R2_MERGEPEN );

		Rectangle( hdc, rc.left, rc.top, rc.right, rc.bottom );

		// clean up
		SelectObject( hdc, hOldPen );
		SelectObject( hdc, hOldBrush );
		SetROP2( hdc, iOldMode );
	}
	else
	{
		HiliteVoice( hdc, 0, 0 );
		FirstTime = FALSE;
	}
}
*/







