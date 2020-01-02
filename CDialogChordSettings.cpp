

#include "CDialogChordSettings.h"


const int DIALOG_VOICES_MIN = 2;
const int DIALOG_VOICES_MAX = 12;

CDialogChordSettings::CDialogChordSettings()
{
	lstrcpy( m_szDialogResourceName, "ChordSettings" );
}


BOOL CDialogChordSettings::OnInitDialog( HWND hDlg )
{
	// fill in number of voices
	char szVoices[ 16 ] = "";
	sprintf( szVoices, "%d", m_iVoices );
	::SetDlgItemText(	hDlg, 
						IDC_VOICES, 
						szVoices	);


	return TRUE;
}

BOOL CDialogChordSettings::OnOk( HWND hDlg )
{
	// retrieve number of voices
	char szVoices[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_VOICES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szVoices ), 
							(LPARAM)szVoices	);

	m_iVoices = atoi( szVoices );
	CHECK_BOUNDS( m_iVoices, VOICES );

	return TRUE;
}

BOOL CDialogChordSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}


