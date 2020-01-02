// CNote.cpp

#include "stdafx.h"
#include "resource.h"

#ifndef __CAPPLICATION_H__
#include "CApplication.h"
#endif

#ifndef __CNOTE_H__
#include "CNote.h"
#endif

#ifndef __CONSTANT_H__
#include "Constant.h"
#endif

#ifndef __CBMSTORAGE_H__
#include "CBMStorage.h"
#endif


// note names for relative pitches 0 - 11
static const char* sa_szNoteNames[ 12 ] =
{
	"C ", "C#", "D ", "D#", "E ", "F ", "F#", "G ", "G#", "A ", "A#", "B "
};

// Ypos of note C for 2 clefs, 12 octaves
static const int sa_nYpos[ 2 ][ 12 ] = 
{
	{ 152, 124, 96, 68, 40, 12, -16, -44, -72, -100, -128 },	// CLEF_TREBLE
	{ 104, 76, 48, 20, -8, -36, -64, -92, -120, -148, -176 }	// CLEF_BASS
};

// scale degrees for relative pitches 0 - 11 for each of 6 keys
static const char* sa_szScaleDegree[ 6 ][ 12 ] =
{
	{ "1 ", "1#", "2 ", "2#", "3 ", "4 ", "4#", "5 ", "5#", "6 ", "6#", "7 " },	// CMAJOR
	{ "4 ", "4#", "5 ", "5#", "6 ", "6#", "7 ", "1 ", "1#", "2 ", "2#", "3 " },	// GMAJOR
	{ "6#", "7 ", "1 ", "1#", "2 ", "2#", "3 ", "4 ", "4#", "5 ", "5#", "6 " },	// DMAJOR
	{ "2#", "3 ", "4 ", "4#", "5 ", "5#", "6 ", "6#", "7 ", "1 ", "1#", "2 " },	// AMAJOR
	{ "5#", "6 ", "6#", "7 ", "1 ", "1#", "2 ", "2#", "3 ", "4 ", "4#", "5 " },	// EMAJOR
	{ "1#", "2 ", "2#", "3 ", "4 ", "4#", "5 ", "5#", "6 ", "6#", "7 ", "1 " },	// BMAJOR
};

// half steps to move up major scale (BCDEFGABC)
static const int sa_nMajorScale[] =
{
	1, 2, 2, 1, 2, 2, 2, 1
};

CNote::CNote() :
	CMusicalThing(),
	m_iAbsolutePitch( 0 ),
	m_iRelativePitch( 0 ),
	m_iOctave( 0 ),
	m_iValue( NOTE_QUARTER ),
	m_iBitmapId( BM_QUARTER ),
	m_iCaretBitmapId( 0 ),
	m_iAccidentalBitmapId( BM_NOACC ),
	m_iLedgerLines( 0 ),
	m_iXpos( 0 ),
	m_iYpos( 0 ),
	m_iStaffTop( 0 ),
	m_blStemUp( FALSE )
{
	lstrcpy( m_szRelativePitch, "" );
	lstrcpy( m_szScaleDegree, "" );
}

CNote::CNote(	int iAbsolutePitch, 
				int iKey, 
				int iValue, 
				int iClef, 
				int iXpos, 
				int iStaffTop, 
				int& riTicksUsed, 
				int& riTicksLeftOver	) :
	CMusicalThing(),
	m_iAbsolutePitch( iAbsolutePitch ),
	m_iRelativePitch( 0 ),
	m_iOctave( 0 ),
	m_iValue( iValue ),
	m_iBitmapId( BM_QUARTER ),
	m_iCaretBitmapId( 0 ),
	m_iAccidentalBitmapId( BM_NOACC ),
	m_iLedgerLines( 0 ),
	m_iXpos( iXpos ),
	m_iYpos( 0 ),
	m_iStaffTop( iStaffTop ),
	m_blStemUp( FALSE )
{

	ASSERT( iAbsolutePitch >= NOTE_BOTTOMPITCH && iAbsolutePitch <= NOTE_TOPPITCH );
	ASSERT( iKey >= 0 && iKey < LastKeySignature );
	ASSERT( iValue > 0 );
	ASSERT( iClef >= 0 && iClef < LastClef );

	m_iKey = iKey;
	
	m_iRelativePitch = m_iAbsolutePitch % 12;
	lstrcpy( m_szRelativePitch, sa_szNoteNames[ m_iRelativePitch ] );
	lstrcpy( m_szScaleDegree, sa_szScaleDegree[ m_iKey ][ m_iRelativePitch ] );
	
	m_iOctave = m_iAbsolutePitch / 12;

	m_iClef = iClef;
	
	FindYpos();

	FindBitmapId( riTicksLeftOver );
	riTicksUsed = m_iValue;

	FindLedgerLines();
}

const CNote& 
CNote::operator= ( const CNote& rvalue )
{
	CMusicalThing::operator=( rvalue );

	m_iAbsolutePitch = rvalue.m_iAbsolutePitch;
	m_iRelativePitch = rvalue.m_iRelativePitch;
	lstrcpy( m_szRelativePitch, rvalue.m_szRelativePitch );
	lstrcpy( m_szScaleDegree, rvalue.m_szScaleDegree );
	m_iOctave = rvalue.m_iOctave;
	m_iValue = rvalue.m_iValue;
	m_iBitmapId = rvalue.m_iBitmapId;
	m_iCaretBitmapId = rvalue.m_iCaretBitmapId;
	m_iAccidentalBitmapId = rvalue.m_iAccidentalBitmapId;
	m_iLedgerLines = rvalue.m_iLedgerLines;
	m_iXpos = rvalue.m_iXpos;
	m_iYpos = rvalue.m_iYpos;
	m_iStaffTop = rvalue.m_iStaffTop;
	m_blStemUp = rvalue.m_blStemUp;
	
	return *this;
}

void CNote::Dump( char* szString ) const
{
	char sz[ 1024 ];

	sprintf( sz, "START NOTE:\r\n" ); lstrcat( szString, sz );

	CMusicalThing::Dump( szString );

	sprintf( sz, "m_iAbsolutePitch = %d\r\n", m_iAbsolutePitch ); lstrcat( szString, sz );
	sprintf( sz, "m_iRelativePitch = %d\r\n", m_iRelativePitch ); lstrcat( szString, sz );
	sprintf( sz, "m_szRelativePitch = %s\r\n", m_szRelativePitch ); lstrcat( szString, sz );
	sprintf( sz, "m_szScaleDegree = %s\r\n", m_szScaleDegree ); lstrcat( szString, sz );
	sprintf( sz, "m_iOctave = %d\r\n", m_iOctave ); lstrcat( szString, sz );
	sprintf( sz, "m_iValue = %d\r\n", m_iValue ); lstrcat( szString, sz );
	sprintf( sz, "m_iBitmapId = %d\r\n", m_iBitmapId ); lstrcat( szString, sz );
	sprintf( sz, "m_iCaretBitmapId = %d\r\n", m_iCaretBitmapId ); lstrcat( szString, sz );
	sprintf( sz, "m_iAccidentalBitmapId = %d\r\n", m_iAccidentalBitmapId ); lstrcat( szString, sz );
	sprintf( sz, "m_iLedgerLines = %d\r\n", m_iLedgerLines ); lstrcat( szString, sz );
	sprintf( sz, "m_iXpos = %d\r\n", m_iXpos ); lstrcat( szString, sz );
	sprintf( sz, "m_iYpos = %d\r\n", m_iYpos ); lstrcat( szString, sz );
	sprintf( sz, "m_iStaffTop = %d\r\n", m_iStaffTop ); lstrcat( szString, sz );
	sprintf( sz, "m_blStemUp = %d\r\n", m_blStemUp ); lstrcat( szString, sz );

	sprintf( sz, "END NOTE:\r\n" ); lstrcat( szString, sz );
}

BOOL CNote::Draw( HDC hdc ) const
{
	int iXOrigin = CApplication::GetXOrigin();
	int iYOrigin = CApplication::GetYOrigin();

	int iStaffTop;
	if (CApplication::IsGrandStaff())
	{
		switch (m_iClef)
		{
			case CLEF_TREBLE :
				iStaffTop = STAFF_TOP;
				break;

			case CLEF_BASS :
				iStaffTop = STAFF_TOP + VOICE_HEIGHT;
				break;
		}
	}
	else
	{
		iStaffTop = m_iStaffTop;
	}

	// draw accidental
	if (m_iAbsolutePitch != NOTE_REST)
	{
		if (m_blStemUp)
		{
			CBMStorage::DrawBitmap(	hdc, 
									(iXOrigin + m_iXpos) / CApplication::GetZoomDivisor(), 
									(iYOrigin + iStaffTop + m_iYpos + NOTE_SPACEABOVE + NOTE_HEIGHT)  / CApplication::GetZoomDivisor(), 
									CApplication::GetZoomDivisor(),
									m_iAccidentalBitmapId		);
		}
		else
		{
			CBMStorage::DrawBitmap(	hdc, 
									(iXOrigin + m_iXpos) / CApplication::GetZoomDivisor(), 
									(iYOrigin + iStaffTop + m_iYpos + NOTE_SPACEABOVE - NOTE_ACCHEIGHT) / CApplication::GetZoomDivisor(), 
									CApplication::GetZoomDivisor(),
									m_iAccidentalBitmapId		);
		}
	}

	// draw ledger lines
	if (m_iLedgerLines > 0) // ledger lines above staff
	{
		int nLedgerLineYpos = (iStaffTop - STAFF_SPACEWIDTH) / CApplication::GetZoomDivisor();
		for (int i = 0; i < m_iLedgerLines; i++)
		{
			::MoveToEx(	hdc, 
						(iXOrigin + m_iXpos - 1) / CApplication::GetZoomDivisor(),
						(iYOrigin + nLedgerLineYpos) / CApplication::GetZoomDivisor(),
						NULL	);
			::LineTo(	hdc, 
						(iXOrigin + m_iXpos + NOTE_WIDTH) / CApplication::GetZoomDivisor(),
						(iYOrigin + nLedgerLineYpos) / CApplication::GetZoomDivisor()	);

			nLedgerLineYpos -= STAFF_SPACEWIDTH / CApplication::GetZoomDivisor();
		}
	}
	else if (m_iLedgerLines < 0) // ledger lines below staff
	{
		int nLedgerLineYpos = (iStaffTop + STAFF_HEIGHT + STAFF_SPACEWIDTH) / CApplication::GetZoomDivisor();
		for (int i = 0; i > m_iLedgerLines; i--)
		{
			::MoveToEx(	hdc, 
						(iXOrigin + m_iXpos - 1) / CApplication::GetZoomDivisor(),
						(iYOrigin + nLedgerLineYpos) / CApplication::GetZoomDivisor(),
						NULL	);
			::LineTo(	hdc, 
						(iXOrigin + m_iXpos + NOTE_WIDTH) / CApplication::GetZoomDivisor(), 
						(iYOrigin + nLedgerLineYpos) / CApplication::GetZoomDivisor()	);

			nLedgerLineYpos += STAFF_SPACEWIDTH / CApplication::GetZoomDivisor();
		}
	}

	// draw note
	CBMStorage::DrawBitmap(	hdc, 
							(iXOrigin + m_iXpos) / CApplication::GetZoomDivisor(), 
							(iYOrigin + iStaffTop + m_iYpos) / CApplication::GetZoomDivisor(), 
							CApplication::GetZoomDivisor(),
							m_iBitmapId	);

	return TRUE;
}

// change AbsolutePitch
BOOL CNote::ChangePitch( ENoteDirection Direction )
{

	ASSERT( Direction >= 0 && Direction < Last_NoteDirection );

	int nMove;
	int iAbsolutePitch = m_iAbsolutePitch;

	switch (Direction)
	{
		case NOTE_UP :
			nMove = atoi( m_szScaleDegree );
			iAbsolutePitch += sa_nMajorScale[ nMove ];
			break;

		case NOTE_UPOCTAVE :
			iAbsolutePitch += 12;
			break;

		case NOTE_DOWN :
			nMove = atoi(m_szScaleDegree) - 1;
			iAbsolutePitch -= sa_nMajorScale[ nMove ];
			break;

		case NOTE_DOWNOCTAVE :
			iAbsolutePitch -= 12;
			break;
	}

	// error-checking
	if (iAbsolutePitch < NOTE_BOTTOMPITCH || iAbsolutePitch > NOTE_TOPPITCH)
	{
		return FALSE;
	}

	return SetAbsolutePitch( iAbsolutePitch );
}

BOOL CNote::SetAbsolutePitch( int iAbsolutePitch )
{
	ASSERT( iAbsolutePitch >= NOTE_BOTTOMPITCH && iAbsolutePitch <= NOTE_TOPPITCH );

	m_iAbsolutePitch = iAbsolutePitch;

	// update other members
	m_iRelativePitch = m_iAbsolutePitch % 12;
	lstrcpy( m_szRelativePitch, sa_szNoteNames[ m_iRelativePitch ] );
	lstrcpy( m_szScaleDegree, sa_szScaleDegree[ m_iKey ][ m_iRelativePitch ] );
	m_iOctave = m_iAbsolutePitch / 12;

	int nDummy;
	FindBitmapId( nDummy );

	FindYpos();

	FindLedgerLines();

	return TRUE;
}

BOOL CNote::SetRelativePitch( int nRelativePitch )
{
	nRelativePitch %= 12;
	if (nRelativePitch < 0)
	{
		nRelativePitch += 12;
	}

	return SetAbsolutePitch( m_iOctave * 12 + nRelativePitch );
}

BOOL CNote::SetSzRelativePitch( const char* szRelativePitch )
{

	int nRelativePitch;

	if		(ARE_EQUAL( szRelativePitch, "C"))	nRelativePitch = 0;
	else if (ARE_EQUAL( szRelativePitch, "C#"))	nRelativePitch = 1;
	else if (ARE_EQUAL( szRelativePitch, "D"))	nRelativePitch = 2;
	else if (ARE_EQUAL( szRelativePitch, "D#"))	nRelativePitch = 3;
	else if (ARE_EQUAL( szRelativePitch, "E"))	nRelativePitch = 4;
	else if (ARE_EQUAL( szRelativePitch, "F"))	nRelativePitch = 5;
	else if (ARE_EQUAL( szRelativePitch, "F#"))	nRelativePitch = 6;
	else if (ARE_EQUAL( szRelativePitch, "G"))	nRelativePitch = 7;
	else if (ARE_EQUAL( szRelativePitch, "G#"))	nRelativePitch = 8;
	else if (ARE_EQUAL( szRelativePitch, "A"))	nRelativePitch = 9;
	else if (ARE_EQUAL( szRelativePitch, "A#"))	nRelativePitch = 10;
	else if (ARE_EQUAL( szRelativePitch, "B"))	nRelativePitch = 11;
	else ASSERT( 0 );

	return SetRelativePitch( nRelativePitch );
}

BOOL CNote::SetScaleDegree( const char* szScaleDegree )
{

	int nRelativePitch;

	if		(ARE_EQUAL( szScaleDegree, "1"))		nRelativePitch = 0;
	else if (ARE_EQUAL( szScaleDegree, "1#"))		nRelativePitch = 1;
	else if (ARE_EQUAL( szScaleDegree, "2"))		nRelativePitch = 2;
	else if (ARE_EQUAL( szScaleDegree, "2#"))		nRelativePitch = 3;
	else if (ARE_EQUAL( szScaleDegree, "3"))		nRelativePitch = 4;
	else if (ARE_EQUAL( szScaleDegree, "4"))		nRelativePitch = 5;
	else if (ARE_EQUAL( szScaleDegree, "4#"))		nRelativePitch = 6;
	else if (ARE_EQUAL( szScaleDegree, "5"))		nRelativePitch = 7;
	else if (ARE_EQUAL( szScaleDegree, "5#"))		nRelativePitch = 8;
	else if (ARE_EQUAL( szScaleDegree, "6"))		nRelativePitch = 9;
	else if (ARE_EQUAL( szScaleDegree, "6#"))		nRelativePitch = 10;
	else if (ARE_EQUAL( szScaleDegree, "7"))		nRelativePitch = 11;
	else ASSERT( 0 );

	return SetRelativePitch( nRelativePitch );
}

BOOL CNote::SetOctave( int nOctave )
{

	ASSERT( nOctave >= NOTE_BOTTOMOCTAVE && nOctave <= NOTE_TOPOCTAVE );

	return SetAbsolutePitch( nOctave * 12 + m_iRelativePitch );
}

BOOL CNote::SetKey( int iKey )
{
	if (CMusicalThing::SetKey( iKey ))
	{
		return SetAbsolutePitch( m_iAbsolutePitch );
	}

	return FALSE;
}

BOOL CNote::SetValue( int iValue )
{
	m_iValue = iValue;

	FindYpos();

	int iDummy;
	FindBitmapId( iDummy );

	return TRUE;
}

BOOL CNote::SetClef( int iClef )
{
	if (CMusicalThing::SetClef( iClef ))
	{
		FindYpos();
		FindLedgerLines();

		return TRUE;
	}

	return FALSE;
}

BOOL CNote::IsSharp() const
{
	return (m_szRelativePitch[ 1 ] == '#');
}

int CNote::ChromaticToKey( int nPitch )
{
	char szScaleDegree[ 5 ];
	lstrcpy( szScaleDegree, sa_szScaleDegree[ m_iKey ][ nPitch ] );
	int nNewPitch;

	if		(ARE_EQUAL( szScaleDegree, "1 "))	nNewPitch = 0;
	else if (ARE_EQUAL( szScaleDegree, "1#"))	nNewPitch = 1;
	else if (ARE_EQUAL( szScaleDegree, "2 "))	nNewPitch = 2;
	else if (ARE_EQUAL( szScaleDegree, "2#"))	nNewPitch = 3;
	else if (ARE_EQUAL( szScaleDegree, "3 "))	nNewPitch = 4;
	else if (ARE_EQUAL( szScaleDegree, "4 "))	nNewPitch = 5;
	else if (ARE_EQUAL( szScaleDegree, "4#"))	nNewPitch = 6;
	else if (ARE_EQUAL( szScaleDegree, "5 "))	nNewPitch = 7;
	else if (ARE_EQUAL( szScaleDegree, "5#"))	nNewPitch = 8;
	else if (ARE_EQUAL( szScaleDegree, "6 "))	nNewPitch = 9;
	else if (ARE_EQUAL( szScaleDegree, "6#"))	nNewPitch = 10;
	else if (ARE_EQUAL( szScaleDegree, "7 "))	nNewPitch = 11;
	else ASSERT( 0 );

	return nNewPitch;
}

// static method
void CNote::TicksToString( int nTicks, char* szTicks )
{
	switch (nTicks)
	{
		case NOTE_THIRTYSECOND :	lstrcpy( szTicks, "Thirty-second" );	break;
		case NOTE_SIXTEENTH :		lstrcpy( szTicks, "Sixteenth" );		break;
		case NOTE_SIXTEENTHD :		lstrcpy( szTicks, "Dotted Sixteenth" );	break;
		case NOTE_EIGHTH :			lstrcpy( szTicks, "Eighth" );			break;
		case NOTE_EIGHTHD :			lstrcpy( szTicks, "Dotted Eighth" );	break;
		case NOTE_QUARTER :			lstrcpy( szTicks, "Quarter" );			break;
		case NOTE_QUARTERD :		lstrcpy( szTicks, "Dotted Quarter" );	break;
		case NOTE_HALF :			lstrcpy( szTicks, "Half" );				break;
		case NOTE_HALFD :			lstrcpy( szTicks, "Dotted Half" );		break;
		case NOTE_WHOLE :			lstrcpy( szTicks, "Whole" );			break;
		
		default:					lstrcpy( szTicks, "" );
	}
}

// static method
int CNote::StringToTicks( const char* szString )
{

	if		(ARE_EQUAL( szString, "Thirty-second"))		return NOTE_THIRTYSECOND;
	else if (ARE_EQUAL( szString, "Sixteenth"))			return NOTE_SIXTEENTH;
	else if (ARE_EQUAL( szString, "Dotted Sixteenth"))	return NOTE_SIXTEENTHD;
	else if (ARE_EQUAL( szString, "Eighth"))			return NOTE_EIGHTH;
	else if (ARE_EQUAL( szString, "Dotted Eighth"))		return NOTE_EIGHTHD;
	else if (ARE_EQUAL( szString, "Quarter"))			return NOTE_QUARTER;
	else if (ARE_EQUAL( szString, "Dotted Quarter"))	return NOTE_QUARTERD;
	else if (ARE_EQUAL( szString, "Half"))				return NOTE_HALF;
	else if (ARE_EQUAL( szString, "Dotted Half"))		return NOTE_HALFD;
	else if (ARE_EQUAL( szString, "Whole"))				return NOTE_WHOLE;
	
	else												return 0;
}

// static method
void CNote::SharpenCaretBitmapId( int& rnCaretBitmapId )
{
	switch (rnCaretBitmapId)
	{
		case IDB_CSIXTEENTH : rnCaretBitmapId = IDB_CSIXTEENTHS; break;
		case IDB_CEIGHTH : rnCaretBitmapId = IDB_CEIGHTHS; break;
		case IDB_CEIGHTHD : rnCaretBitmapId = IDB_CEIGHTHDS; break;
		case IDB_CQUARTER : rnCaretBitmapId = IDB_CQUARTERS; break;
		case IDB_CQUARTERD : rnCaretBitmapId = IDB_CQUARTERDS; break;
		case IDB_CHALF : rnCaretBitmapId = IDB_CHALFS; break;
		case IDB_CHALFD : rnCaretBitmapId = IDB_CHALFDS; break;
		case IDB_CWHOLE : rnCaretBitmapId = IDB_CWHOLES; break;
	}
}

// static method
void CNote::TransposeTickDiatonic(	BYTE& rbTick, 
									int nPitches, 
									ENoteDirection Direction, 
									int iKey	)
{

	ASSERT( Direction >= 0 && Direction < Last_NoteDirection );

	for (int i = 0; i < nPitches; i++)
	{
		int nRelativePitch = rbTick % 12;
		char szScaleDegree[ 5 ];
		lstrcpy( szScaleDegree, sa_szScaleDegree[ iKey ][ nRelativePitch ] );

		int nMove;

		BYTE bOldTick = rbTick;

		switch (Direction)
		{
			case NOTE_UP :
				nMove = atoi(szScaleDegree);
				rbTick += (BYTE)sa_nMajorScale[ nMove ];
				break;

			case NOTE_DOWN :
				nMove = atoi(szScaleDegree) - 1;
				rbTick -= (BYTE)sa_nMajorScale[ nMove ];
				break;
		}

		// error-checking
		if (rbTick < NOTE_BOTTOMPITCH || rbTick >= NOTE_TOPPITCH)
		{
			rbTick = bOldTick;
		}
	}
}

// static method
int CNote::StringToKey( const char* szKey )
{
	if		(ARE_EQUAL( szKey, "C Major")) return CMAJOR;
	else if (ARE_EQUAL( szKey, "G Major")) return GMAJOR;
	else if (ARE_EQUAL( szKey, "D Major")) return DMAJOR;
	else if (ARE_EQUAL( szKey, "A Major")) return AMAJOR;
	else if (ARE_EQUAL( szKey, "E Major")) return EMAJOR;
	else if (ARE_EQUAL( szKey, "B Major")) return BMAJOR;

	else return CMAJOR;
}

// static method
void CNote::KeyToString( int iKey, char* szKey )
{
	switch (iKey)
	{
		case CMAJOR : lstrcpy( szKey, "C Major" ); break;
		case GMAJOR : lstrcpy( szKey, "G Major" ); break;
		case DMAJOR : lstrcpy( szKey, "D Major" ); break;
		case AMAJOR : lstrcpy( szKey, "A Major" ); break;
		case EMAJOR : lstrcpy( szKey, "E Major" ); break;
		case BMAJOR : lstrcpy( szKey, "B Major" ); break;

		default : lstrcpy( szKey, "C Major" );
	}
}

// protected methods

void CNote::FindYpos()
{
	if (m_iAbsolutePitch == NOTE_REST)
	{
		m_iYpos = 0;
	}
	else
	{
		m_iYpos = sa_nYpos[ m_iClef ][ m_iOctave ];

		switch (m_szRelativePitch[ 0 ])
		{
			case 'C' : m_iYpos -= 0; break;
			case 'D' : m_iYpos -= 4; break;
			case 'E' : m_iYpos -= 8; break;
			case 'F' : m_iYpos -= 12; break;
			case 'G' : m_iYpos -= 16; break;
			case 'A' : m_iYpos -= 20; break;
			case 'B' : m_iYpos -= 24; break;

			default : ASSERT( 0 );
		}
	}
}

void CNote::FindBitmapId( int& riTicksLeftOver )
{
	int iBitmap[ 4 ][ NOTE_WHOLE ] =
	{
		{
			BM_THIRTYSECONDR,
			BM_SIXTEENTHR,
			BM_SIXTEENTHDR,
			BM_EIGHTHR, BM_EIGHTHR,
			BM_EIGHTHDR, BM_EIGHTHDR,
			BM_QUARTERR, BM_QUARTERR, BM_QUARTERR, BM_QUARTERR,
			BM_QUARTERDR, BM_QUARTERDR, BM_QUARTERDR, BM_QUARTERDR,
			BM_HALFR, BM_HALFR, BM_HALFR, BM_HALFR, BM_HALFR, BM_HALFR, BM_HALFR, BM_HALFR, 
			BM_HALFDR, BM_HALFDR, BM_HALFDR, BM_HALFDR, BM_HALFDR, BM_HALFDR, BM_HALFDR, BM_HALFDR,
			BM_WHOLER
		},
		{
			BM_THIRTYSECOND,
			BM_SIXTEENTH,
			BM_SIXTEENTHD,
			BM_EIGHTH, BM_EIGHTH,
			BM_EIGHTHD, BM_EIGHTHD,
			BM_QUARTER, BM_QUARTER, BM_QUARTER, BM_QUARTER,
			BM_QUARTERD, BM_QUARTERD, BM_QUARTERD, BM_QUARTERD,
			BM_HALF, BM_HALF, BM_HALF, BM_HALF, BM_HALF, BM_HALF, BM_HALF, BM_HALF,
			BM_HALFD, BM_HALFD, BM_HALFD, BM_HALFD, BM_HALFD, BM_HALFD, BM_HALFD, BM_HALFD,
			BM_WHOLE
		},
		{
			IDB_CTHIRTYSECONDS,
			IDB_CSIXTEENTHS,
			IDB_CSIXTEENTHDS,
			IDB_CEIGHTHS, IDB_CEIGHTHS,
			IDB_CEIGHTHDS, IDB_CEIGHTHDS,
			IDB_CQUARTERS, IDB_CQUARTERS, IDB_CQUARTERS, IDB_CQUARTERS,
			IDB_CQUARTERDS, IDB_CQUARTERDS, IDB_CQUARTERDS, IDB_CQUARTERDS,
			IDB_CHALFS, IDB_CHALFS, IDB_CHALFS, IDB_CHALFS, IDB_CHALFS, IDB_CHALFS, IDB_CHALFS, IDB_CHALFS,
			IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS, IDB_CHALFDS,
			IDB_CWHOLES
		},
		{
			IDB_CTHIRTYSECOND,
			IDB_CSIXTEENTH,
			IDB_CSIXTEENTHD,
			IDB_CEIGHTH, IDB_CEIGHTH,
			IDB_CEIGHTHD, IDB_CEIGHTHD,
			IDB_CQUARTER, IDB_CQUARTER, IDB_CQUARTER, IDB_CQUARTER,
			IDB_CQUARTERD, IDB_CQUARTERD, IDB_CQUARTERD, IDB_CQUARTERD,
			IDB_CHALF, IDB_CHALF, IDB_CHALF, IDB_CHALF, IDB_CHALF, IDB_CHALF, IDB_CHALF, IDB_CHALF, 
			IDB_CHALFD, IDB_CHALFD, IDB_CHALFD, IDB_CHALFD, IDB_CHALFD, IDB_CHALFD, IDB_CHALFD, IDB_CHALFD,
			IDB_CWHOLE
		}
	};

	int iValue = m_iValue - 1;
	if (iValue >= NOTE_WHOLE)
	{
		iValue = NOTE_WHOLE - 1;
	}

	if (m_iAbsolutePitch == NOTE_REST) 
	{
		m_iBitmapId = iBitmap[ 0 ][ iValue ];
	}
	else 
	{
		m_iBitmapId = iBitmap[ 1 ][ iValue ];

		switch (m_iClef)
		{
			case CLEF_TREBLE :
				if (m_iAbsolutePitch < 71)
				{
					TurnStemUp();
				}
				break;

			case CLEF_BASS :
				if (m_iAbsolutePitch < 50)
				{
					TurnStemUp();
				}
				break;
		}

		if (m_szRelativePitch[ 1 ] == '#')
		{
			m_iAccidentalBitmapId = BM_SHARP;
		}
		else
		{
			m_iAccidentalBitmapId = BM_NOACC;
		}
	}

	if (m_szRelativePitch[ 1 ] == '#') m_iCaretBitmapId = iBitmap[ 2 ][ iValue ];
	else m_iCaretBitmapId = iBitmap[ 3 ][ iValue ];

	int iTicksLeftOver[ NOTE_WHOLE ] =
	{
		0, 0, 0, 0, 1, 0, 1, 0, 1, 2, 3, 0, 1, 2, 3, 0,
		1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0
	};

	if (m_iValue > NOTE_WHOLE)
	{
		riTicksLeftOver = m_iValue - NOTE_WHOLE;
	}
	else
	{
		riTicksLeftOver = iTicksLeftOver[ iValue ];
	}

	m_iValue = m_iValue - riTicksLeftOver;
}

void CNote::FindLedgerLines()
{

	m_iLedgerLines = 0;

	if (m_iAbsolutePitch == NOTE_REST) return;

	if (m_iClef == CLEF_TREBLE)
	{
		switch (m_szRelativePitch[ 0 ])
		{
			case 'C' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -18; break;
					case 1 : m_iLedgerLines = -15; break;
					case 2 : m_iLedgerLines = -11; break;
					case 3 : m_iLedgerLines = -8; break;
					case 4 : m_iLedgerLines = -4; break;
					case 5 : m_iLedgerLines = -1; break;
					case 6 : m_iLedgerLines = 0; break;
					case 7 : m_iLedgerLines = 2; break;
					case 8 : m_iLedgerLines = 5; break;
					case 9 : m_iLedgerLines = 9; break;
					case 10 : m_iLedgerLines = 12; break;
				}
				break;

			case 'D' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -18; break;
					case 1 : m_iLedgerLines = -14; break;
					case 2 : m_iLedgerLines = -11; break;
					case 3 : m_iLedgerLines = -7; break;
					case 4 : m_iLedgerLines = -4; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 0; break;
					case 7 : m_iLedgerLines = 2; break;
					case 8 : m_iLedgerLines = 6; break;
					case 9 : m_iLedgerLines = 9; break;
					case 10 : m_iLedgerLines = 13; break;
				}
				break;

			case 'E' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -17; break;
					case 1 : m_iLedgerLines = -14; break;
					case 2 : m_iLedgerLines = -10; break;
					case 3 : m_iLedgerLines = -7; break;
					case 4 : m_iLedgerLines = -3; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 0; break;
					case 7 : m_iLedgerLines = 3; break;
					case 8 : m_iLedgerLines = 6; break;
					case 9 : m_iLedgerLines = 10; break;
					case 10 : m_iLedgerLines = 13; break;
				}
				break;

			case 'F' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -17; break;
					case 1 : m_iLedgerLines = -13; break;
					case 2 : m_iLedgerLines = -10; break;
					case 3 : m_iLedgerLines = -6; break;
					case 4 : m_iLedgerLines = -3; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 0; break;
					case 7 : m_iLedgerLines = 3; break;
					case 8 : m_iLedgerLines = 7; break;
					case 9 : m_iLedgerLines = 10; break;
					case 10 : m_iLedgerLines = 14; break;
				}
				break;

			case 'G' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -16; break;
					case 1 : m_iLedgerLines = -13; break;
					case 2 : m_iLedgerLines = -9; break;
					case 3 : m_iLedgerLines = -6; break;
					case 4 : m_iLedgerLines = -2; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 0; break;
					case 7 : m_iLedgerLines = 4; break;
					case 8 : m_iLedgerLines = 7; break;
					case 9 : m_iLedgerLines = 11; break;
					case 10 : m_iLedgerLines = 14; break;
				}
				break;

			case 'A' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -16; break;
					case 1 : m_iLedgerLines = -12; break;
					case 2 : m_iLedgerLines = -9; break;
					case 3 : m_iLedgerLines = -5; break;
					case 4 : m_iLedgerLines = -2; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 1; break;
					case 7 : m_iLedgerLines = 4; break;
					case 8 : m_iLedgerLines = 8; break;
					case 9 : m_iLedgerLines = 11; break;
				}
				break;

			case 'B' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -15; break;
					case 1 : m_iLedgerLines = -12; break;
					case 2 : m_iLedgerLines = -8; break;
					case 3 : m_iLedgerLines = -5; break;
					case 4 : m_iLedgerLines = -1; break;
					case 5 : m_iLedgerLines = 0; break;
					case 6 : m_iLedgerLines = 1; break;
					case 7 : m_iLedgerLines = 5; break;
					case 8 : m_iLedgerLines = 8; break;
					case 9 : m_iLedgerLines = 12; break;
				}
				break;

		}
	}
	else if (m_iClef == CLEF_BASS)
	{
		switch (m_szRelativePitch[ 0 ])
		{
			case 'C' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -12; break;
					case 1 : m_iLedgerLines = -9; break;
					case 2 : m_iLedgerLines = -5; break;
					case 3 : m_iLedgerLines = -2; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 1; break;
					case 6 : m_iLedgerLines = 4; break;
					case 7 : m_iLedgerLines = 8; break;
					case 8 : m_iLedgerLines = 11; break;
					case 9 : m_iLedgerLines = 15; break;
					case 10 : m_iLedgerLines = 18; break;
				}
				break;

			case 'D' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -12; break;
					case 1 : m_iLedgerLines = -8; break;
					case 2 : m_iLedgerLines = -5; break;
					case 3 : m_iLedgerLines = -1; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 1; break;
					case 6 : m_iLedgerLines = 5; break;
					case 7 : m_iLedgerLines = 8; break;
					case 8 : m_iLedgerLines = 12; break;
					case 9 : m_iLedgerLines = 15; break;
					case 10 : m_iLedgerLines = 19; break;
				}
				break;

			case 'E' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -11; break;
					case 1 : m_iLedgerLines = -8; break;
					case 2 : m_iLedgerLines = -4; break;
					case 3 : m_iLedgerLines = -1; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 2; break;
					case 6 : m_iLedgerLines = 5; break;
					case 7 : m_iLedgerLines = 9; break;
					case 8 : m_iLedgerLines = 12; break;
					case 9 : m_iLedgerLines = 16; break;
					case 10 : m_iLedgerLines = 19; break;
				}
				break;

			case 'F' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -11; break;
					case 1 : m_iLedgerLines = -7; break;
					case 2 : m_iLedgerLines = -4; break;
					case 3 : m_iLedgerLines = 0; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 2; break;
					case 6 : m_iLedgerLines = 6; break;
					case 7 : m_iLedgerLines = 9; break;
					case 8 : m_iLedgerLines = 13; break;
					case 9 : m_iLedgerLines = 16; break;
					case 10 : m_iLedgerLines = 20; break;
				}
				break;

			case 'G' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -10; break;
					case 1 : m_iLedgerLines = -7; break;
					case 2 : m_iLedgerLines = -3; break;
					case 3 : m_iLedgerLines = 0; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 3; break;
					case 6 : m_iLedgerLines = 6; break;
					case 7 : m_iLedgerLines = 10; break;
					case 8 : m_iLedgerLines = 13; break;
					case 9 : m_iLedgerLines = 17; break;
					case 10 : m_iLedgerLines = 20; break;
				}
				break;

			case 'A' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -10; break;
					case 1 : m_iLedgerLines = -6; break;
					case 2 : m_iLedgerLines = -3; break;
					case 3 : m_iLedgerLines = 0; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 3; break;
					case 6 : m_iLedgerLines = 7; break;
					case 7 : m_iLedgerLines = 10; break;
					case 8 : m_iLedgerLines = 14; break;
					case 9 : m_iLedgerLines = 17; break;
				}
				break;

			case 'B' :
				switch (m_iOctave)
				{
					case 0 : m_iLedgerLines = -9; break;
					case 1 : m_iLedgerLines = -6; break;
					case 2 : m_iLedgerLines = -2; break;
					case 3 : m_iLedgerLines = 0; break;
					case 4 : m_iLedgerLines = 0; break;
					case 5 : m_iLedgerLines = 4; break;
					case 6 : m_iLedgerLines = 7; break;
					case 7 : m_iLedgerLines = 11; break;
					case 8 : m_iLedgerLines = 14; break;
					case 9 : m_iLedgerLines = 18; break;
				}
				break;

		}
	}
}

void CNote::TurnStemUp()
{

	m_blStemUp = TRUE;

	switch (m_iBitmapId)
	{
		case BM_THIRTYSECOND :	m_iBitmapId = BM_THIRTYSECONDU;	break;
		case BM_SIXTEENTH :		m_iBitmapId = BM_SIXTEENTHU;	break;
		case BM_SIXTEENTHD :	m_iBitmapId = BM_SIXTEENTHDU;	break;
		case BM_EIGHTH :		m_iBitmapId = BM_EIGHTHU;		break;
		case BM_EIGHTHD :		m_iBitmapId = BM_EIGHTHDU;		break;
		case BM_QUARTER :		m_iBitmapId = BM_QUARTERU;		break;
		case BM_QUARTERD :		m_iBitmapId = BM_QUARTERDU;		break;
		case BM_HALF :			m_iBitmapId = BM_HALFU;			break;
		case BM_HALFD :			m_iBitmapId = BM_HALFDU;		break;
		case BM_WHOLE :			m_iBitmapId = BM_WHOLEU;		break;
	}
}

