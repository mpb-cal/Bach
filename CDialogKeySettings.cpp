
#include "CDialogKeySettings.h"
#include "Constant.h"
#include "CNote.h"



CDialogKeySettings::CDialogKeySettings()
{
	lstrcpy( m_szDialogResourceName, "KeySettings" );
}


BOOL CDialogKeySettings::OnInitDialog( HWND hDlg )
{
	// fill in key combo box and set current selection
	char szKey[ 128 ] = "";
	for (int i = CMAJOR; i < LastKeySignature; i++)
	{
		CNote::KeyToString( i, szKey );
		::SendDlgItemMessage(	hDlg, 
								IDC_KEY, 
								CB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szKey	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_KEY, 
							CB_SETCURSEL, 
							(WPARAM)m_iKey, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogKeySettings::OnOk( HWND hDlg )
{
	// retrieve key
	m_iKey = ::SendDlgItemMessage(	hDlg, 
									IDC_KEY, 
									CB_GETCURSEL, 
									(WPARAM)0, 
									(LPARAM)0	);

	return TRUE;
}

BOOL CDialogKeySettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

