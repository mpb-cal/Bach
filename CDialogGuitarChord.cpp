
#include "CDialogGuitarChord.h"
#include "CString.h"
#include "Constant.h"


const int DIALOG_TICKS_MIN = 1;
const int DIALOG_TICKS_MAX = 32;

CDialogGuitarChord::CDialogGuitarChord()
{
	lstrcpy( m_szDialogResourceName, "GuitarChord" );
}


BOOL CDialogGuitarChord::OnInitDialog( HWND hDlg )
{
	::CheckDlgButton( hDlg, IDC_CHORDBOOL, m_blChord);
	::CheckDlgButton( hDlg, IDC_BASSNOTE, m_blBassNote );
	::CheckDlgButton( hDlg, IDC_MELODY, m_blMelody );

	char szTicks[ 16 ] = "";
	sprintf( szTicks, "%d", m_iTicks );
	::SetDlgItemText(	hDlg, 
						IDC_TICKS, 
						szTicks	);

	CString Roots[ 12 ];
	CString ChordTypes[ 19 ];

	Roots[ 0 ] = "C";
	Roots[ 1 ] = "Db";
	Roots[ 2 ] = "D";
	Roots[ 3 ] = "Eb";
	Roots[ 4 ] = "E";
	Roots[ 5 ] = "F";
	Roots[ 6 ] = "Gb";
	Roots[ 7 ] = "G";
	Roots[ 8 ] = "Ab";
	Roots[ 9 ] = "A";
	Roots[ 10 ] = "Bb";
	Roots[ 11 ] = "B";

	ChordTypes[ 0 ] = "M";
	ChordTypes[ 1 ] = "M7";
	ChordTypes[ 2 ] = "m";
	ChordTypes[ 3 ] = "m7";
	ChordTypes[ 4 ] = "m9";
	ChordTypes[ 5 ] = "m11";
	ChordTypes[ 6 ] = "7";
	ChordTypes[ 7 ] = "b9";
	ChordTypes[ 8 ] = "9";
	ChordTypes[ 9 ] = "13";
	ChordTypes[ 10 ] = "13b9";
	ChordTypes[ 11 ] = "m7b5";
	ChordTypes[ 12 ] = "dim7";
	ChordTypes[ 13 ] = "6";
	ChordTypes[ 14 ] = "m6";
	ChordTypes[ 15 ] = "aug";
	ChordTypes[ 16 ] = "aug7";
	ChordTypes[ 17 ] = "sus";
	ChordTypes[ 18 ] = "sus7";

	int r, c;
	for (r = 0; r < 12; r++)
	{
		for (c = 0; c < 19; c++)
		{
			CString Chord = Roots[ r ] + ChordTypes[ c ];
			::SendDlgItemMessage( hDlg, IDC_CHORD, CB_ADDSTRING, (WPARAM)0, (LPARAM)(char*)Chord );
		}
	}


	::SendDlgItemMessage(	hDlg, 
							IDC_CHORD, 
							CB_SETCURSEL, 
							(WPARAM)m_iChord, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogGuitarChord::OnOk( HWND hDlg )
{
	// retrieve ticks
	char szTicks[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_TICKS, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szTicks ), 
							(LPARAM)szTicks	);

	m_iTicks = atoi( szTicks );
	CHECK_BOUNDS( m_iTicks, TICKS );

	// retrieve chord
	m_iChord = ::SendDlgItemMessage(	hDlg, 
										IDC_CHORD, 
										CB_GETCURSEL, 
										(WPARAM)0, 
										(LPARAM)0	);



	m_blChord = ::IsDlgButtonChecked( hDlg, IDC_CHORDBOOL );
	m_blBassNote = ::IsDlgButtonChecked( hDlg, IDC_BASSNOTE );
	m_blMelody = ::IsDlgButtonChecked( hDlg, IDC_MELODY );

	return TRUE;
}

BOOL CDialogGuitarChord::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

