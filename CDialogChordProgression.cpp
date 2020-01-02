
#include "CApplication.h"
#include "CDialogChordProgression.h"
#include "Constant.h"


const int DIALOG_TICKS_MIN = 1;
const int DIALOG_TICKS_MAX = 32;
const int DIALOG_TICKS_START = 8;

void CPlacedChord::MakeCopy( CLinkedListItem*& rpNewCopy )
{
	CPlacedChord* pPlacedChord = new CPlacedChord;
	*pPlacedChord = *this;
	rpNewCopy = pPlacedChord;
}

BOOL CPlacedChord::Output( FILE* pFile ) const
{
	if (fwrite( &m_iChord, sizeof( m_iChord ), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( &m_iTicks, sizeof( m_iTicks), 1, pFile ) < 1 ) return FALSE;
	if (fwrite( m_szChord, sizeof( m_szChord ), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}

BOOL CPlacedChord::Input( FILE* pFile )
{
	if (fread( &m_iChord, sizeof( m_iChord), 1, pFile ) < 1) return FALSE;
	if (fread( &m_iTicks, sizeof( m_iTicks), 1, pFile ) < 1) return FALSE;
	if (fread( m_szChord, sizeof( m_szChord ), 1, pFile ) < 1 ) return FALSE;

	return TRUE;
}


CDialogChordProgression::CDialogChordProgression() :
	m_iStartMeasure( 1 )
{
	lstrcpy( m_szDialogResourceName, "ChordProgression" );
}


BOOL CDialogChordProgression::OnInitDialog( HWND hDlg )
{
	::CheckDlgButton( hDlg, IDC_CHORDBOOL, m_blChord );
	::CheckDlgButton( hDlg, IDC_BASSNOTE, m_blBassNote );
	::CheckDlgButton( hDlg, IDC_MELODY, m_blMelody);

	char szText[ 16 ] = "";

	sprintf( szText, "%d", DIALOG_TICKS_START );
	::SetDlgItemText(	hDlg, 
						IDC_TICKS, 
						szText	);

	sprintf( szText, "%d", m_iStartMeasure );
	::SetDlgItemText(	hDlg, 
						IDC_STARTMEASURE, 
						szText	);

	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"C" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Db" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"D" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Eb" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"E" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"F" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Gb" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"G" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Ab" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"A" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"Bb" );
	::SendDlgItemMessage( hDlg, IDC_RT, LB_ADDSTRING, (WPARAM)0, (LPARAM)"B" );

	::SendDlgItemMessage(	hDlg, 
							IDC_RT, 
							LB_SETCURSEL, 
							(WPARAM)0, 
							(LPARAM)0	);

	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"M" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"M7" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m7" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m9" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m11" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"7" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"b9" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"9" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"13" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"13b9" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m7b5" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"dim7" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"6" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"m6" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"+" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"+7" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"sus4" );
	::SendDlgItemMessage( hDlg, IDC_CHORD, LB_ADDSTRING, (WPARAM)0, (LPARAM)"7sus4" );

	::SendDlgItemMessage(	hDlg, 
							IDC_CHORD, 
							LB_SETCURSEL, 
							(WPARAM)0, 
							(LPARAM)0	);



	return TRUE;
}

BOOL CDialogChordProgression::OnOk( HWND hDlg )
{
	char szStartMeasure[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_STARTMEASURE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szStartMeasure ), 
							(LPARAM)szStartMeasure	);

	m_iStartMeasure = atoi( szStartMeasure );

	m_blChord = ::IsDlgButtonChecked( hDlg, IDC_CHORDBOOL );
	m_blBassNote = ::IsDlgButtonChecked( hDlg, IDC_BASSNOTE );
	m_blMelody = ::IsDlgButtonChecked( hDlg, IDC_MELODY );

	return TRUE;
}

BOOL CDialogChordProgression::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_HISTORY :
		{
			switch (HIWORD( wParam ))
			{
				case LBN_DBLCLK :
				{
					::SendMessage(	hDlg, 
									WM_COMMAND, 
									MAKEWPARAM( IDC_ADDHISTORY, 0 ),
									(LPARAM)NULL	);

					return TRUE;
				}
			}

			return FALSE;
		}

		case IDC_CHORD :
		{
			switch (HIWORD( wParam ))
			{
				case LBN_DBLCLK :
				{
					::SendMessage(	hDlg, 
									WM_COMMAND, 
									MAKEWPARAM( IDC_ADDCHORD, 0 ),
									(LPARAM)NULL	);

					return TRUE;
				}
			}

			return FALSE;
		}

		case IDC_RT :
		{
			switch (HIWORD( wParam ))
			{
				case LBN_DBLCLK :
				{
					::SendMessage(	hDlg, 
									WM_COMMAND, 
									MAKEWPARAM( IDC_ADDCHORD, 0 ),
									(LPARAM)NULL	);

					return TRUE;
				}
			}

			return FALSE;
		}

		case IDC_ADDCHORD :
		{
			int iRoot = ::SendDlgItemMessage(	hDlg, 
												IDC_RT, 
												LB_GETCURSEL, 
												(WPARAM)0, 
												(LPARAM)0	);

			char szRoot[ 16 ] = "";
			::SendDlgItemMessage(	hDlg, 
									IDC_RT, 
									LB_GETTEXT, 
									(WPARAM)iRoot, 
									(LPARAM)szRoot	);
			
			int iChord = ::SendDlgItemMessage(	hDlg, 
												IDC_CHORD, 
												LB_GETCURSEL, 
												(WPARAM)0, 
												(LPARAM)0	);

			char szChord[ 16 ] = "";
			::SendDlgItemMessage(	hDlg, 
									IDC_CHORD, 
									LB_GETTEXT, 
									(WPARAM)iChord, 
									(LPARAM)szChord	);

			lstrcat( szRoot, szChord );			

			char szTicks[ 16 ] = "";
			::SendDlgItemMessage(	hDlg, 
									IDC_TICKS, 
									WM_GETTEXT, 
									(WPARAM)sizeof( szTicks ), 
									(LPARAM)szTicks	);

			int iTicks = atoi( szTicks );
			CHECK_BOUNDS( iTicks , TICKS );

			CPlacedChord* pPlacedChord = new CPlacedChord( szRoot, iRoot * CHORD_TYPES + iChord, iTicks );
			m_PlacedChordList.AppendItem( pPlacedChord );

			PopulateHistoryBox( hDlg );
			PopulateProgressionBox( hDlg );

			::SendDlgItemMessage(	hDlg, 
									IDC_PROGRESSION, 
									LB_SETCURSEL, 
									(WPARAM)(m_PlacedChordList.GetSize() - 1), 
									(LPARAM)0	);

			::EnableWindow( ::GetDlgItem( hDlg, IDC_REM ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), TRUE );

			return TRUE;
		}

		case IDC_ADDHISTORY :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_HISTORY, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);

			if (iSelection == LB_ERR)
			{
				return TRUE;
			}

			char szChord[ 16 ] = "";
			::SendDlgItemMessage(	hDlg, 
									IDC_HISTORY, 
									LB_GETTEXT, 
									(WPARAM)iSelection, 
									(LPARAM)szChord	);

						
			char szTicks[ 16 ] = "";
			::SendDlgItemMessage(	hDlg, 
									IDC_TICKS, 
									WM_GETTEXT, 
									(WPARAM)sizeof( szTicks ), 
									(LPARAM)szTicks	);

			int iTicks = atoi( szTicks );
			CHECK_BOUNDS( iTicks , TICKS );

			CPlacedChord* pPlacedChord = new CPlacedChord(
				((CPlacedChord*)m_HistoryList[ iSelection ])->m_szChord,
				((CPlacedChord*)m_HistoryList[ iSelection ])->m_iChord,
				iTicks	);

			m_PlacedChordList.AppendItem( pPlacedChord );

			PopulateProgressionBox( hDlg );

			::SendDlgItemMessage(	hDlg, 
									IDC_PROGRESSION, 
									LB_SETCURSEL, 
									(WPARAM)(m_PlacedChordList.GetSize() - 1), 
									(LPARAM)0	);

			::EnableWindow( ::GetDlgItem( hDlg, IDC_REM ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), TRUE );

			return TRUE;
		}

		case IDC_REM :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_PROGRESSION, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			m_PlacedChordList.RemoveItem( iSelection );

			PopulateProgressionBox( hDlg );

			int iNewSelection = min( iSelection, m_PlacedChordList.GetSize() - 1 );
			if (iNewSelection >= 0)
			{
				::SendDlgItemMessage(	hDlg, 
										IDC_PROGRESSION, 
										LB_SETCURSEL, 
										(WPARAM)iNewSelection, 
										(LPARAM)0	);
			}
			else
			{
				::EnableWindow( ::GetDlgItem( hDlg, IDC_REMOVE ), FALSE );
				::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), FALSE );
				::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), FALSE );
			}

			return TRUE;
		}
/*
		case IDC_MOVEUP :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_PROGRESSION, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			m_PlacedChordList.RemoveItem( iSelection );

			PopulateProgressionBox( hDlg );

			int iNewSelection = min( iSelection, m_PlacedChordList.GetSize() - 1 );
			if (iNewSelection >= 0)
			{
				::SendDlgItemMessage(	hDlg, 
										IDC_PROGRESSION, 
										LB_SETCURSEL, 
										(WPARAM)iNewSelection, 
										(LPARAM)0	);
			}
			else
			{
				::EnableWindow( ::GetDlgItem( hDlg, IDC_REMOVE ), FALSE );
				::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), FALSE );
				::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), FALSE );
			}

			return TRUE;
		}

		case IDC_MOVEDOWN :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_SECTIONORDER, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			if (iSelection < m_iSections - 1)
			{
				char temp = m_cSectionOrder[ iSelection ];
				m_cSectionOrder[ iSelection ] = m_cSectionOrder[ iSelection + 1 ];
				m_cSectionOrder[ iSelection + 1 ] = temp;

				PopulateSectionOrderBox( hDlg );

				int iNewSelection = min( iSelection + 1, m_iSections - 1 );
				if (iNewSelection >= 0)
				{
					::SendDlgItemMessage(	hDlg, 
											IDC_SECTIONORDER, 
											LB_SETCURSEL, 
											(WPARAM)iNewSelection, 
											(LPARAM)0	);
				}
				else
				{
					::EnableWindow( ::GetDlgItem( hDlg, IDC_REMOVE ), FALSE );
					::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), FALSE );
					::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), FALSE );
				}
			}

			return TRUE;
		}
*/
		case IDC_CLEARHISTORY :
		{
			return TRUE;
		}

		case IDC_OPEN :
		{
			char szFileName[ MAX_PATH ] = "";

			OPENFILENAME ofn;
			::ZeroMemory( &ofn, sizeof OPENFILENAME );
			
			ofn.lStructSize			= sizeof OPENFILENAME;
			ofn.hwndOwner			= CApplication::GetMainWindowHandle();
			ofn.hInstance			= CApplication::GetInstance();
			ofn.lpstrFilter			= "Chord Progressions(*.bcp)\0*.bcp\0";
			ofn.lpstrCustomFilter	= NULL;
			ofn.nMaxCustFilter		= 0;
			ofn.nFilterIndex		= 0;
			ofn.lpstrFile			= szFileName;
			ofn.nMaxFile			= MAX_PATH;
			ofn.lpstrFileTitle		= NULL;
			ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
			ofn.lpstrInitialDir		= NULL;
			ofn.lpstrTitle			= NULL;
			ofn.Flags				= OFN_CREATEPROMPT | OFN_EXPLORER;
			ofn.nFileOffset			= 0;
			ofn.nFileExtension		= 0;
			ofn.lpstrDefExt			= "bcp";
			ofn.lCustData			= 0L;
			ofn.lpfnHook			= NULL;
			ofn.lpTemplateName		= NULL;

			if (GetOpenFileName( &ofn ))
			{
				// read the file
				FILE* pFile;
				if ((pFile = fopen( ofn.lpstrFile, "rb" )) == NULL) return FALSE;
				int iChords;
				if (fread( &iChords, sizeof( iChords ), 1, pFile ) < 1 ) return FALSE;
				m_PlacedChordList.RemoveAll();
				for (int i = 0; i < iChords; i++)
				{
					m_PlacedChordList.AppendItem( new CPlacedChord );
				}

				m_PlacedChordList.Input( pFile );
				fclose( pFile );

				PopulateHistoryBox( hDlg );
				PopulateProgressionBox( hDlg );
			}

			break;
		}

		case IDC_SAVE :
		{
			char szFileName[ MAX_PATH ] = "";

			OPENFILENAME ofn;
			::ZeroMemory( &ofn, sizeof OPENFILENAME );
			
			ofn.lStructSize			= sizeof OPENFILENAME;
			ofn.hwndOwner			= CApplication::GetMainWindowHandle();
			ofn.hInstance			= CApplication::GetInstance();
			ofn.lpstrFilter			= "Chord Progressions(*.bcp)\0*.bcp\0";
			ofn.lpstrCustomFilter	= NULL;
			ofn.nMaxCustFilter		= 0;
			ofn.nFilterIndex		= 0;
			ofn.lpstrFile			= szFileName;
			ofn.nMaxFile			= MAX_PATH;
			ofn.lpstrFileTitle		= NULL;
			ofn.nMaxFileTitle		= _MAX_FNAME + _MAX_EXT;
			ofn.lpstrInitialDir		= NULL;
			ofn.lpstrTitle			= NULL;
			ofn.Flags				= OFN_OVERWRITEPROMPT;
			ofn.nFileOffset			= 0;
			ofn.nFileExtension		= 0;
			ofn.lpstrDefExt			= "bcp";
			ofn.lCustData			= 0L;
			ofn.lpfnHook			= NULL;
			ofn.lpTemplateName		= NULL;

			if (::GetSaveFileName( &ofn ))
			{
				FILE* pFile;
				if ((pFile = fopen( ofn.lpstrFile, "wb" )) == NULL) return FALSE;
				int iChords = m_PlacedChordList.GetSize();
				if (fwrite( &iChords, sizeof( iChords ), 1, pFile ) < 1 ) return FALSE;
				m_PlacedChordList.Output( pFile );
				fclose( pFile );

				return TRUE;
			}

			break;
		}

	}

	return FALSE;
}

void CDialogChordProgression::PopulateProgressionBox( HWND hDlg )
{
	::SendDlgItemMessage(	hDlg, 
							IDC_PROGRESSION, 
							LB_RESETCONTENT, 
							(WPARAM)0, 
							(LPARAM)0	);

	for (int i = 0; i < m_PlacedChordList.GetSize(); i++)
	{
		char szChord[ 512 ];
		char szTicks[ 512 ];
		itoa( ((CPlacedChord*)m_PlacedChordList[ i ])->m_iTicks, szTicks, 10 );

		lstrcpy( szChord, ((CPlacedChord*)m_PlacedChordList[ i ])->m_szChord );
		while (lstrlen( szChord ) < 6)
		{
			lstrcat( szChord, " " );
		}
		lstrcat( szChord, "     " );
		lstrcat( szChord, szTicks );

		::SendDlgItemMessage(	hDlg, 
								IDC_PROGRESSION, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szChord	);
	}
}

void CDialogChordProgression::PopulateHistoryBox( HWND hDlg )
{
	::SendDlgItemMessage(	hDlg, 
							IDC_HISTORY, 
							LB_RESETCONTENT, 
							(WPARAM)0, 
							(LPARAM)0	);

	m_HistoryList.RemoveAll();

	for (int i = 0; i < m_PlacedChordList.GetSize(); i++)
	{
		BOOL blAlreadyPresent = FALSE;

		for (int j = 0; j < m_HistoryList.GetSize(); j++)
		{
			if (lstrcmp(
				((CPlacedChord*)m_PlacedChordList[ i ])->m_szChord, 
				((CPlacedChord*)m_HistoryList[ j ])->m_szChord ) 
				== 0)
			{
				blAlreadyPresent = TRUE;
				break;
			}
		}

		if (!blAlreadyPresent)
		{
			CPlacedChord* pHistoryChord = new CPlacedChord;
			*pHistoryChord = *((CPlacedChord*)(m_PlacedChordList[ i ]));
			m_HistoryList.AppendItem( pHistoryChord );
		}
	}

	for (i = 0; i < m_HistoryList.GetSize(); i++)
	{
		char szChord[ 512 ];

		lstrcpy( szChord, ((CPlacedChord*)m_HistoryList[ i ])->m_szChord );

		::SendDlgItemMessage(	hDlg, 
								IDC_HISTORY, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szChord	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_HISTORY, 
							LB_SETCURSEL, 
							(WPARAM)(m_HistoryList.GetSize() - 1), 
							(LPARAM)0	);

}

