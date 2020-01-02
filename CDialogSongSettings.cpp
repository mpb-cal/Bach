
#include "CDialogSongSettings.h"


const int DIALOG_NUMVOICES_MIN = 1;
const int DIALOG_NUMVOICES_MAX = 256;
const int DIALOG_TEMPO_MIN = 10;
const int DIALOG_TEMPO_MAX = 1000;


CDialogSongSettings::CDialogSongSettings()
{
	lstrcpy( m_szDialogResourceName, "SongSettings" );
}


BOOL CDialogSongSettings::OnInitDialog( HWND hDlg )
{
	// initialize allowed time sig values
	m_aiTimeSigN[ 0 ] = 2;
	m_aiTimeSigN[ 1 ] = 3;
	m_aiTimeSigN[ 2 ] = 4;
	m_aiTimeSigN[ 3 ] = 5;
	m_aiTimeSigN[ 4 ] = 6;
	m_aiTimeSigN[ 5 ] = 7;
	m_aiTimeSigN[ 6 ] = 8;
	m_aiTimeSigN[ 7 ] = 9;
	m_aiTimeSigN[ 8 ] = 10;
	m_aiTimeSigN[ 9 ] = 11;
	m_aiTimeSigN[ 10 ] = 12;
	m_aiTimeSigN[ 11 ] = 13;
	m_aiTimeSigN[ 12 ] = 14;
	m_aiTimeSigN[ 13 ] = 15;
	m_aiTimeSigN[ 14 ] = 16;

	m_aiTimeSigD[ 0 ] = 2;
	m_aiTimeSigD[ 1 ] = 4;
	m_aiTimeSigD[ 2 ] = 8;
	m_aiTimeSigD[ 3 ] = 16;

	// fill in number of voices
	char szVoices[ 16 ] = "";
	sprintf( szVoices, "%d", m_iVoices );
	::SetDlgItemText(	hDlg, 
						IDC_NUMVOICES, 
						szVoices	);

	// fill in tempo
	char szTempo[ 16 ] = "";
	sprintf( szTempo, "%d", m_iTempo );
	::SetDlgItemText(	hDlg, 
						IDC_TEMPO, 
						szTempo	);

	// fill in timesign combo box and set current selection
	char szTimeSigN[ 16 ] = "";
	int iCurrentSelection;
	for (int i = 0; i < sizeof( m_aiTimeSigN ) / sizeof( m_aiTimeSigN[ 0 ] ); i++)
	{
		sprintf( szTimeSigN, "%d", m_aiTimeSigN[ i ] );
		::SendDlgItemMessage(	hDlg, 
								IDC_TIMESIGN, 
								CB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szTimeSigN	);
		
		if (m_aiTimeSigN[ i ] == m_iTimeSigN)
		{
			iCurrentSelection = i;
		}
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_TIMESIGN, 
							CB_SETCURSEL, 
							(WPARAM)iCurrentSelection, 
							(LPARAM)0	);

	// fill in timesigd combo box and set current selection
	char szTimeSigD[ 16 ] = "";
	for (i = 0; i < sizeof( m_aiTimeSigD ) / sizeof( m_aiTimeSigD[ 0 ] ); i++)
	{
		sprintf( szTimeSigD, "%d", m_aiTimeSigD[ i ] );
		::SendDlgItemMessage(	hDlg, 
								IDC_TIMESIGD, 
								CB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szTimeSigD	);
		
		if (m_aiTimeSigD[ i ] == m_iTimeSigD)
		{
			iCurrentSelection = i;
		}
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_TIMESIGD, 
							CB_SETCURSEL, 
							(WPARAM)iCurrentSelection, 
							(LPARAM)0	);


	return TRUE;
}

BOOL CDialogSongSettings::OnOk( HWND hDlg )
{
	// retrieve number of voices
	char szVoices[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_NUMVOICES, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szVoices ), 
							(LPARAM)szVoices	);

	m_iVoices = atoi( szVoices );
	CHECK_BOUNDS( m_iVoices, NUMVOICES );

	// retrieve tempo
	char szTempo[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_TEMPO, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szTempo ), 
							(LPARAM)szTempo	);

	m_iTempo = atoi( szTempo );
	CHECK_BOUNDS( m_iTempo, TEMPO );

	// retrieve timesign
	int iCurrentSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_TIMESIGN, 
													CB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);

	m_iTimeSigN = m_aiTimeSigN[ iCurrentSelection ];

	// retrieve timesigd
	iCurrentSelection = ::SendDlgItemMessage(	hDlg, 
												IDC_TIMESIGD, 
												CB_GETCURSEL, 
												(WPARAM)0, 
												(LPARAM)0	);

	m_iTimeSigD = m_aiTimeSigD[ iCurrentSelection ];

	return TRUE;
}

BOOL CDialogSongSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}


