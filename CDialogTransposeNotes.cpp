
#include "CDialogTransposeNotes.h"



CDialogTransposeNotes::CDialogTransposeNotes()
{
	lstrcpy( m_szDialogResourceName, "TransposeNotes" );
}


BOOL CDialogTransposeNotes::OnInitDialog( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::CheckRadioButton(	hDlg, 
						IDC_UP, 
						IDC_DOWN, 
						m_iDirection + IDC_UP	);

	sprintf( szEditText, "%d", m_iHalfSteps );
	::SetDlgItemText(	hDlg, 
						IDC_HALFSTEPS, 
						szEditText	);



	return TRUE;
}

BOOL CDialogTransposeNotes::OnOk( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_HALFSTEPS, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iHalfSteps = atoi( szEditText );

	return TRUE;
}

BOOL CDialogTransposeNotes::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
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

