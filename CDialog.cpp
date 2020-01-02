
#include <stdio.h>

#include "Dialog.h"

#include "CDialogDrumPart.h"

#include "resource.h"



const char DIALOGRESOURCENAME[] = "DrumPart";

static CDialogDrumPart* g_pThis = NULL;


CDialogDrumPart::CDialogDrumPart()
{
	if (!g_pThis)
	{
		g_pThis = this;
	}
}

CDialogDrumPart::~CDialogDrumPart()
{
	g_pThis = NULL;
}

BOOL CDialogDrumPart::DoModal( HINSTANCE hInstance, HWND hwndParent )
{
	if (g_pThis)
	{
		return ::DialogBox(	hInstance, 
							DIALOGRESOURCENAME, 
							hwndParent, 
							(DLGPROC)DialogProcStubDrumPart	);
	}

	return FALSE;
}

BOOL CDialogDrumPart::DialogProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch (iMsg)
	{
		case WM_INITDIALOG :
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
		case WM_COMMAND :
		{	
			switch (LOWORD( wParam ))
			{
				case IDOK :
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



					::EndDialog( hDlg, TRUE );
					return TRUE;
				}
				case IDCANCEL :
				{	
					::EndDialog( hDlg, FALSE );
					return TRUE;
				}

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

			break;
		}
	}

	return FALSE;
}


BOOL CALLBACK DialogProcStubDrumPart( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	if (g_pThis)
	{
		return g_pThis->DialogProc( hDlg, iMsg, wParam, lParam );
	}

	return FALSE;
}


