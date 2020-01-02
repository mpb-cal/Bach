
#include "CDialogHarmonize.h"
#include "Constant.h"


const int DIALOG_CHANNEL_MIN = CHANNEL_MIN;
const int DIALOG_CHANNEL_MAX = CHANNEL_MAX;

CDialogHarmonize::CDialogHarmonize()
{
	lstrcpy( m_szDialogResourceName, "Harmonize" );
}


BOOL CDialogHarmonize::OnInitDialog( HWND hDlg )
{
	char szVoice[ 16 ] = "";
	sprintf( szVoice, "%d", m_iVoice );
	::SetDlgItemText(	hDlg, 
						IDC_VOICE, 
						szVoice	);

	char szMeasures[ 16 ] = "";
	sprintf( szMeasures, "%d", m_iMeasures );
	::SetDlgItemText(	hDlg, 
						IDC_MEASURES, 
						szMeasures	);

	::SendDlgItemMessage( hDlg, IDC_GRANULARITY, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Eighth" );
	::SendDlgItemMessage( hDlg, IDC_GRANULARITY, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Quarter" );
	::SendDlgItemMessage( hDlg, IDC_GRANULARITY, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Half" );
	::SendDlgItemMessage( hDlg, IDC_GRANULARITY, CB_ADDSTRING, (WPARAM)0, (LPARAM)"Whole" );

	::SendDlgItemMessage(	hDlg, 
							IDC_GRANULARITY, 
							CB_SETCURSEL, 
							(WPARAM)m_iGranularity, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogHarmonize::OnOk( HWND hDlg )
{
	// retrieve voice
	char szVoice[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_VOICE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szVoice ), 
							(LPARAM)szVoice	);

	m_iVoice = atoi( szVoice );

	// retrieve measures
	char szMeasures[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_MEASURES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szMeasures ), 
							(LPARAM)szMeasures	);

	m_iMeasures = atoi( szMeasures );

	// retrieve granularity
	m_iGranularity = ::SendDlgItemMessage(	hDlg, 
											IDC_GRANULARITY, 
											CB_GETCURSEL, 
											(WPARAM)0, 
											(LPARAM)0	);
	switch (m_iGranularity)
	{
		case 0 : lstrcpy( m_szGranularity, "Eighth" ); break;
		case 1 : lstrcpy( m_szGranularity, "Quarter" ); break;
		case 2 : lstrcpy( m_szGranularity, "Half" ); break;
		case 3 : lstrcpy( m_szGranularity, "Whole" ); break;
	}

	return TRUE;
}

BOOL CDialogHarmonize::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}

