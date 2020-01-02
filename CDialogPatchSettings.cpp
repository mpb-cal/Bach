
#include "CDialogPatchSettings.h"
#include "Constant.h"
#include "GlobalDefs.h"


CDialogPatchSettings::CDialogPatchSettings()
{
	lstrcpy( m_szDialogResourceName, "PatchSettings" );
}


BOOL CDialogPatchSettings::OnInitDialog( HWND hDlg )
{
	// fill in patch combo box and set current selection
	char szPatch[ 128 ] = "";
	for (int i = 0; i < VOICE_TOTALPATCHES; i++)
	{
		lstrcpy( szPatch, g_pszPatches[ i ] );
		::SendDlgItemMessage(	hDlg, 
								IDC_PATCH, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szPatch	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_PATCH, 
							LB_SETCURSEL, 
							(WPARAM)m_iPatch, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogPatchSettings::OnOk( HWND hDlg )
{
	// retrieve patch
	m_iPatch = ::SendDlgItemMessage(	hDlg, 
										IDC_PATCH, 
										LB_GETCURSEL, 
										(WPARAM)0, 
										(LPARAM)0	);

	return TRUE;
}

BOOL CDialogPatchSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

