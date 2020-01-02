
#include "CDialogDrumPart.h"


CDialogDrumPart::CDialogDrumPart()
{
	lstrcpy( m_szDialogResourceName, "DrumPart" );
}


BOOL CDialogDrumPart::OnInitDialog( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::CheckRadioButton(	hDlg, 
						IDC_ROCK1, 
						IDC_SWING1, 
						m_iStyle + IDC_ROCK1	);

	sprintf( szEditText, "%d", m_iKick );
	::SetDlgItemText(	hDlg, 
						IDC_KICK, 
						szEditText	);

	sprintf( szEditText, "%d", m_iSnare );
	::SetDlgItemText(	hDlg, 
						IDC_SNARE, 
						szEditText	);

	sprintf( szEditText, "%d", m_iHiHat );
	::SetDlgItemText(	hDlg, 
						IDC_HIHAT, 
						szEditText	);

	sprintf( szEditText, "%d", m_iTom1 );
	::SetDlgItemText(	hDlg, 
						IDC_TOM1, 
						szEditText	);

	sprintf( szEditText, "%d", m_iTom2 );
	::SetDlgItemText(	hDlg, 
						IDC_TOM2, 
						szEditText	);

	sprintf( szEditText, "%d", m_iStartMeasure );
	::SetDlgItemText(	hDlg, 
						IDC_STARTATMEASURE, 
						szEditText	);

	sprintf( szEditText, "%d", m_iMeasures );
	::SetDlgItemText(	hDlg, 
						IDC_MEASURES, 
						szEditText	);


	return TRUE;
}

BOOL CDialogDrumPart::OnOk( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_MEASURES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iMeasures = atoi( szEditText );

	::SendDlgItemMessage(	hDlg, 
							IDC_STARTATMEASURE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText	);

	m_iStartMeasure = atoi( szEditText );

	return TRUE;
}

BOOL CDialogDrumPart::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_ROCK1 :
		case IDC_ROCK2 :
		case IDC_TECHNO1 :
		case IDC_SKA1 :
		case IDC_HIPHOP1 :
		case IDC_SWING1 :
		{
			m_iStyle = LOWORD( wParam ) - IDC_ROCK1;
			return TRUE;
		}
	}

	return FALSE;
}




