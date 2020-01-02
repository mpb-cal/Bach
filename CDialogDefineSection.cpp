
#include "CDialogDefineSection.h"



CDialogDefineSection::CDialogDefineSection() :
	m_iFirstMeasure( 1 ),
	m_iLastMeasure( 1 )
{
	lstrcpy( m_szDialogResourceName, "DefineSection" );
}


BOOL CDialogDefineSection::OnInitDialog( HWND hDlg )
{
	char szText[ 512 ] = "";
	sprintf( szText, "%d", m_iFirstMeasure );
	::SetDlgItemText(	hDlg, 
						IDC_FIRSTMEASURE, 
						szText	);

	sprintf( szText, "%d", m_iLastMeasure );
	::SetDlgItemText(	hDlg, 
						IDC_LASTMEASURE, 
						szText	);

	::SetDlgItemText(	hDlg, 
						IDC_NAME, 
						m_szName	);

	return TRUE;
}

BOOL CDialogDefineSection::OnOk( HWND hDlg )
{
	char szText[ 512 ] = "";

	::SendDlgItemMessage(	hDlg, 
							IDC_FIRSTMEASURE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szText ), 
							(LPARAM)szText	);
	m_iFirstMeasure = atoi( szText );

	::SendDlgItemMessage(	hDlg, 
							IDC_LASTMEASURE, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szText ), 
							(LPARAM)szText	);
	m_iLastMeasure = atoi( szText );

	::SendDlgItemMessage(	hDlg, 
							IDC_NAME, 
							WM_GETTEXT, 
							(WPARAM)sizeof( m_szName ), 
							(LPARAM)m_szName	);

	return TRUE;
}

BOOL CDialogDefineSection::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}



