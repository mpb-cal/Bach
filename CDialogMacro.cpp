
#include "CDialogMacro.h"


const int DIALOG_REPETITIONS_MIN = 1;
const int DIALOG_REPETITIONS_MAX = 10;


CDialogMacro::CDialogMacro()
{
	lstrcpy( m_szDialogResourceName, "Macro" );
}


BOOL CDialogMacro::OnInitDialog( HWND hDlg )
{
	// fill in macro combo box
	char szMacro[ MACRO_MAXLENGTH ] = "";
	for (int i = 0; i < 1; i++)
	{
		lstrcpy( szMacro, m_szMacro );
		::SendDlgItemMessage(	hDlg, 
								IDC_MACRO, 
								CB_ADDSTRING, 
								(WPARAM)0, 
								(LPARAM)szMacro	);
	}

	// fill in number of repetitions
	char szRepetitions[ 16 ] = "";
	sprintf( szRepetitions, "%d", m_iRepetitions );
	::SetDlgItemText(	hDlg, 
						IDC_REPETITIONS, 
						szRepetitions	);


	return TRUE;
}

BOOL CDialogMacro::OnOk( HWND hDlg )
{
	// retrieve macro
	::SendDlgItemMessage(	hDlg, 
							IDC_MACRO, 
							WM_GETTEXT, 
							(WPARAM)MACRO_MAXLENGTH, 
							(LPARAM)m_szMacro	);

	// retrieve number of repetitions
	char szRepetitions[ 16 ] = "";
	::SendDlgItemMessage(	hDlg, 
							IDC_REPETITIONS, 
							WM_GETTEXT, 
							(WPARAM)sizeof( szRepetitions ), 
							(LPARAM)szRepetitions	);

	m_iRepetitions = atoi( szRepetitions );
	CHECK_BOUNDS( m_iRepetitions, REPETITIONS );

	return TRUE;
}

BOOL CDialogMacro::CommandHandler( HWND hDlg, WPARAM wParam, LPARAM lParam )
{

	return FALSE;
}


