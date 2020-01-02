
#include "CDialogPercussion.h"
#include "Constant.h"
#include "GlobalDefs.h"
#include "CNote.h"
#include "CApplication.h"

CDialogPercussion::CDialogPercussion()
{
	lstrcpy( m_szDialogResourceName, "Percussion" );
}


BOOL CDialogPercussion::OnInitDialog( HWND hDlg )
{
	// fill in pitch combo box
	char szText[ 128 ] = "";
	for (int i = 0; i < PERCUSSION_TOTALPITCHES; i++)
	{
		lstrcpy( szText, g_pszPercussionPitches[ i ] );
		::SendDlgItemMessage(	hDlg, 
								IDC_PITCH, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szText	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_PITCH, 
							LB_SETCURSEL, 
							(WPARAM)m_iPitch, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogPercussion::OnOk( HWND hDlg )
{
	// retrieve pitch
	m_iPitch = ::SendDlgItemMessage(	hDlg, 
										IDC_PITCH, 
										LB_GETCURSEL, 
										(WPARAM)0, 
										(LPARAM)0	);

	// see if pitch is diatonic in CMajor
	m_iPitch = m_iPitch + PERCUSSION_FIRSTPITCH;
	int iDummy1, iDummy2;
	char szPitch[ 8 ] = "";
	CNote note(	m_iPitch,
				CMAJOR,
				NOTE_QUARTER,
				CLEF_TREBLE,
				0,
				0,
				iDummy1,
				iDummy2	);
	note.GetRelativePitch( szPitch );
	if (szPitch[ 1 ] == '#')
	{
		::MessageBox(	CApplication::GetMainWindowHandle(),
						"Use the '3' key to enter percussion note.",
						"Bach",
						MB_ICONEXCLAMATION | MB_OK	);
		m_iPitch--;
	}
	else
	{
		::MessageBox(	CApplication::GetMainWindowHandle(),
						"Use the spacebar to enter percussion note.",
						"Bach",
						MB_ICONEXCLAMATION | MB_OK	);
	}


	return TRUE;
}

BOOL CDialogPercussion::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

