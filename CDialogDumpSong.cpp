
#include "CDialogDumpSong.h"



CDialogDumpSong::CDialogDumpSong()
{
	lstrcpy( m_szDialogResourceName, "DumpSong" );
}


BOOL CDialogDumpSong::OnInitDialog( HWND hDlg )
{
	UINT ui = ::SetDlgItemText(	hDlg, 
								IDC_TEXT, 
								(LPCTSTR)m_szText	);

	return TRUE;
}

BOOL CDialogDumpSong::OnOk( HWND hDlg )
{

	return TRUE;
}

BOOL CDialogDumpSong::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

