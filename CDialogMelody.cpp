
#include "CDialogMelody.h"


const int DIALOG_MEASURES_MIN = 1;
const int DIALOG_MEASURES_MAX = 256;

CDialogMelody::CDialogMelody()
{
	lstrcpy( m_szDialogResourceName, "Melody" );
}


BOOL CDialogMelody::OnInitDialog( HWND hDlg )
{
	// fill in number of measures
	char szMeasures[ 16 ] = "";
	sprintf( szMeasures, "%d", m_iMeasures );
	::SetDlgItemText(	hDlg, 
						IDC_MEASURES, 
						szMeasures	);

	return TRUE;
}

BOOL CDialogMelody::OnOk( HWND hDlg )
{
	// retrieve number of measures
	char szMeasures[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_MEASURES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szMeasures ), 
							(LPARAM)szMeasures	);

	m_iMeasures = atoi( szMeasures );
	CHECK_BOUNDS( m_iMeasures, MEASURES );

	return TRUE;
}

BOOL CDialogMelody::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}



