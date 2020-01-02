
#include "CDialogTransposeDiatonic.h"



CDialogTransposeDiatonic::CDialogTransposeDiatonic()
{
	lstrcpy( m_szDialogResourceName, "TransposeDiatonic" );
}


BOOL CDialogTransposeDiatonic::OnInitDialog( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::CheckRadioButton(	hDlg, 
						IDC_UP, 
						IDC_DOWN, 
						m_iDirection + IDC_UP	);

	sprintf( szEditText, "%d", m_iPitches );
	::SetDlgItemText(	hDlg, 
						IDC_PITCHES, 
						szEditText	);



	return TRUE;
}

BOOL CDialogTransposeDiatonic::OnOk( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_PITCHES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iPitches = atoi( szEditText );

	return TRUE;
}

BOOL CDialogTransposeDiatonic::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_UP :
		case IDC_DOWN :
		{
			m_iDirection = LOWORD( wParam ) - IDC_UP;
			return TRUE;
		}
	}

	return FALSE;
}

