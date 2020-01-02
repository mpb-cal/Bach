
#include "CDialogRecord.h"


const int DIALOG_VOICE_MIN = 1;
const int DIALOG_VOICE_MAX = 256;


CDialogRecord::CDialogRecord()
{
	lstrcpy( m_szDialogResourceName, "RecordSettings" );
}


BOOL CDialogRecord::OnInitDialog( HWND hDlg )
{
	char szVoice[ 16 ] = "";
	sprintf( szVoice, "%d", m_iVoice );
	::SetDlgItemText(	hDlg, 
						IDC_VOICE, 
						szVoice	);

	char szMaxTime[ 16 ] = "";
	sprintf( szMaxTime, "%d", m_iMaxTime );
	::SetDlgItemText(	hDlg, 
						MAXTIME, 
						szMaxTime	);

	::CheckRadioButton(	hDlg, 
						IDC_11025, 
						IDC_11025,
						m_iSampleRate + IDC_11025	);

	::CheckRadioButton(	hDlg, 
						IDC_8BIT, 
						IDC_16BIT,
						m_iBitsPerSample + IDC_8BIT	);

	::CheckRadioButton(	hDlg, 
						IDC_MONO, 
						IDC_STEREO,
						m_iMode + IDC_MONO	);

	return TRUE;
}

BOOL CDialogRecord::OnOk( HWND hDlg )
{
	char szEditText[ 16 ] = "";

	// retrieve voice
	::SendDlgItemMessage(	hDlg, 
							IDC_VOICE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText );

	m_iVoice = atoi( szEditText );

	// retrieve max time
	::SendDlgItemMessage(	hDlg, 
							MAXTIME, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szEditText ), 
							(LPARAM)szEditText 	);

	m_iMaxTime = atoi( szEditText );

	return TRUE;
}

BOOL CDialogRecord::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_11025 :
		{
			m_iSampleRate = LOWORD( wParam ) - IDC_11025;
			return TRUE;
		}

		case IDC_8BIT :
		case IDC_16BIT :
		{
			m_iBitsPerSample = LOWORD( wParam ) - IDC_8BIT;
			return TRUE;
		}

		case IDC_MONO :
		case IDC_STEREO :
		{
			m_iMode = LOWORD( wParam ) - IDC_MONO;
			return TRUE;
		}
	}

	return FALSE;
}


