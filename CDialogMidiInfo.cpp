

#include "CDialogMidiInfo.h"



CDialogMidiInfo::CDialogMidiInfo()
{
	lstrcpy( m_szDialogResourceName, "MidiInfo" );
}


BOOL CDialogMidiInfo::OnInitDialog( HWND hDlg )
{
	char szText[ 16 ] = "";

	// fill in file format
	sprintf( szText, "%d", m_iFileFormat );
	::SetDlgItemText(	hDlg, 
						IDC_FILEFORMAT, 
						szText	);

	// fill in tracks
	sprintf( szText, "%d", m_iTracks );
	::SetDlgItemText(	hDlg, 
						IDC_TRACKS, 
						szText	);

	// fill in tempo
	sprintf( szText, "%d", m_iTempo );
	::SetDlgItemText(	hDlg, 
						IDC_TEMPO, 
						szText	);

	// fill in list view headers


	return TRUE;
}

BOOL CDialogMidiInfo::OnOk( HWND hDlg )
{

	return TRUE;
}

BOOL CDialogMidiInfo::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}


