
#include "DialogSongSettings.h"

#include "resource.h"

#include <stdio.h>

const int NUMVOICES_MIN = 1;
const int NUMVOICES_MAX = 256;


int m_iVoices = 8;
char m_szVoices[ 16 ];

char _szOutOfBoundsFormat[ 512 ] = "Please enter an integer between %d and %d.";

#define CHECK_BOUNDS( x, y )	if (x < ##y##_MIN || x > ##y##_MAX)	\
	{	\
		char _szOutOfBounds[ 512 ];	\
		sprintf( _szOutOfBounds, _szOutOfBoundsFormat, ##y##_MIN, ##y##_MAX );				\
		MessageBox( hDlg, _szOutOfBounds, "Bach", MB_OK | MB_ICONEXCLAMATION );	\
		SendDlgItemMessage(	hDlg,												\
							IDC_##y##,										\
							EM_SETSEL,										\
							(WPARAM)0,											\
							(LPARAM)-1	);										\
																				\
		return TRUE;															\
	}



BOOL CALLBACK DialogProcSongSettings( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch (iMsg)
	{
		case WM_INITDIALOG :
		{
			sprintf( m_szVoices, "%d", m_iVoices );
			SetDlgItemText(	hDlg, 
								IDC_NUMVOICES, 
								m_szVoices	);

			return TRUE;
		}
		case WM_COMMAND :
		{	
			switch (LOWORD( wParam ))
			{
				case IDOK :
				{
					// retrieve number of voices
					SendDlgItemMessage(	hDlg, 
										IDC_NUMVOICES, 
										EM_GETLINE, 
										(WPARAM)0, 
										(LPARAM)m_szVoices	);

					m_iVoices = atoi( m_szVoices );
					CHECK_BOUNDS( m_iVoices, NUMVOICES );

					EndDialog( hDlg, 0 );
					return TRUE;
				}
				case IDCANCEL :
				{	
					EndDialog( hDlg, 0 );
					return TRUE;
				}
				case IDC_NUMVOICES :
				{
					return TRUE;
				}
			}

			break;
		}
	}

	return FALSE;
}
