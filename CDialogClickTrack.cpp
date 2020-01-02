
#include "CDialogClickTrack.h"


CDialogClickTrack::CDialogClickTrack()
{
	lstrcpy( m_szDialogResourceName, "ClickTrack" );
}


BOOL CDialogClickTrack::OnInitDialog( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	sprintf( szEditText, "%d", m_iMeasures );
	::SetDlgItemText(	hDlg, 
						IDC_MEASURES, 
						szEditText	);

	sprintf( szEditText, "%d", m_iVoice );
	::SetDlgItemText(	hDlg, 
						IDC_VOICE, 
						szEditText	);

	return TRUE;
}

BOOL CDialogClickTrack::OnOk( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_VOICE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iVoice = atoi( szEditText );

	::SendDlgItemMessage(	hDlg, 
							IDC_MEASURES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iMeasures = atoi( szEditText );

	return TRUE;
}

BOOL CDialogClickTrack::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	return FALSE;
}




