
#include "CDialogSongStructure.h"
#include "CSection.h"



CDialogSongStructure::CDialogSongStructure()
{
	lstrcpy( m_szDialogResourceName, "SongStructure" );
}


BOOL CDialogSongStructure::OnInitDialog( HWND hDlg )
{
	char szListText[ 512 ] = "";

	for (int i = 0; i < m_pSectionsList->GetSize(); i++)
	{
		lstrcpy( szListText, ((CSection*)m_pSectionsList->operator[]( i ))->m_szName );
		::SendDlgItemMessage(	hDlg, 
								IDC_SECTIONLIST, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szListText	);
	}

	::SendDlgItemMessage(	hDlg, 
							IDC_SECTIONLIST, 
							LB_SETCURSEL, 
							(WPARAM)0, 
							(LPARAM)0	);

	for (i = 0; i < m_iSections; i++)
	{
		lstrcpy( szListText, ((CSection*)m_pSectionsList->operator[]( m_cSectionOrder[ i ] ))->m_szName );
		::SendDlgItemMessage(	hDlg, 
								IDC_SECTIONORDER, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szListText	);
	}

	if (::SendDlgItemMessage(	hDlg, 
								IDC_SECTIONORDER, 
								LB_SETCURSEL, 
								(WPARAM)0, 
								(LPARAM)0	) == -1)
	{
		::EnableWindow( ::GetDlgItem( hDlg, IDC_REMOVE ), FALSE );
		::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), FALSE );
		::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), FALSE );
	}

	return TRUE;
}

BOOL CDialogSongStructure::OnOk( HWND hDlg )
{

	return TRUE;
}

BOOL CDialogSongStructure::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{
	switch (LOWORD( wParam ))
	{
		case IDC_ADD :
		{
			int iSection = ::SendDlgItemMessage(	hDlg, 
													IDC_SECTIONLIST, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			m_cSectionOrder[ m_iSections++ ] = iSection;

			PopulateSectionOrderBox( hDlg );

			::SendDlgItemMessage(	hDlg, 
									IDC_SECTIONORDER, 
									LB_SETCURSEL, 
									(WPARAM)m_iSections - 1, 
									(LPARAM)0	);

			::EnableWindow( ::GetDlgItem( hDlg, IDC_REMOVE ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEUP ), TRUE );
			::EnableWindow( ::GetDlgItem( hDlg, IDC_MOVEDOWN ), TRUE );

			return TRUE;
		}

		case IDC_REMOVE :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_SECTIONORDER, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			for (int i = iSelection; i < m_iSections; i++)
			{
				m_cSectionOrder[ i ] = m_cSectionOrder[ i + 1 ];
			}
			m_iSections--;

			PopulateSectionOrderBox( hDlg );

			int iNewSelection = min( iSelection, m_iSections - 1 );
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

			return TRUE;
		}

		case IDC_MOVEUP :
		{
			int iSelection = ::SendDlgItemMessage(	hDlg, 
													IDC_SECTIONORDER, 
													LB_GETCURSEL, 
													(WPARAM)0, 
													(LPARAM)0	);
			
			if (iSelection > 0)
			{
				char temp = m_cSectionOrder[ iSelection - 1 ];
				m_cSectionOrder[ iSelection - 1 ] = m_cSectionOrder[ iSelection ];
				m_cSectionOrder[ iSelection ] = temp;

				PopulateSectionOrderBox( hDlg );

				int iNewSelection = min( iSelection - 1, m_iSections - 1 );
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

	}

	return FALSE;
}

void CDialogSongStructure::PopulateSectionOrderBox( HWND hDlg )
{
	char szText[ 512 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_SECTIONORDER, 
							LB_RESETCONTENT, 
							(WPARAM)0, 
							(LPARAM)0	);

	for (int i = 0; i < m_iSections; i++)
	{
		lstrcpy( szText, ((CSection*)m_pSectionsList->operator[]( m_cSectionOrder[ i ] ))->m_szName );
		::SendDlgItemMessage(	hDlg, 
								IDC_SECTIONORDER, 
								LB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szText	);
	}
}

