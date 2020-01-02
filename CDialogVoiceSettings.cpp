
#include "CDialogVoiceSettings.h"
#include "Constant.h"
#include "GlobalDefs.h"


const int DIALOG_CHANNEL_MIN = CHANNEL_MIN;
const int DIALOG_CHANNEL_MAX = CHANNEL_MAX;
const int DIALOG_VELOCITY_MIN = VELOCITY_MIN;
const int DIALOG_VELOCITY_MAX = VELOCITY_MAX;

CDialogVoiceSettings::CDialogVoiceSettings()
{
	lstrcpy( m_szDialogResourceName, "VoiceSettings" );
}


BOOL CDialogVoiceSettings::OnInitDialog( HWND hDlg )
{
	// fill in voice
	char szVoice[ 16 ] = "";
	sprintf( szVoice, "%d", m_iVoice );
	::SetDlgItemText(	hDlg, 
						IDC_VOICE, 
						szVoice	);

	// fill in velocity
	char szVelocity[ 16 ] = "";
	sprintf( szVelocity, "%d", m_iVelocity );
	::SetDlgItemText(	hDlg, 
						IDC_VELOCITY, 
						szVelocity	);

	// fill in channel
	char szChannel[ 16 ] = "";
	sprintf( szChannel, "%d", m_iChannel );
	::SetDlgItemText(	hDlg, 
						IDC_CHANNEL, 
						szChannel	);

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

	// fill in clef combo box and set current selection
	char szClef[ 16 ] = "";
	for (i = 0; i < LastClef; i++)
	{
		switch (i)
		{
			case CLEF_TREBLE : lstrcpy( szClef, "Treble" ); break;
			case CLEF_BASS : lstrcpy( szClef, "Bass" ); break;
		}

		::SendDlgItemMessage(	hDlg, 
								IDC_CLEF, 
								CB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szClef	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_CLEF, 
							CB_SETCURSEL, 
							(WPARAM)m_iClef, 
							(LPARAM)0	);


	// set Mute checkbox
	CheckDlgButton( hDlg, IDC_MUTE, m_blMute );

	return TRUE;
}

BOOL CDialogVoiceSettings::OnOk( HWND hDlg )
{
	// retrieve channel
	char szChannel[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_CHANNEL, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szChannel ), 
							(LPARAM)szChannel	);

	m_iChannel = atoi( szChannel );
	CHECK_BOUNDS( m_iChannel, CHANNEL );

	// retrieve velocity
	char szVelocity[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_VELOCITY, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szVelocity ), 
							(LPARAM)szVelocity	);

	m_iVelocity = atoi( szVelocity );
	CHECK_BOUNDS( m_iVelocity, VELOCITY );

	// retrieve patch
	m_iPatch = ::SendDlgItemMessage(	hDlg, 
										IDC_PATCH, 
										LB_GETCURSEL, 
										(WPARAM)0, 
										(LPARAM)0	);


	// retrieve clef
	m_iClef = ::SendDlgItemMessage(	hDlg, 
									IDC_CLEF, 
									CB_GETCURSEL, 
									(WPARAM)0, 
									(LPARAM)0	);



	// retrieve Mute
	m_blMute = IsDlgButtonChecked( hDlg, IDC_MUTE );

	return TRUE;
}

BOOL CDialogVoiceSettings::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_COLOR :
		{
			CHOOSECOLOR	cc;
			COLORREF	crCustColors[ 16 ];

			cc.lStructSize		= sizeof( CHOOSECOLOR );
			cc.hwndOwner		= NULL;
			cc.hInstance		= NULL;
			cc.rgbResult		= m_crColor;
			cc.lpCustColors		= crCustColors;
			cc.Flags			= CC_RGBINIT | CC_FULLOPEN;
			cc.lCustData		= 0L;
			cc.lpfnHook			= NULL;
			cc.lpTemplateName	= NULL;

			if (ChooseColor( &cc ) == TRUE)
			{
				m_crColor = cc.rgbResult;
			}

			return TRUE;
		}
	}

	return FALSE;
}

